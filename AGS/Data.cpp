#include "Data.h"

CShader		Shader;
CCamera		Camera;
CMesh		Mesh;
LARGE_INTEGER oldValue, newValue, frequency;

double simulationTimePassed;
POINT newPossition, oldPossition;

int MeshId[3];

void MeshesInit()
{
	MeshId[0] = CResourceManager::Instance().LoadMesh("MESHES\\buildings\\chicken_shop.obj");
	MeshId[1] = CResourceManager::Instance().LoadMesh("MESHES\\buildings\\chicken_shop.obj");
	MeshId[2] = CResourceManager::Instance().LoadMesh("MESHES\\buildings\\drug_store.obj");
}

void ShaderInit()
{
	Shader.LoadVertexShader((char*)"SHADER\\Example.vsh");
	Shader.LoadFragmentShader((char*)"SHADER\\Example.fsh");
	Shader.Link();
}