#include "EntryModel.hpp"
#include <sstream>

vemt::db::EntryModel::EntryModel(
	const vemt::type::IntParam & id,
	const vemt::type::IntParam & discord_uid,
	const vemt::type::IntParam & current_phase,
	const vemt::type::IntParam & contact_channel_id,
	const vemt::type::IntParam & questionary_message_id,
	const vemt::type::DatetimeParam & created_at,
	const vemt::type::DatetimeParam & updated_at
) noexcept :
	id_(id),
	discord_user_id_(discord_uid),
	current_phase_(current_phase),
	contact_channel_id_(contact_channel_id),
	questionary_message_id_(questionary_message_id),
	created_at_(created_at),
	updated_at_(updated_at){
}
vemt::db::EntryModel::EntryModel(
	const vemt::type::IntParam & discord_uid,
	const vemt::type::IntParam & current_phase,
	const vemt::type::IntParam & contact_channel_id,
	const vemt::type::IntParam & questionary_message_id
) noexcept :
	discord_user_id_(discord_uid),
	current_phase_(current_phase),
	contact_channel_id_(contact_channel_id),
	questionary_message_id_(questionary_message_id){
}
vemt::db::EntryModel::EntryModel(const EntryModel & c) noexcept :
	id_(c.id_),
	discord_user_id_(c.discord_user_id_),
	current_phase_(c.current_phase_),
	contact_channel_id_(c.contact_channel_id_),
	questionary_message_id_(c.questionary_message_id_),
	created_at_(c.created_at_),
	updated_at_(c.updated_at_){
}

int vemt::db::EntryModel::getId() const{
	return this->id_.get();
}
int vemt::db::EntryModel::getDiscordUid() const{
	return this->discord_user_id_.get();
}
int vemt::db::EntryModel::getCurrentPhase() const{
	return this->current_phase_.get();
}
int vemt::db::EntryModel::getQueryStatusMessageId() const{
	return this->contact_channel_id_.get();
}
int vemt::db::EntryModel::getWorkingStatusMessageId() const{
	return this->questionary_message_id_.get();
}
time_t vemt::db::EntryModel::getCreatedAt() const{
	return this->created_at_.get();
}
time_t vemt::db::EntryModel::getUpdatedAt() const{
	return this->updated_at_.get();
}

std::string vemt::db::EntryModel::toString() const {
	std::stringstream ret;
    ret << "{model: 'EntryModel', "
		<< "id: '" << id_.get() << "', "
		<< "discord_user_id: '" << discord_user_id_.get() << "', "
		<< "current_phase: '" << current_phase_.get() << "', "
		<< "contact_channel_id: '" << contact_channel_id_.get() << "', "
		<< "questionary_message_id: '" << questionary_message_id_.get() << "', "
		<< "created_at: '" << created_at_.getAsString() << "', "
		<< "updated_at: '" << updated_at_.getAsString() << "'"
		<< "}";
    return ret.str();
}
