#ifndef VEMT_DB_RESULTMODEL_INCLUDED
#define VEMT_DB_RESULTMODEL_INCLUDED
#include "type/Type.hpp"
namespace vemt
{
namespace db
{

class ResultModel
{
public:
	ResultModel(
		const vemt::type::IntParam & id,
		const vemt::type::IntParam & submission_id,
		const vemt::type::IntParam & as_phase,
		const vemt::type::BoolParam & is_passed,
		const vemt::type::StringParam & log_text,
		const vemt::type::DatetimeParam & created_at
	) noexcept;
	ResultModel(
		const vemt::type::IntParam & submission_id,
		const vemt::type::IntParam & as_phase,
		const vemt::type::BoolParam & is_passed,
		const vemt::type::StringParam & log_text
	) noexcept;
	ResultModel(const ResultModel & copy) noexcept;
	~ResultModel(){}

	const vemt::type::IntParam & getId() const;
	const vemt::type::IntParam & getSubmissionId() const;
	const vemt::type::IntParam & getAsPhase() const;
	const vemt::type::BoolParam & getIsPassed() const;
	const vemt::type::StringParam & getLogText() const;
	const vemt::type::DatetimeParam & getCreatedAt() const;

	void getSubmissionId(const vemt::type::IntParam & value);
	void getAsPhase(const vemt::type::IntParam & value);
	void getIsPassed(const vemt::type::BoolParam & value);
	void getLogText(const vemt::type::StringParam & value);

	std::string toString() const;

private:
	const vemt::type::IntParam id_;
	vemt::type::IntParam submission_id_;
	vemt::type::IntParam as_phase_;
	vemt::type::BoolParam is_passed_;
	vemt::type::StringParam log_text_;
	vemt::type::DatetimeParam created_at_;
};
} // namespace db
} // namespace vemt
#endif
