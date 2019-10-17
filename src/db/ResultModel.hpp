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

	const int getId();
	const int getSubmissionId();
	const int getAsPhase();
	const std::string getLogText();
	const time_t getCreatedAt();

	void setSubmissionId(int v);
	void setAsPhase(int v);
	void setLogText(std::string v);

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
