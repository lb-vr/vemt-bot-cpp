#include "http.h"

const char * dscd::GenericSession::getMethodName(const RequestMethod & method) {
	static constexpr char const * methodNames[] = { "POST", "PATCH", "DELETE", "GET", "PUT" };
	return methodNames[method];
}
