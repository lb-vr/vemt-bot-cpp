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
	);
	ResultModel(
		const vemt::db::type::IntParam & discord_uid,
		const vemt::db::type::StringParam & package_url,
		const vemt::db::type::IntParam & current_phase
	);
	~ResultModel(){}

	int getId();
	int getSubmissionId();
	int getAsPhase();
	int getCurrentPhase();
	std::chrono::system_clock::time_point getCreatedAt();
	std::chrono::system_clock::time_point getUpdatedAt();
	std::string toString();

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
