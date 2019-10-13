#include "AnswerModel.hpp"

vemt::db::AnswerModel::AnswerModel(
	const vemt::db::type::IntParam & id,
	const vemt::db::type::IntParam & entry_id,
	const vemt::db::type::IntParam & question_item_id,
	const vemt::db::type::StringParam & item_value,
	const vemt::db::type::DatetimeParam & created_at,
	const vemt::db::type::DatetimeParam & updated_at
) noexcept :
	id_(id),
	entry_id_(entry_id),
	question_item_id_(question_item_id),
	item_value_(item_value),
	created_at_(created_at),
	updated_at_(updated_at){
}
vemt::db::AnswerModel::AnswerModel(
	const vemt::db::type::IntParam & entry_id,
	const vemt::db::type::IntParam & question_item_id,
	const vemt::db::type::StringParam & item_value
) noexcept :
	entry_id_(entry_id),
	question_item_id_(question_item_id),
	item_value_(item_value){
}
vemt::db::AnswerModel::AnswerModel(const AnswerModel & c) noexcept :
	id_(c.id_),
	entry_id_(c.entry_id_),
	question_item_id_(c.question_item_id_),
	item_value_(c.item_value_),
	created_at_(c.created_at_),
	updated_at_(c.updated_at_){
}

const int vemt::db::AnswerModel::getId(){
	return this->id_.get();
}
const int vemt::db::AnswerModel::getEntryId(){
	return this->entry_id_.get();
}
const int vemt::db::AnswerModel::getQuestionItemId(){
	return this->question_item_id_.get();
}
const std::string vemt::db::AnswerModel::getItemValue(){
	return this->item_value_.get();
}
const std::chrono::system_clock::time_point vemt::db::AnswerModel::getCreatedAt(){
	return this->created_at_.get();
}
const std::chrono::system_clock::time_point vemt::db::AnswerModel::getUpdatedAt(){
	return this->updated_at_.get();
}

const std::string vemt::db::AnswerModel::toString() const {
	std::stringstream ret;
    ret << "{model: 'AnswerModel', "
		<< "id: '" << id_.get() << "', "
		<< "entry_id: '" << entry_id_.get() << "', "
		<< "question_item_id: '" << question_item_id_.get() << "', "
		<< "item_value: '" << item_value_.get() << "', "
		<< "created_at: '" << created_at_.getAsString() << "', "
		<< "updated_at: '" << updated_at_.getAsString() << "'"
		<< "}";
    return ret.str();
}
