#include "OnEditMessageProcess.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"

std::unordered_map<std::string, std::unique_ptr<vemt::bot::OnEditMessageProcess>> vemt::bot::OnEditMessageProcess::class_instances_;

vemt::bot::OnEditMessageProcess::OnEditMessageProcess() noexcept {}
vemt::bot::OnEditMessageProcess::~OnEditMessageProcess() noexcept {}

bool vemt::bot::OnEditMessageProcess::addClass(std::unique_ptr<OnEditMessageProcess>&& instance) {
	auto cmd = instance->getCommandStr();
	if (OnEditMessageProcess::class_instances_.count(cmd)) {
		logging::error << "Failed to addClass, already added. Command = " << cmd << std::endl;
		return false;
	}
	OnEditMessageProcess::class_instances_[cmd] = std::move(instance);
	return true;
}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::OnEditMessageProcess::getClass(const std::string & cmd) {
	if (OnEditMessageProcess::class_instances_.count(cmd)) {
		return OnEditMessageProcess::class_instances_.at(cmd)->create();
	}
	return std::unique_ptr<OnEditMessageProcess>();	// nullptr
}
