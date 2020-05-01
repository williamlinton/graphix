#include "FileReader.h"

FileReader::FileReader()
{

}

std::vector<unsigned char> FileReader::ReadFile(std::string path)
{
	std::vector<wchar_t> cd;
	cd.resize(256);
	GetCurrentDirectory(256, &cd[0]);

	std::vector<unsigned char> bytes;
	FILE* handle;
	errno_t err = fopen_s(&handle, path.c_str(), "rb");
	fseek(handle, 0, SEEK_END);
	long filesize = ftell(handle);
	fseek(handle, 0, 0);
	bytes.resize(filesize);
	fread_s(&bytes[0], filesize, sizeof(unsigned char), filesize, handle);
	fclose(handle);
	return bytes;
}
