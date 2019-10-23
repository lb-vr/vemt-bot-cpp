#ifndef VEMT_DB_SUBMISSIONMODEL_INCLUDED
#define VEMT_DB_SUBMISSIONMODEL_INCLUDED
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "type/Type.hpp"
namespace vemt
{
namespace db
{

class SubmissionModel
{
public:
    SubmissionModel(
        const vemt::type::IntParam & id,
        const vemt::type::IntParam & entry_id,
        const vemt::type::StringParam & package_url,
        const vemt::type::PhaseParam & current_phase,
        const vemt::type::DatetimeParam & created_at,
        const vemt::type::DatetimeParam & updated_at
    ) noexcept;
    SubmissionModel(
        const vemt::type::IntParam & entry_id_,
        const vemt::type::StringParam & package_url,
        const vemt::type::PhaseParam & current_phase
    ) noexcept;
    ~SubmissionModel(){}

    const vemt::type::IntParam & getId() const;
    const vemt::type::IntParam & getEntryId() const;
    const vemt::type::StringParam & getPackageUrl() const;
    const vemt::type::PhaseParam & getCurrentPhase() const;
    const vemt::type::DatetimeParam & getCreatedAt() const;
    const vemt::type::DatetimeParam & getUpdatedAt() const;

    void setEntryId(const vemt::type::IntParam & value);
    void setPackageUrl(const vemt::type::StringParam & value);
    void setCurrentPhase(const vemt::type::PhaseParam & value);

    std::string toString();

private:
    vemt::type::IntParam id_;
    vemt::type::IntParam entry_id_;
    vemt::type::StringParam package_url_;
    vemt::type::PhaseParam current_phase_;
    vemt::type::DatetimeParam created_at_;
    vemt::type::DatetimeParam updated_at_;
};
} // namespace db
} // namespace vemt
#endif
