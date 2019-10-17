﻿#include "Reset.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"

vemt::bot::ResetProcess::ResetProcess() noexcept {}
vemt::bot::ResetProcess::~ResetProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::ResetProcess::create(void)
{ return std::make_unique<vemt::bot::ResetProcess>();}

std::string vemt::bot::ResetProcess::getCommandStr(void) const
{ return "+reset"; }

void vemt::bot::ResetProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのチャンネルでのみ受け付けています。");
	if (!this->isServerOwner(client, message))
		throw AuthenticationFailed(L"このコマンドを実行する権限がありません。");
}

void vemt::bot::ResetProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	if (!this->isServer(client, message)) return;

	if (this->isServerOwner(client, message)) {

		// リセットする
		logging::info << "Start to reset server. ServerID = " << message.serverID.string() << std::endl;
		client.sendMessageW(message.channelID, L"サーバーをもとに戻しています。");

		// BOTの名前を戻す
		client.editNickname(message.serverID, "");
		logging::debug << " - Reset nickname. ServerID = " << message.serverID.string() << std::endl;

		// チャンネルを消す
		auto channels = client.getServerChannels(message.serverID).vector();
		for (auto & p : channels) {
			if (((p.name == "status" || p.name == "bot-control" || p.name == "entry") && p.type == p.SERVER_TEXT)
				|| (p.name == "bot" && p.type == p.SERVER_CATEGORY)) {
				client.deleteChannel(p.ID);
				logging::debug << " - Deleted " << p.name << " channel. ServerID = " << message.serverID.string() << std::endl;
			}
		}

		// 権限を消す
		auto roles = client.getRoles(message.serverID).vector();
		for (auto & p : roles) {
			if (p.name == "BOT-Admin" || p.name == "Exhibitor") {
				client.deleteRole(message.serverID, p.ID);
				logging::debug << " - Deleted " << p.name << " role. ServerID = " << message.serverID.string() << std::endl;
			}
		}

		logging::info << "Finished resetting server. ServerID = " << message.serverID.string() << std::endl;
		client.sendSuccessMessage(message.channelID,L"サーバーをもとの状態へリセットしました。");
	}
}