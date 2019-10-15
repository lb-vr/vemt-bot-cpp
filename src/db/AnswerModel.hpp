#ifndef VEMT_DB_ANSWERMODEL_INCLUDED
#define VEMT_DB_ANSWERMODEL_INCLUDED
#include "type/Param.hpp"
namespace vemt
{
namespace db
{

class AnswerModel
{
public:
	AnswerModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::IntParam & entry_id,
		const vemt::db::type::IntParam & question_item_id,
		const vemt::db::type::StringParam & item_value,
		const vemt::db::type::DatetimeParam & created_at,
		const vemt::db::type::DatetimeParam & updated_at
	) noexcept;
	AnswerModel(
		const vemt::db::type::IntParam & entry_id,
		const vemt::db::type::IntParam & question_item_id,
		const vemt::db::type::StringParam & item_value
	) noexcept;
	AnswerModel(const AnswerModel & copy) noexcept;
	~AnswerModel(){}

	const int getId();
	const int getEntryId();
	const int getQuestionItemId();
	const std::string getItemValue();
	const time_t getCreatedAt();
	const time_t getUpdatedAt();
	const std::string toString() const;

private:
	const vemt::db::type::IntParam id_;
	vemt::db::type::IntParam entry_id_;
	vemt::db::type::IntParam question_item_id_;
	vemt::db::type::StringParam item_value_;
	vemt::db::type::DatetimeParam created_at_;
	vemt::db::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
