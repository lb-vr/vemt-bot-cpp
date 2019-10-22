#define _CRT_SECURE_NO_WARNINGS
#include "Logger.hpp"
#include <mutex>
#include <memory>
#include <iomanip>
#include <thread>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <array>
#include <algorithm>

namespace {
std::mutex _mutex;
std::array<wlib::Logger::Destination, wlib::Logger::kLevelNum> _distinations
{ wlib::Logger::Destination::kOut, wlib::Logger::Destination::kOut, wlib::Logger::Destination::kOut, wlib::Logger::Destination::kOut,
	wlib::Logger::Destination::kErr, wlib::Logger::Destination::kErr, wlib::Logger::Destination::kErr };
std::array<bool, wlib::Logger::kLevelNum> _enable_level{ false, false, false, true, true, true, true };
bool _stdout_enable = false, _stderr_enable = true, _delete_empty_line = false;
// String Coloring
#if defined(__unix__) || defined(__linux__)
std::string changeColorToGreen(void) { return "\x1b[1m\x1b[32m"; }
std::string changeColorToRed(void) { return "\x1b[1m\x1b[31m"; }
std::string changeColorToYellow(void) { return "\x1b[1m\x1b[33m"; }
std::string changeColorToFatal(void) { return "\x1b[41m\x1b[30m"; }
std::string resetColorStr(void) { return "\x1b[0m"; }
void resetColorPrc(void) {}
#elif defined(_WIN64) || defined(_WIN32)
#include <Windows.h>
std::string changeColorToGreen(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY); return ""; }
std::string changeColorToRed(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY); return ""; }
std::string changeColorToYellow(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); return ""; }
std::string changeColorToFatal(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_INTENSITY); return ""; }
std::string resetColorStr(void) { return ""; }
void resetColorPrc(void){ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); }
#endif
// redirect in deleter
std::streambuf * _cout_buf = nullptr;
std::streambuf * _cerr_buf = nullptr;
struct _cout_deleter { void operator()(wlib::LoggerStream * ptr) { std::cout.rdbuf(_cout_buf); } };
struct _cerr_deleter { void operator()(wlib::LoggerStream * ptr) { std::cerr.rdbuf(_cerr_buf); } };
std::unique_ptr<wlib::LoggerStream, _cout_deleter> _cout;
std::unique_ptr<wlib::LoggerStream, _cerr_deleter> _cerr;
};

// Extern variable instances
wlib::LoggerStream wlib::trace(wlib::Logger::kTrace);
wlib::LoggerStream wlib::perf(wlib::Logger::kPerformance);
wlib::LoggerStream wlib::debug(wlib::Logger::kDebug);
wlib::LoggerStream wlib::info(wlib::Logger::kInfo);
wlib::LoggerStream wlib::warn(wlib::Logger::kWarning);
wlib::LoggerStream wlib::error(wlib::Logger::kError);
wlib::LoggerStream wlib::fatal(wlib::Logger::kFatal);

void wlib::Logger::setRedirectionCout(const Level dst_level){
	if (dst_level != kLevelNum) {
		_cout = std::unique_ptr<wlib::LoggerStream, _cout_deleter>(new LoggerStream(dst_level));
		_cout_buf = std::cout.rdbuf(_cout->rdbuf());
	}
	else wAssertM(false, "Invalid argument. dst_level cannot be kLevelNum.");
}

void wlib::Logger::setRedirectionCerr(const Level dst_level){
	if (dst_level != kLevelNum) {
		_cerr = std::unique_ptr<wlib::LoggerStream, _cerr_deleter>(new LoggerStream(dst_level));
		_cerr_buf = std::cerr.rdbuf(_cerr->rdbuf());
	}
	else wAssertM(false, "Invalid argument. dst_level cannot be kLevelNum.");
}

void wlib::Logger::setOutputEnabled(bool stdout_enable, bool stderr_enable)
{ _stdout_enable = stdout_enable; _stderr_enable = stderr_enable; }

void wlib::Logger::setOutputEnabled(bool trace_enable, bool performance_enable, bool debug_enable, bool info_enable, bool warning_enable, bool error_enable, bool fatal_enable)
{
	_enable_level.at(kTrace) = trace_enable;
	_enable_level.at(kPerformance) = performance_enable;
	_enable_level.at(kDebug) = debug_enable;
	_enable_level.at(kInfo) = info_enable;
	_enable_level.at(kWarning) = warning_enable;
	_enable_level.at(kError) = error_enable;
	_enable_level.at(kFatal) = fatal_enable;
}

void wlib::Logger::setOutputAllDistinationEnabled(void) { setOutputEnabled(true, true, true, true, true, true, true); }
void wlib::Logger::setOutputAllLevelEnabled(void) { setOutputEnabled(true, true); }

wlib::Logger::Logger(const std::string & header) : header_((header.empty() ? "" : "[" + header + "]")) {}
void wlib::Logger::setDestination(const Destination trace, const Destination performance, const Destination debug, const Destination info, const Destination warning, const Destination error, const Destination fatal){
	_distinations.at(kTrace) = trace;
	_distinations.at(kPerformance) = performance;
	_distinations.at(kDebug) = debug;
	_distinations.at(kInfo) = info;
	_distinations.at(kWarning) = warning;
	_distinations.at(kError) = error;
	_distinations.at(kFatal) = fatal;
}

void wlib::Logger::deleteEmptyLine(const bool is_delete) { _delete_empty_line = is_delete; }
std::string wlib::Logger::source_information(const std::string file, const std::string func, const int line)
{ return std::string(file + "::" + func + "(" + std::to_string(line) + ")"); }

void wlib::Logger::_print(const char buffer[], const Level level) const{
	//[STAT] 2017-10-15 03:47:24 Th:[thread_no] [filename.cpp]::[function]([line])

	if (!_enable_level.at(level)) return;
	if (!(_distinations.at(level) == Destination::kOut && _stdout_enable) && !(_distinations.at(level) == Destination::kErr && _stderr_enable)) return;

	std::stringstream output_text_sstream;
	std::string buffer_str(buffer);
	if (!(_delete_empty_line && buffer_str.find_first_of("\r\n\0") == 0)) {
		std::replace(buffer_str.begin(), buffer_str.end(), '\n', ' ');

		//get the now date and time
		const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		const std::tm * local_now(std::localtime(&now));
		if (!local_now) {
			std::cerr << "failed to malloc in localtime function.";
			return;
		}

		// system count
		auto elapse = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

		//formatting status string
		std::string reset_color_str;

		//status text
		switch (level) {
		case kTrace:		output_text_sstream << "TRACE"; break;
		case kDebug:		output_text_sstream << "DEBUG"; break;
		case kPerformance:	output_text_sstream << "PERF "; break;
		case kInfo:			output_text_sstream << changeColorToGreen() << "INFO "; reset_color_str = resetColorStr(); break;
		case kWarning:		output_text_sstream << changeColorToYellow() << "WARN "; reset_color_str = resetColorStr(); break;
		case kError:		output_text_sstream << changeColorToRed() << "ERROR"; reset_color_str = resetColorStr(); break;
		case kFatal:		output_text_sstream << changeColorToFatal() << "FATAL"; reset_color_str = resetColorStr(); break;
		default: return;
		}

		output_text_sstream << " " << std::put_time(local_now, "%F %T") << "::" << elapse.count() << " Th:" << std::this_thread::get_id();
		output_text_sstream << " | " << this->header_ << buffer_str << reset_color_str;
	}
	output_text_sstream << "\n";

	//for thread safe, lock with mutex
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_distinations.at(level) == Destination::kOut) std::fprintf(stdout, "%s", output_text_sstream.str().c_str());
		else std::fprintf(stderr, "%s", output_text_sstream.str().c_str());
	} 

	resetColorPrc();
}

#if defined(_WIN64) || defined(_WIN32)
namespace {

std::wstring cp_to_wide(const std::string &s, UINT codepage)
{
	int in_length = (int)s.length();
	int out_length = MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
	std::wstring result(out_length, L'\0');
	if (out_length) MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0], out_length);
	return result;
}
std::string wide_to_cp(const std::wstring &s, UINT codepage)
{
	int in_length = (int)s.length();
	int out_length = WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
	std::string result(out_length, '\0');
	if (out_length) WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0], out_length, 0, 0);
	return result;
}
std::string utf8_to_cp(const std::string &s, UINT codepage)
{
	if (codepage == CP_UTF8) return s;
	std::wstring wide = cp_to_wide(s, CP_UTF8);
	return wide_to_cp(wide, codepage);
}
std::string utf8_to_ansi(const std::string &s)
{
	return utf8_to_cp(s, 932);
}

}
#endif
// ======== Stream ========
wlib::LoggerBuffer::LoggerBuffer(const Logger::Level _level, const std::string & header)
	: Logger(header), level(_level) { setp(this->buffer, this->buffer + kBufferSize - 2); setg(this->buffer, this->buffer, this->buffer + kBufferSize - 2); }
wlib::LoggerBuffer::~LoggerBuffer(){}
int wlib::LoggerBuffer::sync(void){ 
	*pptr() = '\0';
#if defined(_WIN64) || defined(_WIN32)
	//std::string sjis = utf8_to_ansi(std::string(this->buffer));
	std::string sjis(this->buffer);
	this->_print(sjis.c_str(), this->level);
#else
	this->_print(this->buffer, this->level);
#endif
	pbump(static_cast<int>(pbase() - pptr()));
	return 0;
}
wlib::LoggerStream::LoggerStream(const Logger::Level _level, const std::string & _header)
	: std::iostream(&logger), logger(_level, _header)
{}

// ======== Assert Utility Functions ==========
void wlib::wAssertFunc(const bool expression, const std::string & srcinfo, const std::string & message)
{ if(!expression) {fatal << srcinfo << "[ASSERTION FAULT]" << message << std::endl; fatal.flush(); ::abort();} }
void wlib::abort(const std::string & srcinfo, const std::string & message)
{ fatal << srcinfo << "[PROGRAM ABORTED]" << message << std::endl; fatal.flush(); ::abort(); }
