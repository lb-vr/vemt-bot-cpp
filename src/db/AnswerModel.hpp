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
	);
	AnswerModel(
		const vemt::db::type::IntParam & entry_id,
		const vemt::db::type::IntParam & question_item_id,
		const vemt::db::type::StringParam & item_value
	);
	~AnswerModel(){}

	const int getId();
	const int getEntryId();
	const int getQuestionItemId();
	const std::string getItemValue();
	std::chrono::system_clock::time_point getCreatedAt();
	std::chrono::system_clock::time_point getUpdatedAt();
	std::string toString();

private:
	const vemt::db::type::IntParam id;
	vemt::db::type::IntParam entry_id;
	vemt::db::type::IntParam question_item_id;
	vemt::db::type::StringParam item_value;
	vemt::db::type::DatetimeParam created_at;
	vemt::db::type::DatetimeParam updated_at;
};
} // namespace db
} // namespace vemt
#endif
