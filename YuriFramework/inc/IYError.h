#pragma once
#include "YError.h"

#include <string>

class IYError {
public:
	std::string getLastError() const;
protected:
	void setError(YError error);
	YError currentError = YError::NONE;
};