#pragma once
#include <vector>
#include <string>
class FileReader
{
public:
	FileReader();
	std::vector<unsigned char> ReadFile(std::string path);
private:
};

