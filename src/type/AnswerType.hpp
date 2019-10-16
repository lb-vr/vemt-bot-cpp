#ifndef VEMT_TYPE_ANSWERTYPE_HPP
#define VEMT_TYPE_ANSWERTYPE_HPP

namespace vemt {
namespace type {

enum struct AnswerType : int {
	kString,
	kNumber,
	kPicture,
	kJsonFile,
	kJson,
	kRegex
};

}
}

#endif