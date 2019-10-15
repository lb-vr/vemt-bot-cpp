#ifndef VEMT_BOT_ONMESSAGEPROCESS_HPP
#define VEMT_BOT_ONMESSAGEPROCESS_HPP

#include "EventProcessBase.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace vemt {
namespace bot {

class OnMessageProcess : public EventProcessBase {

public:
	OnMessageProcess() noexcept;
	virtual ~OnMessageProcess() noexcept;

	static bool addClass(std::unique_ptr<OnMessageProcess> && instance);

	///
	/// @brief コマンドから対応するプロセッサクラスのインスタンスを戻す
	/// @param cmd コマンド文字列
	/// @return インスタンスポインタ
	/// @retval nullptr コマンドがコマンドリストに存在しなかった場合
	///
	static std::unique_ptr<EventProcessBase> getClass(const std::string & cmd);

private:
	static std::unordered_map<std::string, std::unique_ptr<OnMessageProcess>> class_instances_;
};

}
}

#endif