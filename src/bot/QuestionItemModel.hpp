#ifndef VEMT_QUESTIONITEMMODEL_HPP
#define VEMT_QUESTIONITEMMODEL_HPP

#include "Phase.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <regex>

namespace json11 {
class Json;	// �O���Q��
}

namespace vemt {
namespace bot {

class QuestionItemModel {
public:

	enum Type {
		kUnknown,
		kString,
		kNumber,
		kPicture,
		kJsonFile,
		kJson,
		kRegex
	};

	enum ValidationResult {
		kOk = 0,				///< ����
		kAnswerEmpty,			///< �񓚂��󂾂����ꍇ
		kUnexceptedType,		///< �񓚂̃^�C�v���~�X�}�b�`������
		kUnmatchRegex,			///< ���K�\���Ƀ}�b�`���Ȃ�����
		kOutOfChoise,			///< �I�����ڈȊO������
		kOverMaxLength,			///< �񓚕������K��ȏ�
		kUneditablePhase,		///< �ĕҏW�s�\�ȃt�F�[�Y
		kUneditableDatetime,	///< �ĕҏW�s�\�Ȏ���
	};

	/// @brief Database�p�̃R���X�g���N�^
	QuestionItemModel(
		const unsigned long int id,
		const std::string & text,
		const std::string & detail_text,
		const Type type,
		const std::string & regex_rule,
		const std::vector<std::string> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief Json�p�[�X�p�̃R���X�g���N�^
	///
	/// id�ɂ��Ă̓_�~�[������H
	QuestionItemModel(
		const std::string & text,
		const std::string & detail_text,
		const Type type,
		const std::string & regex_rule,
		const std::vector<std::string> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief �R�s�[�R���X�g���N�^
	QuestionItemModel(const QuestionItemModel & copy) noexcept;

	/// @brief �f�X�g���N�^
	~QuestionItemModel();

	/// @brief �����̕�����ɑ΂��ăo���f�[�e�B���O������
	ValidationResult validate(const std::string & answer) const;

	// -- Get Accessor --
	const unsigned long int getId() const;
	const std::string & getText() const;
	const std::string & getDetailText() const;
	const Type getType() const;
	const std::regex getRegex() const;
	const std::string getRegexRule() const;
	const std::vector<std::string> & getChoise() const;
	const unsigned int getLength() const;
	const bool getIsRequired() const;
	const Phase getRequiredWhenPhase() const;
	const std::chrono::system_clock::time_point getRequireWhenDatetime() const;
	const bool getMultiline() const;

	// -- Set Accessor --
	void setText(const std::string & text);
	void setDetailText(const std::string & detail_text);
	void setType(const Type type);
	void setRegexRule(const std::string & regex_rule);
	void setChoise(const std::vector<std::string> & choise);
	void setLength(const unsigned int length);
	void setIsRequired(const bool required);
	void setRequiredWhenPhase(const Phase phase);
	void setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint);
	void setMultiline(const bool multiline);

	// -- utility --
	std::string toString() const;

	// -- parser ---
	static QuestionItemModel createFromJson(const json11::Json & json, std::string & error_msg);

	// -- caster --
	static std::string type2str(const Type type);
	static Type str2type(const std::string & str);

private:
	const unsigned long int id_;			// ID�͐�΂ɕs��
	std::string text_;
	std::string detail_text_;
	Type type_;
	std::string regex_rule_;
	std::vector<std::string> choise_;
	unsigned int length_;
	Phase required_when_phase_;
	std::chrono::system_clock::time_point required_when_datetime_;
	bool multiline_;
	bool is_required_;
};



}
}

#endif