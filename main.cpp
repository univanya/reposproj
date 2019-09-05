#include <iostream>
#include "AneuMeshLoader.h"
#include "Menu.h"
#include "set"
#include <Algorithm>
#include "Algorithm.h"
int main(int argc, char** argv)
{
	std::string path;
	AneuMeshLoader mesh;
	bool loaded = false;
	while (!loaded)
	{
		try {
			std::cout << "File load: ";
			if (argc > 1)
				std::cout << (path = argv[1]) << std::endl;
			else
				std::cin >> path;
			mesh.LoadMesh(path);
			loaded = true;
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
	std::cout << "Mesh was successfully loaded" << std::endl << std::endl;

	cls_Application menu;
	menu.MainNode({
		"Print loaded data",
		"Find element by 2 node IDs",
		"Find element by 3 node IDs",
		"Get nodes by surface ID",
		"Get elements by material ID",
		"get surfaces by ID",
		"Quadrate mesh",
		"Get nodes neighbours"
	}, {
		[&mesh]() {std::cout << "\nYour Mesh:\n" << mesh; },
		[&mesh]() {
			std::cout << "Enter node IDs: ";
			int ids[2];
			std::cin >> ids[0] >> ids[1];
			auto result = mesh.findElem(ids[0], ids[1]);
			std::cout << std::endl;
			for (auto &a : result)
			{
				std::cout << "FElemenet ID: " << a.ID << "\tMaterial ID: " << a.mat_ID << "\tNodes\' IDs: ";
				for (auto &b : a.node_ids) std::cout << ' ' << b;
				std::cout << std::endl;
			}
		},
			[&mesh]()
		{
			std::cout << "Enter node IDs: ";
			int ids[3];
			std::cin >> ids[0] >> ids[1] >> ids[2];
			std::cout << std::endl;
			auto result = mesh.findElem(ids[0], ids[1], ids[2]);
			for (auto &a : result)
			{
				std::cout << "Node ID: " << a.ID << "\tMaterial ID: " << a.mat_ID << "\tNodes\' IDs: ";
				for (auto &b : a.node_ids) std::cout << ' ' << b;
				std::cout << std::endl;
			}
		},
			[&mesh]()
		{
			std::cout << "Enter surface ID: ";
			int id;
			std::cin >> id;
			std::cout << std::endl;
			std::vector<int> result = mesh.getNodesBySurfID(id);
			std::cout << "Found Surf_Elements: " << std::endl;
			for (auto &a : result) std::cout << a << std::endl;
		},
			[&mesh]()
		{
			std::cout << "Enter material ID: ";
			int id;
			std::cin >> id;
			std::cout << std::endl;
			std::vector<FElement> result = mesh.getElementsByMatID(id);
			std::cout << "Found elements: " << std::endl;
			ForEach(result.begin(), result.end(), [](FElement elem) {std::cout << elem; });
		},
			[&mesh]() {
			std::cout << "Enter surface ID: ";
			int id;
			std::cin >> id;
			std::cout << std::endl;
			std::vector<SurfaceFE> result = mesh.getSurfacesByID(id);
			std::cout << "Found surfaces: " << std::endl;
			for (auto &a : result)
			{
				std::cout << "ID: " << a.ID << "\tNodes\' IDs: ";
				for (auto &b : a.node_ids) std::cout << ' ' << b;
				std::cout << std::endl;
			}
		},
			[&mesh]() 
		{
			mesh.quadrateMeshElements();
			std::cout << "Mesh succsessfully quadrated" << std::endl;
		},
			[&mesh]() 
		{
			auto result = mesh.getNeighbors();
			for (int i = 0; i != result.size(); i++)
			{
				std::cout << "Neighbors of node " << i + 1 << ":\t";
				for (int j = 0; j != result[i].size(); j++)
					std::cout << ' ' << result[i][j];
				std::cout << std::endl;
			}
		}
	}); 
	menu.Run();
	std::getchar();
	return 0;
}