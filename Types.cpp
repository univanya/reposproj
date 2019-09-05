#include "Types.h"

std::ostream& operator<<(std::ostream& stream, const Node& node)
{
	stream << "Node ID: " << node.ID << "\tCoords: ";
	for (const auto& a : node.coords)
		stream << a << '\t';
	stream << std::endl;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const FElement& elem)
{
	stream << "FElement ID: " << elem.ID << "\tMat. ID: " << elem.mat_ID << "\tNode IDs: ";
	for (const auto& a : elem.node_ids)
		stream << a << '\t';
	stream << std::endl;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const SurfaceFE& surf)
{
	stream << "Surface ID: " << surf.ID << "\tMat. ID: " << surf.FE_type_ID << "\tNode IDs: ";
	for (const auto& a : surf.node_ids)
		stream << a << '\t';
	stream << std::endl;
	return stream;
}