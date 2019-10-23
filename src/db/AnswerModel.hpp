#ifndef VEMT_DB_ANSWERMODEL_INCLUDED
#define VEMT_DB_ANSWERMODEL_INCLUDED
#include "type/Type.hpp"
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

	const vemt::type::IntParam & getId() const;
	const vemt::type::IntParam & getEntryId() const;
	const vemt::type::IntParam & getQuestionItemId() const;
	const vemt::type::StringParam & getItemValue() const;
	const vemt::type::DatetimeParam & getCreatedAt() const;
	const vemt::type::DatetimeParam & getUpdatedAt() const;
	const std::string toString() const;

	void setEntryId(const vemt::type::IntParam & value);
	void setQuestionItemId(const vemt::type::IntParam & value);
	void setItemValue(const vemt::type::StringParam & value);

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
