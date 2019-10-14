#ifndef VEMT_DB_ENTRYMODEL_INCLUDED
#define VEMT_DB_ENTRYMODEL_INCLUDED
#include "type/Param.hpp"
namespace vemt
{
namespace db
{

class EntryModel
{
public:
	EntryModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::IntParam & discord_uid,
		const vemt::db::type::StringParam & package_url,
		const vemt::db::type::IntParam & current_phase,
		const vemt::db::type::IntParam & query_status_message_id_,
		const vemt::db::type::IntParam & working_status_message_id_,
		const vemt::db::type::DatetimeParam & created_at,
		const vemt::db::type::DatetimeParam & updated_at
	) noexcept;
	EntryModel(
		const vemt::db::type::IntParam & discord_uid,
		const vemt::db::type::StringParam & package_url,
		const vemt::db::type::IntParam & current_phase,
		const vemt::db::type::IntParam & query_status_message_id_,
		const vemt::db::type::IntParam & working_status_message_id_
	) noexcept;
	EntryModel(const EntryModel & copy) noexcept;
	~EntryModel(){}

	int getId();
	int getDiscordUid();
	std::string getPackageUrl();
	int getCurrentPhase();
	int getQueryStatusMessageId();
	int getWorkingStatusMessageId();
	std::chrono::system_clock::time_point getCreatedAt();
	std::chrono::system_clock::time_point getUpdatedAt();
	std::string toString() const;

private:
	const vemt::db::type::IntParam id_;
	vemt::db::type::IntParam discord_user_id_;
	vemt::db::type::StringParam package_url_;
	vemt::db::type::IntParam current_phase_;
	vemt::db::type::IntParam query_status_message_id_;
	vemt::db::type::IntParam working_status_message_id_;
	vemt::db::type::DatetimeParam created_at_;
	vemt::db::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
