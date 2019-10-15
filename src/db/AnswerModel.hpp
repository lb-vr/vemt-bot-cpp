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
		const vemt::type::IntParam & id,
		const vemt::type::IntParam & entry_id,
		const vemt::type::IntParam & question_item_id,
		const vemt::type::StringParam & item_value,
		const vemt::type::DatetimeParam & created_at,
		const vemt::type::DatetimeParam & updated_at
	) noexcept;
	AnswerModel(
		const vemt::type::IntParam & entry_id,
		const vemt::type::IntParam & question_item_id,
		const vemt::type::StringParam & item_value
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
	const vemt::type::IntParam id_;
	vemt::type::IntParam entry_id_;
	vemt::type::IntParam question_item_id_;
	vemt::type::StringParam item_value_;
	vemt::type::DatetimeParam created_at_;
	vemt::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
