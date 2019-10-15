#include "OnMessageProcess.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"

std::unordered_map<std::string, std::unique_ptr<vemt::bot::OnMessageProcess>> vemt::bot::OnMessageProcess::class_instances_;

vemt::bot::OnMessageProcess::OnMessageProcess() noexcept {}
vemt::bot::OnMessageProcess::~OnMessageProcess() noexcept {}

bool vemt::bot::OnMessageProcess::addClass(std::unique_ptr<OnMessageProcess>&& instance) {
	auto cmd = instance->getCommandStr();
	if (OnMessageProcess::class_instances_.count(cmd)) {
		logging::error << "Failed to addClass, already added. Command = " << cmd << std::endl;
		return false;
	}
	OnMessageProcess::class_instances_[cmd] = std::move(instance);
	return true;
}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::OnMessageProcess::getClass(const std::string & cmd) {
	if (OnMessageProcess::class_instances_.count(cmd)) {
		return OnMessageProcess::class_instances_.at(cmd)->create();
	}
	return std::unique_ptr<OnMessageProcess>();	// nullptr
}

