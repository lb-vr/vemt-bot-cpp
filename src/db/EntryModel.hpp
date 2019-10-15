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
		const vemt::type::IntParam & id,
		const vemt::type::IntParam & discord_uid,
		const vemt::type::StringParam & package_url,
		const vemt::type::IntParam & current_phase,
		const vemt::type::IntParam & query_status_message_id_,
		const vemt::type::IntParam & working_status_message_id_,
		const vemt::type::DatetimeParam & created_at,
		const vemt::type::DatetimeParam & updated_at
	) noexcept;
	EntryModel(
		const vemt::type::IntParam & discord_uid,
		const vemt::type::StringParam & package_url,
		const vemt::type::IntParam & current_phase,
		const vemt::type::IntParam & query_status_message_id_,
		const vemt::type::IntParam & working_status_message_id_
	) noexcept;
	EntryModel(const EntryModel & copy) noexcept;
	~EntryModel(){}

	int getId();
	int getDiscordUid();
	std::string getPackageUrl();
	int getCurrentPhase();
	int getQueryStatusMessageId();
	int getWorkingStatusMessageId();
	time_t getCreatedAt();
	time_t getUpdatedAt();
	std::string toString() const;

private:
	const vemt::type::IntParam id_;
	vemt::type::IntParam discord_user_id_;
	vemt::type::StringParam package_url_;
	vemt::type::IntParam current_phase_;
	vemt::type::IntParam query_status_message_id_;
	vemt::type::IntParam working_status_message_id_;
	vemt::type::DatetimeParam created_at_;
	vemt::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
