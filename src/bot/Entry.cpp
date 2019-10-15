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

void vemt::bot::EntryProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	if (!this->isServer(client, message)) return;

	const Settings & settings = Settings::getSettings(message.serverID.number());

	// entryチャンネルか調べる
	bool is_entry_channel = [&]() {
		auto channels = client.getServerChannels(message.serverID).vector();
		for (const auto c : channels) {
			if (c.ID == message.channelID && c.name == "entry") {
				return true;
			}
		}
		return false;
	}();
	if (!is_entry_channel) {
		client.sendFailedMessage(message.channelID, L"エントリーはentryチャンネルでのみ受け付けています。");
		logging::warn << "User " << message.author.username << "#" << message.author.discriminator << " Entry rejected. Entry is only on entry channel." << std::endl;
		return;
	}


	// エントリー期間か調べる


	// データベースにアクセスして、既に仮エントリー済みでないか調べる
	try {
		auto _author_id = message.author.ID.number();
		auto entried = db::EntriesTable(message.serverID.string() + ".db").getByDiscordUid(_author_id);
		
		client.sendFailedMessage(message.channelID, L"既にエントリー済みです。");
		logging::warn << "User " << message.author.username << "#" << message.author.discriminator << " Entry rejected. Already entried. Entry ID = " << entried.getId() << std::endl;
		return;
	}
	catch (db::DatabaseException) {

	}

	// 大丈夫ならDMチャンネルを作成
	auto dm_channel = client.createTextChannel(message.serverID, message.author.ID.string(), sd::Snowflake<sd::Channel>(settings.getExhibitorCategory())).cast();
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

	std::wstring wstr = L"**" + question.getTitle() + L"**\\n";
	wstr += question.getHeader() + L"\\n";
	for (const auto & p : question.getQuestionItem()) {
		wstr += L"**Q" + std::to_wstring(p.getId()) + L". " + p.getText() + L"**\\n";
		if (!p.getDetailText().empty()) wstr += p.getDetailText() + L"\\n";
		wstr += L"    <未回答>\\n";
		wstr += L"\\n";
	}

	client.sendMessageW(dm_channel, wstr);


	// 仮エントリーを受け付けました、DMを確認してくださいとメッセージ	
	client.sendMentionW(message.channelID, message.author, L"仮エントリーを受け付けました。");
	logging::info << "Accepted pre-entry. User = " << message.author.username << " (" << message.author.ID.string() << ")" << std::endl;

}