#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include <filesystem>
class FileReader
{
public:
	FileReader();
	std::vector<unsigned char> ReadFile(std::string path);
private:
};

