#include "Answer.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"
#include "Settings.hpp"
#include "bot/QuestionItem.hpp"
#include "db/QuestionItemsTable.hpp"
#include "db/AnswersTable.hpp"
#include "db/EntriesTable.hpp"
#include "util/string_util.hpp"

vemt::bot::AnswerProcess::AnswerProcess() noexcept : EventProcessBase(){}

vemt::bot::AnswerProcess::~AnswerProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::AnswerProcess::create(void) {
	return std::unique_ptr<AnswerProcess>();
}

std::string vemt::bot::AnswerProcess::getCommandStr(void) const {
	return "+answer";
}

void vemt::bot::AnswerProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	auto serverID = message.serverID.number();
	auto exhibitor_role = Settings::getSettings(serverID).getExhibitorRole();
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"+answerコマンドはサーバーでのみ受け付けています。");
	if (!this->isRole(client, message, exhibitor_role))
		throw AuthenticationFailed(L"+configコマンドは出展者用のコマンドです。");

	auto authorId = message.author.ID.number();
	if (db::EntriesTable(this->getDatabaseFilepath(message)).getByDiscordUid(authorId).empty())
		throw AuthenticationFailed(L"仮エントリーが未完了です。");
}

void vemt::bot::AnswerProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	// This command is only for direct message.
	// もし公開サーバーで回答された場合は、回答として採用するが、その後すぐメッセージを削除する

	// argument parser and validating
	if (args.size() < 3) throw ProcessException(L"`+answer <問題番号> <問題の答え/画像>`という形で、スペースで区切って記入してください。");
	int question_id = std::stoi(args.at(1));

	// Question IDが問題としてあるか？
	auto question_items = db::QuestionItemsTable(this->getDatabaseFilepath(message)).getById(question_id);
	if (question_items.size() != 1) throw ProcessException(L"指定された番号に該当する問題がありません。");
	QuestionItem qitem(question_items.at(0));

	// Userを取得
	auto authorId = message.author.ID.number();
	auto author_entry_model = db::EntriesTable(this->getDatabaseFilepath(message)).getByDiscordUid(authorId).at(0);
	
	// TODO
	type::PhaseParam current_phase;
	current_phase.setAsInt(author_entry_model.getCurrentPhase());

	type::PhaseParam required_when;
	required_when.setAsInt(qitem.getRequiredWhenPhase());

	std::wstring answer_wstr;
	switch (qitem.getType()) {
	case type::AnswerType::kPicture:
		if (message.attachments.size() != 1)
			throw ProcessException(L"この質問には、画像を一つ添付する必要があります。");
		answer_wstr = util::widen(message.attachments.at(0).url);
		// TODO 画像をダウンロードしてバリデーティング
		break;
	case type::AnswerType::kJsonFile:
		if (message.attachments.size() != 1)
			throw ProcessException(L"この質問には、JSONファイルを一つ添付する必要があります。");
		answer_wstr = util::widen(message.attachments.at(0).url);
		// TODO JSONをダウンロードしてバリデーティング
		break;
	}

	// TODO 中でチェックして、ダメだったらProcessExceptionを呼ぶ
	qitem.validate(answer_wstr);

	// 良ければDBに登録
	
	// Questionを再構築

	// QuestionMessageを更新
}
