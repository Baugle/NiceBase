#include"Config.h"

namespace Base::Config
{
	bool isFileExist(std::string path)
	{
		if (_access(path.c_str(), 0) == -1)	//如果文件夹不存在
			return false;
		return true;
	}
	void creatFile(std::string path)
	{
		_mkdir(path.c_str());
	}

	void checkIsFileExistAndCreat()
	{
		if (isFileExist(PATH) == false)
			creatFile(PATH);

	}
}