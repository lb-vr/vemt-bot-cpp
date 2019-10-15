#include "Entry.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
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
		client.sendFailedMessage(message.channelID, L"エントリーはentryチャンネルでのみ受け付けています。");
		return;
	}

	// エントリー期間か調べる

	// データベースにアクセスして、既に仮エントリー済みでないか調べる

	// 大丈夫ならDMチャンネルを作成
	sd::Snowflake<sd::Channel> dm_channel_sf;
	try {
		auto dm_channel_tmp = client.createDirectMessageChannel(message.author.ID.string()).cast();
		dm_channel_sf = sd::Snowflake<sd::Channel>(dm_channel_tmp.ID.number());


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

		client.sendMessageW(dm_channel_sf, wstr);

		// 仮エントリーを受け付けました、DMを確認してくださいとメッセージ	
		client.sendMentionW(message.channelID, message.author, L"仮エントリーを受け付けました。DMを確認してください。");
		logging::info << "Accepted pre-entry. User = " << message.author.username << " (" << message.author.ID.string() << ")" << std::endl;

	}
	catch (sd::ErrorCode e) {
		if (e == sd::ErrorCode::FORBIDDEN) {
			client.sendFailedMessage(message.channelID, L"設定から「サーバーにいるメンバーからのDMを許可する。」を有効にしてください。");
			logging::warn << "DM Forbidden. User = " << message.author.username << " (" << message.author.ID.string() << ")" << std::endl;
			return;
		}
		else throw e;
	}
}