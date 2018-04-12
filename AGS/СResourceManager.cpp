#include "ÑResourceManager.h"

int CResourceManager::LoadMesh(std::string filename)
{
	int meshID;
	auto variable = meshes_index.find(filename);
	if (variable != meshes_index.end())
	{
		std::cout <<"Mesh: "<< filename << " exist "<< variable->second<<std::endl;
		return variable->second;
	}
	else
	{
		CMesh* Mesh = new CMesh;
		Mesh->Load(filename.c_str());
		meshes.push_back(Mesh);
		meshes_index.insert(std::pair<std::string, int>(filename, (int)meshes.size() - 1));
		std::cout << "Mesh: " << filename << " is loaded with ID: " << (int)meshes.size() - 1 << std::endl;
		return (int)meshes.size() - 1;
	}
}

int CResourceManager::LoadTexture(std::string filename)
{
	int textureID;
	auto variable = textures_index.find(filename);
	if (variable != textures_index.end())
	{
		std::cout << "Texture: " << filename << " exist " << variable->second << std::endl;
		return variable->second;
	}
	else
	{
		CTexture* Texture = new CTexture;
		Texture->Load(filename.c_str());
		textures.push_back(Texture);
		textures_index.insert(std::pair<std::string, int>(filename, (int)textures.size()-1));
		std::cout << "Texture: " << filename << " is loaded with ID: " << (int)textures.size() - 1 << std::endl;
		return (int)textures.size() - 1;
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

CTexture * CResourceManager::GetTexture(int index)
{
	CTexture *Texture;
	try
	{
		Texture = textures[index];
	}
	catch (std::exception ex)
	{
		return nullptr;
	}
	return Texture;
}
