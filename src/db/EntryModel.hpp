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

	const vemt::type::IntParam & getId() const;
	const vemt::type::IntParam & getDiscordUid() const;
	const vemt::type::IntParam & getCurrentPhase() const;
	const vemt::type::IntParam & getContactChannelId() const;
	const vemt::type::IntParam & getQuestionaryMessageId() const;
	const vemt::type::DatetimeParam & getCreatedAt() const;
	const vemt::type::DatetimeParam & getUpdatedAt() const;
	std::string toString() const;

	void setDiscordUid(const vemt::type::IntParam & value);
	void setCurrentPhase(const vemt::type::IntParam & value);
	void setContactChannelId(const vemt::type::IntParam & value);
	void setQuestionaryMessageId(const vemt::type::IntParam & value);

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
