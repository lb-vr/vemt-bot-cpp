#ifndef VEMT_BOT_RESET_HPP
#define VEMT_BOT_RESET_HPP

#include "OnMessageProcess.hpp"

namespace vemt {
namespace bot {

class ResetProcess : public OnMessageProcess {
public:
	ResetProcess() noexcept;
	~ResetProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, dscd::Message & message) const override;
	virtual void run(Client & client, dscd::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif