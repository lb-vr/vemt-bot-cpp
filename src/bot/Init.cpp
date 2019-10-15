#include "Init.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
#include "Settings.hpp"

namespace {
const std::string kBotCategoryName = "bot";
const std::string kBotControlChannelName = "bot-control";
const std::string kEntryChannelName = "entry";
const std::string kStatusChannelName = "status";
const std::string kExhibitorCategoryName = "exhibitor";
const std::string kBotAdminRoleName = "BOT-Admin";
const std::string kExhibitorRoleName = "Exhibitor";
}

vemt::bot::InitProcess::InitProcess() noexcept
{}

vemt::bot::InitProcess::~InitProcess() noexcept
{}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::InitProcess::create(void)
{ return std::make_unique<vemt::bot::InitProcess>(); }

std::string vemt::bot::InitProcess::getCommandStr(void) const
{ return "+init"; }

void vemt::bot::InitProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) {
	if (!this->isServer(client, message)) return;

	if(this->isServerOwner(client, message)){
		logging::info << "Start to initialize server. serverID = " << message.serverID.string() << std::endl;
		client.sendMessageW(message.channelID, L"サーバーの初期化を開始します。初期化中は設定を変更しないでください。");

		// TODO: 既に予約されているチャンネル名などがないかチェックを行う
		{
			auto channels = client.getServerChannels(message.serverID).vector();
			for (const auto & ch : channels) {
				if (ch.name == kBotCategoryName ||
					ch.name == kBotControlChannelName ||
					ch.name == kEntryChannelName ||
					ch.name == kStatusChannelName ||
					ch.name == kExhibitorCategoryName) {
					client.sendFailedMessage(message.channelID, L"既に予約されているカテゴリー・チャンネルが存在しています。");
					logging::warn << "Stop initialize server. Already existing channel. Channel = " << ch.name << std::endl;
					return;
				}
			}
			auto roles = client.getRoles(message.serverID).vector();
			for (const auto & role : roles) {
				if (role.name == kExhibitorRoleName ||
					role.name == kBotAdminRoleName) {
					client.sendFailedMessage(message.channelID, L"既に予約されているロールが存在しています。");
					logging::warn << "Stop initialize server. Already existing role. Role = " << role.name << std::endl;
					return;
				}
			}
		}

		// everyone権限を取得
		auto everyone = client.getRoleFromName(message.serverID, "@everyone");

		// 自分の権限を取得
		auto vemt_bot_role = client.getRoleFromName(message.serverID, "vemt-bot");

		// 名前を変更
		client.editNickname(message.serverID, "VEMT");
		logging::debug << " - Changed name to VEMT. serverID = " << message.serverID.string() << std::endl;

		// BOT管理者権限を作成
		auto bot_admin = client.createRole(message.serverID, "BOT-Admin", SleepyDiscord::NONE, 0xff0000u, true, true).cast();
		logging::debug << " - Create Bot Admin role. serverID = " << message.serverID.string() << std::endl;

		// 出展者権限を作成
		auto exhibitor_role = client.createRole(message.serverID, "Exhibitor", SleepyDiscord::NONE, 0x00ffffu, true, true).cast();
		logging::debug << " - Create Exhibitor role. serverID = " << message.serverID.string() << std::endl;

		// カテゴリを作成
		auto category = client.createCategory(message.serverID, "bot").cast();
		logging::debug << " - Create BOT channel. serverID = " << message.serverID.string() << std::endl;

		auto exhibitor_cat = client.createCategory(message.serverID, "exhibitor").cast();
		logging::debug << " - Create EXHIBITOR channel. server ID = " << message.serverID.string() << std::endl;

		// チャンネルを作成
		auto bot_control_ch = client.createTextChannel(message.serverID, "bot-control", category.ID).cast();
		client.editChannelTopic(bot_control_ch, u8"BOT操作専用チャンネルです。BOT-Adminのみが読み書きできます。+helpでヘルプを表示します。");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(bot_admin.ID.number()),
			sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, sd::Permission::MANAGE_ROLES, "role");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()),
			sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()),
			0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");
		logging::debug << " - Create bot-control channel. serverID = " << message.serverID.string() << std::endl;

		auto entry_ch = client.createTextChannel(message.serverID, "entry", category.ID).cast();
		client.editChannelTopic(entry_ch, u8"出展エントリー用のチャンネルです。エントリー期間中のみ書き込みでき、+entryコマンドでエントリーできます。");
		client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
		// TODO
		//client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		logging::debug << " - Create entry channel. serverID = " << message.serverID.string() << std::endl;

		auto status_ch = client.createTextChannel(message.serverID, "status", category.ID).cast();
		client.editChannelTopic(status_ch, u8"BOTステータス確認用のチャンネルです。ミュート推奨です。");
		client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
		client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		logging::debug << " - Create status channel. serverID = " << message.serverID.string() << std::endl;

		// セッティングに格納

		auto _server_id = message.serverID.number();
		auto _bot_cat_id = category.ID.number();
		auto _bot_control_ch_id = bot_control_ch.ID.number();
		auto _entry_ch_id = entry_ch.ID.number();
		auto _status_ch_id = status_ch.ID.number();
		auto _exhibitor_cat_id = exhibitor_cat.ID.number();
		auto _bot_admin_role_id = bot_admin.ID.number();
		auto _exhibitor_role_id = exhibitor_role.ID.number();
		auto _vemt_bot_role_id = vemt_bot_role.ID.number();
		auto _everyone_role_id = everyone.ID.number();

		Settings::createSettings(
			_server_id, _bot_cat_id, _bot_control_ch_id, _entry_ch_id, _status_ch_id, _exhibitor_cat_id,
			_bot_admin_role_id, _exhibitor_role_id, _vemt_bot_role_id, _everyone_role_id);

		// 完了通知
		client.sendSuccessMessage(message.channelID, L"サーバーの初期化が完了しました。");
		logging::info << "Finished initializing the server. serverID = " << message.serverID.string() << std::endl;
	}
	else {
		client.sendFailedMessage(message.channelID, L"このコマンドを実行する権限がありません。");
	}
}
