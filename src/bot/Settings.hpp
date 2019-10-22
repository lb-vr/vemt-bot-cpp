#ifndef VEMT_BOT_SETTINGS_HPP
#define VEMT_BOT_SETTINGS_HPP

#include <unordered_map>
#include <stdexcept>

namespace vemt {
namespace bot {

class Settings {
public:

	class SettingsNotFoundException : std::runtime_error {
		using std::runtime_error::runtime_error;
	};
	class InvalidFormattedException : std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	Settings(
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
		const int64_t evetyone_role
	);

	Settings(const Settings &) noexcept = default;

	// Server
	const int64_t getServerID() const;

	// Category
	const int64_t getBotCategory() const;
	const int64_t getContactCategory() const;

	// Channel
	const int64_t getBotControlChannel() const;
	const int64_t getEntryChannel() const;
	const int64_t getStatusChannel() const;
	const int64_t getQueryChannel() const;

	// Role
	const int64_t getBotAdminRole() const;
	const int64_t getExhibitorRole() const;
	const int64_t getManagerRole() const;
	const int64_t getVemtBotRole() const;
	const int64_t getEveryoneRole() const;

	const std::vector<int64_t> getAllCreatedChannels() const;
	const std::vector<int64_t> getAllCreatedCategories() const;
	const std::vector<int64_t> getAllCreatedRoles() const;

	void save() const;

	static Settings & getSettings(const int64_t serverID);

	static void clearCache(const int64_t serverID);

private:
	const int64_t server_id_;

	const int64_t bot_category_;
	const int64_t contact_category_;
	
	const int64_t bot_control_channel_;
	const int64_t entry_channel_;
	const int64_t status_channel_;
	const int64_t query_channel_;

	const int64_t bot_admin_role_;
	const int64_t exhibitor_role_;
	const int64_t manager_role_;
	const int64_t vemt_bot_role_;
	const int64_t everyone_role_;

	static std::unordered_map<int64_t, Settings> server_settings_;

	static const std::string _getFilename(const int64_t serverID);
};

}
}

#endif