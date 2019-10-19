﻿#include "AnswerModel.hpp"
#include <sstream>

vemt::db::AnswerModel::AnswerModel(
	const vemt::type::IntParam & id,
	const vemt::type::IntParam & entry_id,
	const vemt::type::IntParam & question_item_id,
	const vemt::type::StringParam & item_value,
	const vemt::type::DatetimeParam & created_at,
	const vemt::type::DatetimeParam & updated_at
) noexcept :
	id_(id),
	entry_id_(entry_id),
	question_item_id_(question_item_id),
	item_value_(item_value),
	created_at_(created_at),
	updated_at_(updated_at){
}
vemt::db::AnswerModel::AnswerModel(
	const vemt::type::IntParam & entry_id,
	const vemt::type::IntParam & question_item_id,
	const vemt::type::StringParam & item_value
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

const vemt::type::IntParam & vemt::db::AnswerModel::getId() const{
	return this->id_;
}
const vemt::type::IntParam & vemt::db::AnswerModel::getEntryId() const{
	return this->entry_id_;
}
const vemt::type::IntParam & vemt::db::AnswerModel::getQuestionItemId() const{
	return this->question_item_id_;
}
const vemt::type::StringParam & vemt::db::AnswerModel::getItemValue() const{
	return this->item_value_;
}
const vemt::type::DatetimeParam & vemt::db::AnswerModel::getCreatedAt() const{
	return this->created_at_;
}
const vemt::type::DatetimeParam & vemt::db::AnswerModel::getUpdatedAt() const{
	return this->updated_at_;
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

void vemt::db::AnswerModel::setEntryId(const vemt::type::IntParam & value){
	this->entry_id_ = value;
}
