#pragma once
#include "Common.h"

#include <io.h>
#include <direct.h>

#include <string>

namespace Base::Config
{
	bool isFileExist(std::string path);
	void creatFile(std::string path);

	void checkIsFileExistAndCreat();

}