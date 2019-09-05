#pragma once
#include <iostream>
#include <vector>

struct Node
{
	int ID;
	std::vector<float> coords;
	bool IsTop = true;
};

struct FElement
{
	int ID;
	int mat_ID;
	std::vector<int> node_ids;
};

struct SurfaceFE
{
	int ID;
	int FE_type_ID;
	int Surf_type_ID;
	std::vector<int> node_ids;
};

std::ostream& operator<<(std::ostream& stream, const Node& node);
std::ostream& operator<<(std::ostream& stream, const FElement& element);
std::ostream& operator<<(std::ostream& stream, const SurfaceFE& surface);