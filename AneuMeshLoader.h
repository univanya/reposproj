#pragma once
#include "MeshLoader.h"
#include "Types.h"

class AneuMeshLoader :
	public MeshLoader
{
private:
	std::vector<Node> nodes;
	std::vector<FElement> elements;
	std::vector<SurfaceFE> surfaces;
	std::vector<int> findSurf(int id1, int id_2);
	Node interpolatedNode(Node node_1, Node node_2);
public:

	virtual void LoadMesh(const std::string& path) override;
	std::vector<Node>& getNodes();
	std::vector<FElement>& getElements();
	std::vector<SurfaceFE>& getSurfaces();
	std::vector<FElement> findElem(int id_1, int id_2, int id_3);
	std::vector<FElement> findElem(int id_1, int id_2);

	std::vector<int> getNodesBySurfID(int sur_ID);
	std::vector<FElement> getElementsByMatID(int mat_ID);
	std::vector<SurfaceFE> getSurfacesByID(int sur_ID);
	std::vector<std::vector<int>> getNeighbors();
	void quadrateMeshElements();
};


std::ostream& operator<<(std::ostream& stream, AneuMeshLoader& mesh);