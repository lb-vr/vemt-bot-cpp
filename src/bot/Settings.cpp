#include "Settings.hpp"
#include "util/json11.hpp"
#include <string>
#include <fstream>
#include "util/Logger.hpp"

std::unordered_map<int64_t, vemt::bot::Settings> vemt::bot::Settings::server_settings_;

vemt::bot::Settings::Settings(
	const int64_t serverID,
	const int64_t bot_category,
	const int64_t contact_category,
	const int64_t bot_control_channel,
	const int64_t entry_channel,
	const int64_t status_channel,
	const int64_t query_channel,
	const int64_t bot_admin_role,
	const int64_t exhibitor_role,
	const int64_t manager_role,
	const int64_t vemt_bot_role,
	const int64_t everyone_role
):
	server_id_			(serverID),
	bot_category_		(bot_category),
	contact_category_	(contact_category),
	bot_control_channel_(bot_control_channel),
	entry_channel_		(entry_channel),
	status_channel_		(status_channel),
	query_channel_		(query_channel),
	bot_admin_role_		(bot_admin_role),
	exhibitor_role_		(exhibitor_role),
	manager_role_		(manager_role),
	vemt_bot_role_		(vemt_bot_role),
	everyone_role_		(everyone_role)
{}


const int64_t vemt::bot::Settings::getServerID() const			{ return this->server_id_; }

const int64_t vemt::bot::Settings::getBotCategory() const		{ return this->bot_category_; }
const int64_t vemt::bot::Settings::getContactCategory() const	{ return this->contact_category_; }

const int64_t vemt::bot::Settings::getBotControlChannel() const { return this->bot_control_channel_; }
const int64_t vemt::bot::Settings::getEntryChannel() const		{ return this->entry_channel_; }
const int64_t vemt::bot::Settings::getStatusChannel() const		{ return this->status_channel_; }
const int64_t vemt::bot::Settings::getQueryChannel() const		{ return this->query_channel_; }

const int64_t vemt::bot::Settings::getBotAdminRole() const		{ return this->bot_admin_role_; }
const int64_t vemt::bot::Settings::getExhibitorRole() const		{ return this->exhibitor_role_; }
const int64_t vemt::bot::Settings::getManagerRole() const		{ return this->manager_role_; }
const int64_t vemt::bot::Settings::getVemtBotRole() const		{ return this->vemt_bot_role_; }
const int64_t vemt::bot::Settings::getEveryoneRole() const		{ return this->everyone_role_; }

const std::vector<int64_t> vemt::bot::Settings::getAllCreatedChannels() const {
	return std::vector<int64_t>({
		this->getBotControlChannel(),
		this->getEntryChannel(),
		this->getStatusChannel(),
		this->getQueryChannel()
		});
}

const std::vector<int64_t> vemt::bot::Settings::getAllCreatedCategories() const {
	return std::vector<int64_t>({
		this->getBotCategory(),
		this->getContactCategory()
		});
}

const std::vector<int64_t> vemt::bot::Settings::getAllCreatedRoles() const {
	return std::vector<int64_t>({
		this->getBotAdminRole(),
		this->getExhibitorRole(),
		this->getManagerRole()
		});
}

const std::string vemt::bot::Settings::_getFilename(const int64_t serverID) {
	return std::to_string(serverID) + ".json";
}

vemt::bot::Settings & vemt::bot::Settings::getSettings(const int64_t serverID) {
	if (Settings::server_settings_.count(serverID)) {
		return Settings::server_settings_.at(serverID);
	}
	logging::info << "Require server settings. Server ID = " << serverID << std::endl;

	const std::string filename = Settings::_getFilename(serverID);
	logging::debug << " - filename = " << filename << std::endl;

	std::ifstream ifst(filename, std::ios::in);
	if (!ifst) SettingsNotFoundException("Server ID = " + std::to_string(serverID));

	std::istreambuf_iterator<char> it(ifst);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);
	std::string err;
	json11::Json json = json11::Json::parse(str, err);
	if (!err.empty()) throw InvalidFormattedException("Invalid json format. Reason = " + err);

	try {
		Settings settings = Settings(
			std::stoll(json["server"].asString()),
			std::stoll(json["categories"]["bot"].asString()),
			std::stoll(json["categories"]["contact"].asString()),
			std::stoll(json["channels"]["bot_control"].asString()),
			std::stoll(json["channels"]["entry"].asString()),
			std::stoll(json["channels"]["status"].asString()),
			std::stoll(json["channels"]["query"].asString()),
			std::stoll(json["roles"]["bot_admin"].asString()),
			std::stoll(json["roles"]["exhibitor"].asString()),
			std::stoll(json["roles"]["manager"].asString()),
			std::stoll(json["roles"]["vemt_bot"].asString()),
			std::stoll(json["roles"]["everyone"].asString()));

		logging::debug << "-------------------------------------------------------" << std::endl;
		logging::debug << " - [SV:server]          : " << settings.getServerID() << std::endl;
		logging::debug << " -   [CAT:BOT]          : " << settings.getBotCategory() << std::endl;
		logging::debug << " -     [CH:bot_control] : " << settings.getBotControlChannel() << std::endl;
		logging::debug << " -     [CH:entry]       : " << settings.getEntryChannel() << std::endl;
		logging::debug << " -     [CH:status]      : " << settings.getStatusChannel() << std::endl;
		logging::debug << " -     [CH:query]       : " << settings.getQueryChannel() << std::endl;
		logging::debug << " -   [CAT:CONTACT]      : " << settings.getContactCategory() << std::endl;
		logging::debug << "-------------------------------------------------------" << std::endl;
		logging::debug << " - Role [BotAdmin]      : " << settings.getBotAdminRole() << std::endl;
		logging::debug << " - Role [Exhibitor]     : " << settings.getExhibitorRole() << std::endl;
		logging::debug << " - Role [Manager]       : " << settings.getManagerRole() << std::endl;
		logging::debug << " - Role [vemt-bot]      : " << settings.getVemtBotRole() << std::endl;
		logging::debug << "-------------------------------------------------------" << std::endl;

		Settings::server_settings_.emplace(settings.getServerID(), settings);
		return Settings::server_settings_.at(settings.getServerID());
	}
	catch (std::invalid_argument e) {
		throw InvalidFormattedException("Invalid Argument. Reason = " + std::string(e.what()));
	}
}

void vemt::bot::Settings::save() const {
	const std::string filename = Settings::_getFilename(this->getServerID());

	json11::Json _server_id			(std::to_string(this->getServerID()));

	json11::Json _channels({
		{"bot_control",	json11::Json(std::to_string(this->getBotControlChannel()))},
		{"entry",		json11::Json(std::to_string(this->getEntryChannel()))},
		{"status",		json11::Json(std::to_string(this->getStatusChannel()))},
		{"query",		json11::Json(std::to_string(this->getQueryChannel()))}
		});

	json11::Json _categories({
		{"bot",			json11::Json(std::to_string(this->getBotCategory()))},
		{"contact",		json11::Json(std::to_string(this->getContactCategory()))}
		});
	
	json11::Json _roles({
		{"bot_admin",	json11::Json(std::to_string(this->getBotAdminRole()))},
		{"exhibitor",	json11::Json(std::to_string(this->getExhibitorRole()))},
		{"manager",		json11::Json(std::to_string(this->getManagerRole()))},
		{"vemt_bot",	json11::Json(std::to_string(this->getVemtBotRole()))},
		{"everyone",    json11::Json(std::to_string(this->getEveryoneRole()))}
		});

	json11::Json _root({
		{"server",		_server_id},
		{"channels",	_channels},
		{"categories",	_categories},
		{"roles",		_roles}
		});

	std::ofstream ofst(filename);
	if (!ofst) throw SettingsNotFoundException("Settings file is not found. PATH = " + filename);
	ofst << _root.dump();

	Settings::server_settings_.emplace(this->getServerID(), *this);
}

void vemt::bot::Settings::clearCache(const int64_t serverID) {
	Settings::server_settings_.erase(serverID);
}
