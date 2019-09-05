#include "AneuMeshLoader.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <list>
#include <set>
#include <unordered_map>
#include <functional>
#include <boost\functional\hash.hpp>

void  AneuMeshLoader::LoadMesh(const std::string& path) 
{
	std::ifstream file(path);
	if (!file) 
	{
		throw std::runtime_error("Unable to load mesh file");
	}
	int count, dimension;
	file >> count >> dimension;
	nodes.reserve(count);
	for (int i = 1; i <= count; i++) 
	{
		Node temp_node;
		temp_node.ID = i;
		temp_node.coords.reserve(dimension);
		float coord;
		for (int j = 0; j != dimension; j++) 
		{
			file >> coord;
			temp_node.coords.push_back(coord);
		}
		nodes.push_back(temp_node);
	}

	file >> count >> dimension;
	elements.reserve(count);
	for (int i = 1; i <= count; i++)
	{
		FElement temp_element;
		temp_element.ID = i;
		file >> temp_element.mat_ID;
		int node_ID;
		for (int j = 0; j != dimension; j++)
		{
			file >> node_ID;
			temp_element.node_ids.push_back(node_ID);
		}
		elements.push_back(temp_element);
	}

	file >> count >> dimension;
	surfaces.reserve(count);
	for (int i = 1; i <= count; i++)
	{
		SurfaceFE temp_surface;
		file >> temp_surface.ID;
		int node_ID;
		for (int j = 0; j != dimension; j++)
		{
			file >> node_ID;
			temp_surface.node_ids.push_back(node_ID);
		}
		surfaces.push_back(temp_surface);
	}
	file.close();
}
std::vector<Node>& AneuMeshLoader::getNodes()
{
	return nodes;
}

std::vector<FElement>& AneuMeshLoader::getElements()
{
	return elements;
}

std::vector<SurfaceFE>& AneuMeshLoader::getSurfaces()
{
	return surfaces;
}

std::ostream& operator<<(std::ostream& stream, AneuMeshLoader& mesh)
{
	stream << "Nodes:" << std::endl;
	for (auto &n : mesh.getNodes())
	{
		stream << "ID: " << n.ID << "\tCoords: ";
		for (auto &c : n.coords) stream << "\t" << c;
		stream << std::endl;
	}

	stream << "Elements:" << std::endl;
	for (auto &e : mesh.getElements())
	{
		stream << "ID: " << e.ID << "\tMaterial ID: " << e.mat_ID << "\tNodes\' IDs: ";
		for (auto &n : e.node_ids) stream << ' ' << n;
		stream << std::endl;
	}

	stream << "Surfaces:" << std::endl;
	for (auto &s : mesh.getSurfaces())
	{
		stream << "ID: " << s.ID << "\tNodes\' IDs: ";
		for (auto &n : s.node_ids) stream << ' ' << n;
		stream << std::endl;
	}

	return stream;
}

std::vector<FElement> AneuMeshLoader::findElem(int id_1, int id_2, int id_3)
{
	std::list<FElement> found_elements;
	auto last = elements.begin();
	while (last != elements.end())
	{
		last = find_if(last, elements.end(), [id_1, id_2, id_3](FElement& elem)
		{
			return (std::find(elem.node_ids.begin(), elem.node_ids.end(), id_1) != elem.node_ids.end()
				&& std::find(elem.node_ids.begin(), elem.node_ids.end(), id_2) != elem.node_ids.end()
				&& std::find(elem.node_ids.begin(), elem.node_ids.end(), id_3) != elem.node_ids.end());
		});

		if (last != elements.end())
		{
			found_elements.push_back(*last);
			++last;
		}
	}
	return std::vector<FElement>(found_elements.begin(), found_elements.end());
}

std::vector<FElement> AneuMeshLoader::findElem(int id_1, int id_2)
{
	std::list<FElement> found_elements;
	auto   last = elements.begin();
	while (last != elements.end())
	{
		last = find_if(last, elements.end(), [id_1, id_2](FElement& elem)
		{
			return (std::find(elem.node_ids.begin(), elem.node_ids.end(), id_1) != elem.node_ids.end()
				&& std::find(elem.node_ids.begin(), elem.node_ids.end(), id_2) != elem.node_ids.end());
		});
		if (last != elements.end())
			found_elements.push_back(*last++);
	}
	return std::vector<FElement>(found_elements.begin(), found_elements.end());
}

std::vector<int> AneuMeshLoader::getNodesBySurfID(int sur_ID)
{
	std::set<int> cont;
	for (const auto &s : surfaces)
		if (s.ID == sur_ID)
			for (auto &n : s.node_ids)
				cont.insert(n);
	return std::vector<int>(cont.begin(), cont.end());
}

std::vector<FElement> AneuMeshLoader::getElementsByMatID(int mat_ID)
{
	std::list<FElement>  cont;
	for (const auto &e : elements)
		if (e.mat_ID == mat_ID)
			cont.push_back(e);
	return std::vector<FElement>(cont.begin(), cont.end());
}

std::vector<SurfaceFE> AneuMeshLoader::getSurfacesByID(int sur_ID)
{
	std::list<SurfaceFE>  cont;
	for (const auto &s : surfaces)
		if (s.ID == sur_ID)
			cont.push_back(s);
	return std::vector<SurfaceFE>(cont.begin(), cont.end());
}

std::vector<std::vector<int>> AneuMeshLoader::getNeighbors()
{
	std::vector<std::vector<int>> container;
	container.reserve(nodes.size()*3);

	for (const auto &e : elements)
		for (const auto &node1 : e.node_ids)
			for (const auto &node2 : e.node_ids)
				container[node1].push_back(node2);
	container.shrink_to_fit();

	

	return container;
}

void AneuMeshLoader::quadrateMeshElements()
{
	nodes.reserve(nodes.size() * 3);  //подсчёт нового локального индекса по двум старым
	std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> edges; //рёбрышки
	for (auto& elem : elements)
	{
		elem.node_ids.resize(10);
	}
	for (auto& surf : surfaces)
	{
		surf.node_ids.resize(6);
	}
	for (auto& elem : elements)
	{
		for (int index = 4; index < 10; ++index)
		{
			if (elem.node_ids[index] == 0)
			{
				
				int id1, id2;
				switch (index)//поиск ID узлов ребра по локальному индексу промежуточного
				{
				case 4:
					id1 = 0;
					id2 = 1;
					break;
				case 5:
					id1 = 0;
					id2 = 2;
					break;
				case 6:
					id1 = 0;
					id2 = 3;
					break;
				case 7:
					id1 = 1;
					id2 = 2;
					break;
				case 8:
					id1 = 1;
					id2 = 3;
					break;
				case 9:
					id1 = 2;
					id2 = 3;
					break;
				}
				id1 = elem.node_ids[id1];
				id2 = elem.node_ids[id2];
				
				auto found_node_id = edges.find(std::make_pair(id1, id2));
				auto again_f_n_id = edges.find(std::make_pair(id2, id1));
				if (found_node_id != edges.end()) //Поиск созданного ребрышка
				{
					elements[elem.ID - 1].node_ids[index] = (*found_node_id).second;
				}
				else if (again_f_n_id != edges.end()) 
				{
					elements[elem.ID - 1].node_ids[index] = (*again_f_n_id).second;
				}
				else
				{
					Node new_node = interpolatedNode(nodes[id1 - 1], nodes[id2 - 1]);
					new_node.ID = nodes.size() + 1;
					new_node.IsTop = false;
					nodes.push_back(new_node);
					edges.insert(std::make_pair(std::make_pair(id1, id2), new_node.ID));
					elements[elem.ID - 1].node_ids[index] = new_node.ID;
				}
			}
		}
	}
	int count = 0;// счетчик для поверхностей
	for (auto& surf : surfaces)
	{
		for (int index = 3; index < 6; ++index) 
		{
			if (surf.node_ids[index] == 0) 
			{
				
				int id1, id2;
				switch (index)//поиск ID узлов ребрышка по локальному индексу промежуточного
				{
				case 3:
					id1 = 0;
					id2 = 1;
					break;
				case 4:
					id1 = 1;
					id2 = 2;
					break;
				case 5:
					id1 = 0;
					id2 = 2;
					break;
				}
				id1 = surf.node_ids[id1];
				id2 = surf.node_ids[id2];
				
				auto found_node_id = edges.find(std::make_pair(id1, id2));
				auto again_f_n_id = edges.find(std::make_pair(id2, id1));
				if (found_node_id != edges.end()) //Поиск созданного ребрышка
				{
					surfaces[count].node_ids[index] = (*found_node_id).second;
				}
				else if (again_f_n_id != edges.end()) 
				{
					surfaces[count].node_ids[index] = (*again_f_n_id).second;
				}
			}
		}
		++count;
	}
	nodes.shrink_to_fit();
}
std::vector<int> AneuMeshLoader::findSurf(int id_1, int id_2)
{
	std::list<int> found_elements;
	auto   last = surfaces.begin();
	int count = 0;
	while (last != surfaces.end())
	{

		last = find_if(last, surfaces.end(), [id_1, id_2, &count](SurfaceFE& elem)
		{
			count++;
			return (std::find(elem.node_ids.begin(), elem.node_ids.end(), id_1) != elem.node_ids.end()
				&& std::find(elem.node_ids.begin(), elem.node_ids.end(), id_2) != elem.node_ids.end());
		});

		if (last != surfaces.end())
		{
			last++;
			found_elements.push_back(count - 1);
		}
	}
	return std::vector<int>(found_elements.begin(), found_elements.end());
}

Node AneuMeshLoader::interpolatedNode(Node node_1, Node node_2)
{
	int dimensions = node_1.coords.size();
	if (dimensions != node_2.coords.size())
		throw std::runtime_error("Nodes\' dimensions don\'t match");

	Node result_node;
	result_node.coords.reserve(dimensions);
	for (int i = 0; i != dimensions; i++)
		result_node.coords.push_back((node_1.coords[i] + node_2.coords[i]) / 2.0f);
	return result_node;
}