#include "Settings.h"

std::string GetShaderPath(std::string shaderName)
{
	std::string fullpath = DIR_CSO_FILES;

	fullpath.append("\\");
	fullpath.append(shaderName);

	return fullpath;
}

std::string GetResourcePath(std::string resourceFileName)
{
	std::string fullpath = DIR_RESOURCES;

	fullpath.append("\\");
	fullpath.append(resourceFileName);

	return fullpath;
}
