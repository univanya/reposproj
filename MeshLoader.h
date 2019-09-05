#pragma once
#include <string>
class MeshLoader
{
public:
	virtual void LoadMesh(const std::string& path) = 0;
};