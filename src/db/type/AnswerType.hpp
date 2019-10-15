#ifndef VEMT_DB_TYPE_ANSWERTYPE_HPP
#define VEMT_DB_TYPE_ANSWERTYPE_HPP

namespace vemt {
namespace db {
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
}

#endif