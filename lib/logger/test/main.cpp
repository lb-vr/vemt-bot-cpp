﻿#include "Logger.hpp"

using namespace wlib;
int main(void) {

	Logger::setOutputAllDistinationEnabled();
	Logger::setOutputAllLevelEnabled();

	std::cout << "Before Redirect of cout" << std::endl;

	std::cerr << "Before Redirect of cerr" << std::endl;

	Logger::setRedirectionCout(Logger::kDebug);
	Logger::setRedirectionCerr(Logger::kError);

	LoggerStream original_output(Logger::Level::kDebug, "main");
	original_output << "This is original logger." << std::endl;

	std::cout << "This is cout output" << std::endl;

	std::cerr << "This is cerr output" << std::endl;

	trace << "This is trace output" << std::endl;

	perf << "This is Perform output" << std::endl;

	debug << "This is debug output" << std::endl;

	info << "This is info output" << std::endl;

	warn << "This is warn output" << std::endl;

	error << "This is error output" << std::endl;

	std::cerr << "HOGEHOGE" << std::endl;

	fatal << "This is fatal output" << std::endl;

	trace << SRCINFO << "This is trace output" << std::endl;

	debug << SRCINFO << "This is debug output" << std::endl;

	info << SRCINFO << "This is info output" << std::endl;

	warn << SRCINFO << "This is warn output" << std::endl;

	error << SRCINFO << "This is error output" << std::endl;

	fatal << SRCINFO << "This is fatal output" << std::endl;

	wAssertM(false, "test assertion");

	return 0;
}