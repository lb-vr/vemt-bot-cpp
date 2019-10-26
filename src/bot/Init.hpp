#ifndef VEMT_BOT_INIT_HPP
#define VEMT_BOT_INIT_HPP

#include "OnMessageProcess.hpp"


namespace vemt {
namespace bot {

class InitProcess : public OnMessageProcess {
public:
	InitProcess() noexcept;
	~InitProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, dscd::Message & message) const override;
	virtual void run(Client & client, dscd::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif