#ifndef VEMT_DB_RESULTMODEL_INCLUDED
#define VEMT_DB_RESULTMODEL_INCLUDED
#include "type/Param.hpp"
namespace vemt
{
namespace db
{

class ResultModel
{
public:
	ResultModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::IntParam & submission_id,
		const vemt::db::type::IntParam & as_phase,
		const vemt::db::type::BoolParam & is_passed,
		const vemt::db::type::StringParam & log_text,
		const vemt::db::type::DatetimeParam & created_at
	) noexcept;
	ResultModel(
		const vemt::db::type::IntParam & submission_id,
		const vemt::db::type::IntParam & as_phase,
		const vemt::db::type::BoolParam & is_passed,
		const vemt::db::type::StringParam & log_text
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
	const vemt::db::type::IntParam id_;
	vemt::db::type::IntParam submission_id_;
	vemt::db::type::IntParam as_phase_;
	vemt::db::type::BoolParam is_passed_;
	vemt::db::type::StringParam log_text_;
	vemt::db::type::DatetimeParam created_at_;
};
} // namespace db
} // namespace vemt
#endif
