#ifndef VEMT_BOT_PROCESS_HPP
#define VEMT_BOT_PROCESS_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace SleepyDiscord {
class Message;
}

namespace vemt {
namespace bot {

class Client;

class OnMessageProcess {
public:
	OnMessageProcess() noexcept;
	virtual ~OnMessageProcess() noexcept;

	virtual std::unique_ptr<OnMessageProcess> create() = 0;
	virtual std::string getCommandStr(void) const = 0;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) = 0;

	static bool addClass(std::unique_ptr<OnMessageProcess> && instance);

	///
	/// @brief コマンドから対応するプロセッサクラスのインスタンスを戻す
	/// @param cmd コマンド文字列
	/// @return インスタンスポインタ
	/// @retval nullptr コマンドがコマンドリストに存在しなかった場合
	///
	static std::unique_ptr<OnMessageProcess> getClass(const std::string & cmd);
private:
	static std::unordered_map<std::string, std::unique_ptr<OnMessageProcess>> class_instances_;
};

}
}

#endif