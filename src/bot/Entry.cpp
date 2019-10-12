#include "Entry.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Question.hpp"

vemt::bot::EntryProcess::EntryProcess() noexcept {}

vemt::bot::EntryProcess::~EntryProcess() noexcept {}

std::unique_ptr<vemt::bot::OnMessageProcess> vemt::bot::EntryProcess::create(void)
{ return std::make_unique<EntryProcess>(); }

std::string vemt::bot::EntryProcess::getCommandStr(void) const { return "+entry"; }

void vemt::bot::EntryProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	if (!this->isServer(client, message)) return;

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
		client.sendFailedMessage(message.channelID, u8"エントリーはentryチャンネルでのみ受け付けています。");
		return;
	}

	// エントリー期間か調べる

	// データベースにアクセスして、既に仮エントリー済みでないか調べる

	// 大丈夫ならDMチャンネルを作成
	sd::Snowflake<sd::Channel> dm_channel_sf;
	try {
		auto dm_channel_tmp = client.createDirectMessageChannel(message.author.ID.string()).cast();
		dm_channel_sf = sd::Snowflake<sd::Channel>(dm_channel_tmp.ID.number());
	}
	catch (sd::ErrorCode e) {
		if (e == sd::ErrorCode::AUTHENTICATION_FAILED) {
			client.sendFailedMessage(message.channelID, u8"設定から「サーバーにいるメンバーからのDMを許可する。」を有効にしてください。");
			return;
		}
		else throw e;
	}

	client.sendMessage(dm_channel_sf, u8"現在仮エントリー中です。");

	// 質問をサーバーから取り寄せる
	// TEMP
	Question question(
		"[LB]white",
		"\\U00020BB7",
		//u8"月間新作即売会へのエントリー、ありがとうございます。",
		u8"--------------------------------------------------------\n"
		u8"エントリーに際し、いくつかの質問にお答えください。\n"
		u8"質問に答える際は、`+answer <問題番号> <問題の答え>`と書きこみます。\n"
		u8"サーバーから:thumbsup:のリアクションが付き、以下の質問表の答えが更新されていれば、正しく回答できています。\n"
		u8"回答を修正したい場合は、メッセージをそのまま修正するか、新たに回答しなおしてください。\n"
		u8"（エントリーまでに必須）となっている項目は、本エントリーに進むために回答が必要です。\n"
		u8"エントリーまでに必須の項目を回答頂けましたら、再度`+entry`コマンドで本エントリーを行ってください。\n"
		u8"なお、本エントリーを行うと、エントリーまでに必須の項目は再編集ができなくなりますのでご注意ください。",
		{
			QuestionItemModel(1 ,u8"サークル名", "", QuestionItemModel::Type::kString, ".+", {}, 32, true, Phase::kEntry, std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
			QuestionItemModel(2, u8"ブースのジャンル", "", QuestionItemModel::Type::kString, ".+", {"KAKKOI", "KAWAII", u8"アクセサリー", u8"ネタ", u8"その他"}, 16, true, Phase::kSubmit, std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
			QuestionItemModel(3, u8"サークルカット", "コマンドと一緒に画像を添付してください。", QuestionItemModel::Type::kString, ".+", {}, 32, true, Phase::kPublish, std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
			QuestionItemModel(4, u8"どこに展示したいですか", u8"太陽光のある屋外か、ライティングのほとんどない室内かをお選びいただけます。", QuestionItemModel::Type::kString, ".+", {}, 32, true, Phase::kSubmit, std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
			QuestionItemModel(5, u8"Twitter ID", "ウェブサイトに掲載するものです。@は含めないでください。", QuestionItemModel::Type::kRegex, "[a-zA-Z0-9_]+", {}, 32, true, Phase::kPublish , std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
			QuestionItemModel(6, u8"Pixiv Booth URL", "商品ページではなく、お店のトップページを登録できます。\nウェブサイトに掲載するものです。", QuestionItemModel::Type::kRegex, "(https://)?[a-zA-Z0-9\-.]/booth.pm/?", {}, 64, true, Phase::kEntry, std::chrono::system_clock::from_time_t(MAXLONGLONG), false),
		}
	);

	client.sendMessage(dm_channel_sf, u8"**" + question.getTitle() + u8"**");// +question.getHeader());

	// 仮エントリーを受け付けました、DMを確認してくださいとメッセージ	
	client.sendMention(message.channelID, message.author, u8"仮エントリーを受け付けました。DMを確認してください。");
	

}