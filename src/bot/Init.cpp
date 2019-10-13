#include "Init.hpp"
#include "Client.hpp"
#include "Logger.hpp"

vemt::bot::InitProcess::InitProcess() noexcept
{}

vemt::bot::InitProcess::~InitProcess() noexcept
{}

std::unique_ptr<vemt::bot::OnMessageProcess> vemt::bot::InitProcess::create(void)
{ return std::make_unique<vemt::bot::InitProcess>(); }

std::string vemt::bot::InitProcess::getCommandStr(void) const
{ return "+init"; }

void vemt::bot::InitProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) {
	if (!this->isServer(client, message)) return;

	if(this->isServerOwner(client, message)){
		logging::info << "Start to initialize server. serverID = " << message.serverID.string() << std::endl;
		client.sendMessageW(message.channelID, L"サーバーの初期化を開始します。初期化中は設定を変更しないでください。");

		// TODO: 既に予約されているチャンネル名などがないかチェックを行う

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
		client.createRole(message.serverID, "Exhibitor", SleepyDiscord::NONE, 0x00ffffu, true, true);
		logging::debug << " - Create Exhibitor role. serverID = " << message.serverID.string() << std::endl;

		// カテゴリを作成
		auto category = client.createCategory(message.serverID, "bot").cast();
		logging::debug << " - Create BOT channel. serverID = " << message.serverID.string() << std::endl;

		// チャンネルを作成
		auto bot_control_ch = client.createTextChannel(message.serverID, "bot-control", category.ID).cast();
		client.editChannelTopic(bot_control_ch, u8"BOT操作専用チャンネルです。BOT-Adminのみが書き込みできます。+helpでヘルプを表示します。");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(bot_admin.ID.number()), sd::Permission::SEND_MESSAGES, sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
		client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		logging::debug << " - Create bot-control channel. serverID = " << message.serverID.string() << std::endl;

		auto entry_ch = client.createTextChannel(message.serverID, "entry", category.ID).cast();
		client.editChannelTopic(entry_ch, u8"出展エントリー用のチャンネルです。エントリー期間中のみ書き込みでき、+entryコマンドでエントリーできます。");
		client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
		client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		logging::debug << " - Create entry channel. serverID = " << message.serverID.string() << std::endl;

		auto status_ch = client.createTextChannel(message.serverID, "status", category.ID).cast();
		client.editChannelTopic(status_ch, u8"BOTステータス確認用のチャンネルです。ミュート推奨です。");
		client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
		client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
		logging::debug << " - Create status channel. serverID = " << message.serverID.string() << std::endl;

		// 完了通知
		client.sendSuccessMessage(message.channelID, L"サーバーの初期化が完了しました。");
		logging::info << "Finished initializing the server. serverID = " << message.serverID.string() << std::endl;
	}
	else {
		client.sendFailedMessage(message.channelID, L"このコマンドを実行する権限がありません。");
	}
}
