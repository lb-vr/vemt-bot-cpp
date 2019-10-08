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
	/// @brief �R�}���h����Ή�����v���Z�b�T�N���X�̃C���X�^���X��߂�
	/// @param cmd �R�}���h������
	/// @return �C���X�^���X�|�C���^
	/// @retval nullptr �R�}���h���R�}���h���X�g�ɑ��݂��Ȃ������ꍇ
	///
	static std::unique_ptr<OnMessageProcess> getClass(const std::string & cmd);
private:
	static std::unordered_map<std::string, std::unique_ptr<OnMessageProcess>> class_instances_;
};

}
}

#endif