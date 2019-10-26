#ifndef VEMT_BOT_ENTRY_HPP
#define VEMT_BOT_ENTRY_HPP

#include "OnMessageProcess.hpp"

namespace vemt {
namespace bot {

class EntryProcess : public OnMessageProcess {
public:
	EntryProcess() noexcept;
	~EntryProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, dscd::Message & message) const override;
	virtual void run(Client & client, dscd::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif