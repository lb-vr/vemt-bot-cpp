#include "Entry.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
#include "Question.hpp"
#include "Settings.hpp"
#include "db/EntriesTable.hpp"
#include "db/DatabaseException.hpp"

vemt::bot::EntryProcess::EntryProcess() noexcept {}

vemt::bot::EntryProcess::~EntryProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::EntryProcess::create(void)
{ return std::make_unique<EntryProcess>(); }

std::string vemt::bot::EntryProcess::getCommandStr(void) const { return "+entry"; }

void vemt::bot::EntryProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	const Settings & settings = Settings::getSettings(message.serverID.number());
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのentryチャンネルでのみ受け付けています。");
	if (settings.getEntryChannel() != message.channelID.number())
		throw AuthenticationFailed(L"エントリーはentryチャンネルでのみ受け付けています。");
	// TODO: 開催期間かどうか？
	
	// データベースにアクセスして、既に仮エントリー済みでないか調べる
	auto _author_id = message.author.ID.number();
	auto entried = db::EntriesTable(message.serverID.string() + ".db").getByDiscordUid(_author_id);
	if (!entried.empty()) throw AuthenticationFailed(L"既にエントリー済みです。");
}

void vemt::bot::EntryProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	const Settings & settings = Settings::getSettings(message.serverID.number());

	auto dm_channel = client.createTextChannel(message.serverID, message.author.username + "_" + message.author.discriminator, sd::Snowflake<sd::Channel>(settings.getExhibitorCategory())).cast();
	logging::debug << "Created text channel for contacting." << std::endl;
	client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getVemtBotRole()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES | sd::Permission::MANAGE_ROLES, 0, "role");
	client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getEveryoneRole()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES | sd::Permission::MANAGE_ROLES, "role");
	client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(message.author.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, sd::Permission::MANAGE_ROLES, "member");
	client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getBotAdminRole()),
		sd::Permission::READ_MESSAGES, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_ROLES, "role");


	// 質問をサーバーから取り寄せる
	// TEMP
	Question question(
		L"[LB]white",
		L"月間新作即売会へのエントリー、ありがとうございます。",
		L"--------------------------------------------------------\\n"
		L"エントリーに際し、いくつかの質問にお答えください。\\n"
		L"質問に答える際は、`+answer <問題番号> <問題の答え>`と書きこみます。\\n"
		L"サーバーから:thumbsup:のリアクションが付き、以下の質問表の答えが更新されていれば、正しく回答できています。\\n"
		L"回答を修正したい場合は、メッセージをそのまま修正するか、新たに回答しなおしてください。\\n"
		L"（エントリーまでに必須）となっている項目は、本エントリーに進むために回答が必要です。\\n"
		L"エントリーまでに必須の項目を回答頂けましたら、再度`+entry`コマンドで本エントリーを行ってください。\\n"
		L"なお、本エントリーを行うと、エントリーまでに必須の項目は再編集ができなくなりますのでご注意ください。\\n"
		L"--------------------------------------------------------\\n",
		{
			QuestionItem(L"サークル名", L"", type::AnswerType::kString, L".+", {}, 32, true, Phase::kEntry, (time_t)MAXINT, false),
			QuestionItem(L"ブースのジャンル", L"", type::AnswerType::kString, L".+", {L"KAKKOI", L"KAWAII", L"アクセサリー", L"ネタ", L"その他"}, 16, true, Phase::kSubmit, (time_t)MAXINT, false),
			QuestionItem(L"サークルカット", L"コマンドと一緒に画像を添付してください。", type::AnswerType::kString, L".+", {}, 32, true, Phase::kPublish, (time_t)MAXINT, false),
			QuestionItem(L"どこに展示したいですか", L"太陽光のある屋外か、ライティングのほとんどない室内かをお選びいただけます。", type::AnswerType::kString, L".+", {}, 32, true, Phase::kSubmit, (time_t)MAXINT, false),
			QuestionItem(L"Twitter ID", L"ウェブサイトに掲載するものです。@は含めないでください。", type::AnswerType::kRegex, L"[a-zA-Z0-9_]+", {}, 32, true, Phase::kPublish , (time_t)MAXINT, false),
			QuestionItem(L"Pixiv Booth URL", L"商品ページではなく、お店のトップページを登録できます。\\nウェブサイトに掲載するものです。", type::AnswerType::kRegex, L"(https://)?[a-zA-Z0-9\-.]/booth.pm/?", {}, 64, true, Phase::kEntry, (time_t)MAXINT, false),
		}
	);


	auto setsumei = client.sendMessageW(dm_channel, question.createAsQuestionMessage()).cast();
	client.pinMessage2(dm_channel, setsumei);

	// TODO: EntryModelを継承して型を作る
	// EntryにsendMessageW(...,wstr)をどうにかして組み込む
	auto _dm_channel_id = dm_channel.ID.number();
	auto _status_message_id = setsumei.ID.number();
	
	//
	//db::EntriesTable(message.serverID.string() + ".db").insert(
	//	db::EntryModel(message.author.ID.number(), Phase::kPreEntry.to_int(), _dm_channel_id, _status_message_id));


	// 仮エントリーを受け付けました、DMを確認してくださいとメッセージ	
	client.sendMentionW(message.channelID, message.author, L"仮エントリーを受け付けました。");
	logging::info << "Accepted pre-entry. User = " << message.author.username << " (" << message.author.ID.string() << ")" << std::endl;

}