#ifndef VEMT_BOT_CONFIG_HPP
#define VEMT_BOT_CONFIG_HPP

#include "OnMessageProcess.hpp"

namespace dscd {
class Attachment;
}

namespace vemt {
namespace bot {

class ConfigProcess : public OnMessageProcess {
public:
	ConfigProcess() noexcept;
	~ConfigProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, dscd::Message & message) const override;
	virtual void run(Client & client, dscd::Message & message, const std::vector<std::string> & args) override;

private:
	void question_upload(Client & client, dscd::Message & message, const dscd::Attachment & fpath);
	void question_add(Client & client, dscd::Message & message, const std::string & param);
};

}
}

#endif