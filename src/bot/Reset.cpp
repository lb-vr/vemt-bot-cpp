#include "Reset.hpp"
#include "Client.hpp"
#include "Settings.hpp"
#include "util/Logger.hpp"

vemt::bot::ResetProcess::ResetProcess() noexcept {}
vemt::bot::ResetProcess::~ResetProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::ResetProcess::create(void)
{ return std::make_unique<vemt::bot::ResetProcess>();}

std::string vemt::bot::ResetProcess::getCommandStr(void) const
{ return "+reset"; }

void vemt::bot::ResetProcess::authenticate(Client & client, dscd::Message & message) const {
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのチャンネルでのみ受け付けています。");
	if (!this->isServerOwner(client, message))
		throw AuthenticationFailed(L"このコマンドを実行する権限がありません。");
}

void vemt::bot::ResetProcess::run(Client & client, dscd::Message & message, const std::vector<std::string>& args) {
	Settings settings = Settings::getSettings(message.serverID.number());

	// リセットする
	logging::info << "Start to reset server. ServerID = " << message.serverID.string() << std::endl;
	client.sendMessageW(message.channelID, L"サーバーをもとに戻しています。");

	// BOTの名前を戻す
	logging::debug << " - Resetting nickname." << std::endl;
	client.editNickname(message.serverID, "");
	logging::info << " - Reset nickname." << std::endl;

	// チャンネルを消す
	logging::debug << " - Deleting channels." << std::endl;
	auto channels = client.getServerChannels(message.serverID).vector();
	for (const auto ch : settings.getAllCreatedChannels()) {
		for (const auto & sv : channels) {
			if (ch == sv.ID.number()) {
				logging::debug << " -- Deleting " << sv.name << " (" << sv.ID.number() << ") channel." << std::endl;
				client.deleteChannel(sv.ID.number());
				logging::info << " -- Deleted " << sv.name << " (" << sv.ID.number() << ") channel." << std::endl;
				break;
			}
		}
	}
	logging::info << " - Deleted channels." << std::endl;

	// カテゴリを消す
	logging::debug << " - Deleting categories." << std::endl;
	for (const auto cat : settings.getAllCreatedCategories()) {
		for (const auto & sv : channels) {
			if (cat == sv.ID.number()) {
				logging::debug << " -- Deleting " << sv.name << " (" << sv.ID.number() << ") channel." << std::endl;
				client.deleteChannel(sv.ID.number());
				logging::info << " -- Deleted " << sv.name << " (" << sv.ID.number() << ") channel." << std::endl;
				break;
			}
		}
	}
	logging::info << " - Deleted categories." << std::endl;

	// ロールを消す
	logging::debug << " - Deleting Roles." << std::endl;
	auto roles = client.getRoles(message.serverID).vector();
	for (const auto r : settings.getAllCreatedRoles()) {
		for (auto & p : roles) {
			if (r == p.ID.number()) {
				logging::debug << " -- Deleting " << p.name << " (" << p.ID.number() << ") role." << std::endl;
				client.deleteRole(message.serverID, p.ID);
				logging::info << " -- Deleting " << p.name << " (" << p.ID.number() << ") role." << std::endl;
			}
		}
	}

	Settings::clearCache(message.serverID.number());
	logging::info << " - Cleared cache." << std::endl;

	logging::info << "Finished resetting server." << std::endl;

	client.sendSuccessMessage(message.channelID, L"サーバーをもとの状態へリセットしました。");
}
