#ifndef VEMT_BOT_ONEDITMESSAGEPROCESS_HPP
#define VEMT_BOT_ONEDITMESSAGEPROCESS_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "EventProcessBase.hpp"

namespace vemt {
namespace bot {

class OnEditMessageProcess : public EventProcessBase{
public:
	OnEditMessageProcess() noexcept;
	virtual ~OnEditMessageProcess() noexcept;

	static bool addClass(std::unique_ptr<OnEditMessageProcess> && instance);

	///
	/// @brief コマンドから対応するプロセッサクラスのインスタンスを戻す
	/// @param cmd コマンド文字列
	/// @return インスタンスポインタ
	/// @retval nullptr コマンドがコマンドリストに存在しなかった場合
	///
	static std::unique_ptr<EventProcessBase> getClass(const std::string & cmd);

private:
	static std::unordered_map<std::string, std::unique_ptr<OnEditMessageProcess>> class_instances_;
};

}
}

#endif