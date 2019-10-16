#include "Settings.hpp"
#include "util/json11.hpp"
#include <string>
#include <fstream>
#include "util/Logger.hpp"

std::unordered_map<int64_t, vemt::bot::Settings> vemt::bot::Settings::server_settings_;

vemt::bot::Settings::Settings(const int64_t serverID, const int64_t bot_category, const int64_t bot_control_channel,
	const int64_t entry_channel, const int64_t status_channel, const int64_t query_channel, const int64_t exhibitor_category, const int64_t bot_admin_role,
	const int64_t exhibitor_role, const int64_t manager_role, const int64_t vemt_bot_role, const int64_t everyone_role)
	: server_id_(serverID), bot_category_(bot_category), bot_control_channel_(bot_control_channel),
	entry_channel_(entry_channel), status_channel_(status_channel), query_channel_(query_channel),
	exhibitor_category_(exhibitor_category), bot_admin_role_(bot_admin_role), exhibitor_role_(exhibitor_role),
	manager_role_(manager_role), vemt_bot_role_(vemt_bot_role), everyone_role_(everyone_role) {}

vemt::bot::Settings::Settings(const Settings & s) noexcept
	: server_id_(s.server_id_), bot_category_(s.bot_category_), bot_control_channel_(s.bot_control_channel_),
	entry_channel_(s.entry_channel_), status_channel_(s.status_channel_), query_channel_(s.query_channel_),
	exhibitor_category_(s.exhibitor_category_), bot_admin_role_(s.bot_admin_role_), exhibitor_role_(s.exhibitor_role_),
	manager_role_(s.manager_role_), vemt_bot_role_(s.vemt_bot_role_), everyone_role_(s.everyone_role_) {}

const int64_t vemt::bot::Settings::getServerID() const { return this->server_id_; }
const int64_t vemt::bot::Settings::getBotCategory() const { return this->bot_category_; }
const int64_t vemt::bot::Settings::getBotControlChannel() const { return this->bot_control_channel_; }
const int64_t vemt::bot::Settings::getEntryChannel() const { return this->entry_channel_; }
const int64_t vemt::bot::Settings::getStatusChannel() const { return this->status_channel_; }
const int64_t vemt::bot::Settings::getQueryChannel() const { return this->query_channel_; }
const int64_t vemt::bot::Settings::getBotAdminRole() const { return this->bot_admin_role_; }
const int64_t vemt::bot::Settings::getExhibitorRole() const { return this->exhibitor_role_; }
const int64_t vemt::bot::Settings::getManagerRole() const { return this->manager_role_; }
const int64_t vemt::bot::Settings::getVemtBotRole() const { return this->vemt_bot_role_; }
const int64_t vemt::bot::Settings::getEveryoneRole() const { return this->everyone_role_; }
const int64_t vemt::bot::Settings::getExhibitorCategory() const { return this->exhibitor_category_; }

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
			std::stoll(json["channels"]["bot_control"].asString()),
			std::stoll(json["channels"]["entry"].asString()),
			std::stoll(json["channels"]["status"].asString()),
			std::stoll(json["channels"]["query"].asString()),
			std::stoll(json["categories"]["exhibitor"].asString()),
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
		logging::debug << " -   [CAT:EXHIBITOR]    : " << settings.getExhibitorCategory() << std::endl;
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

vemt::bot::Settings vemt::bot::Settings::createSettings(const int64_t serverID, const int64_t bot_category, const int64_t bot_control_channel, const int64_t entry_channel,
	const int64_t status_channel, const int64_t query_channel, const int64_t exhibitor_category, const int64_t bot_admin_role, const int64_t exhibitor_role,
	const int64_t manager_role, const int64_t vemt_bot_role, const int64_t everyone_role) {
	const std::string filename = Settings::_getFilename(serverID);

	json11::Json _server_id			(std::to_string(serverID));

	json11::Json _channels({
		{"bot_control",	json11::Json(std::to_string(bot_control_channel))},
		{"entry",		json11::Json(std::to_string(entry_channel))},
		{"status",		json11::Json(std::to_string(status_channel))},
		{"query",		json11::Json(std::to_string(query_channel))}
		});

	json11::Json _categories({
		{"bot",			json11::Json(std::to_string(bot_category))},
		{"exhibitor",	json11::Json(std::to_string(exhibitor_category))}
		});
	
	json11::Json _roles({
		{"bot_admin",	json11::Json(std::to_string(bot_admin_role))},
		{"exhibitor",	json11::Json(std::to_string(exhibitor_role))},
		{"manager",		json11::Json(std::to_string(manager_role))},
		{"vemt_bot",	json11::Json(std::to_string(vemt_bot_role))},
		{"everyone",    json11::Json(std::to_string(everyone_role))}
		});

	json11::Json _root({
		{"server",		json11::Json(std::to_string(serverID))},
		{"channels",	_channels},
		{"categories",	_categories},
		{"roles",		_roles}
		});

	std::ofstream ofst(filename);
	if (!ofst) throw SettingsNotFoundException("Settings file is not found. PATH = " + filename);
	ofst << _root.dump();

	Settings settings(
		serverID, bot_category, bot_control_channel, entry_channel, status_channel, query_channel,
		exhibitor_category, bot_admin_role, exhibitor_category, manager_role, vemt_bot_role, everyone_role);
	Settings::server_settings_.emplace(serverID, settings);
	return settings;
}
