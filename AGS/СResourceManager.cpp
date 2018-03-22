#include "ÑResourceManager.h"

int CResourceManager::LoadMesh(std::string filename)
{
	int meshID;
	auto variable = meshes_index.find(filename);
	if (variable != meshes_index.end())
	{
		return variable->second;
	}
	else
	{
		CMesh* Mesh = new CMesh;
		Mesh->Load(filename.c_str());
		meshes.push_back(Mesh);
		meshes_index.insert(std::pair<std::string, int>(filename, (int)meshes.size() - 1));
		return (int)meshes.size() - 1;
	}
}

CMesh * CResourceManager::GetMesh(int index)
{
	CMesh *Mesh;
	try
	{
		Mesh = meshes[index];
	}
	catch (std::exception ex)
	{
		return nullptr;
	}
	return Mesh;
}