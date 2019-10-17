#ifndef VEMT_DB_ENTRYMODEL_INCLUDED
#define VEMT_DB_ENTRYMODEL_INCLUDED
#include "type/Type.hpp"
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
		const vemt::type::IntParam & current_phase,
		const vemt::type::IntParam & contact_channel_id_,
		const vemt::type::IntParam & questionary_message_id_,
		const vemt::type::DatetimeParam & created_at,
		const vemt::type::DatetimeParam & updated_at
	) noexcept;
	EntryModel(
		const vemt::type::IntParam & discord_uid,
		const vemt::type::IntParam & current_phase,
		const vemt::type::IntParam & contact_channel_id_,
		const vemt::type::IntParam & questionary_message_id_
	) noexcept;
	EntryModel(const EntryModel & copy) noexcept;
	~EntryModel(){}

	int getId() const;
	int getDiscordUid() const;
	int getCurrentPhase() const;
	int getQueryStatusMessageId() const;
	int getWorkingStatusMessageId() const;
	time_t getCreatedAt() const;
	time_t getUpdatedAt() const;
	std::string toString() const;

private:
	const vemt::type::IntParam id_;
	vemt::type::IntParam discord_user_id_;
	vemt::type::IntParam current_phase_;
	vemt::type::IntParam contact_channel_id_;
	vemt::type::IntParam questionary_message_id_;
	vemt::type::DatetimeParam created_at_;
	vemt::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
