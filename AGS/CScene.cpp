#include "CScene.h"

void CScene::init(void)
{
	pugi::xml_parse_result result = resources_description.load_file("Resources.xml");

	Light = CreateLight();

	// временная локальная переменная
	CGraphicObject TempGraphicObject;
	NetProtocol.init("127.0.0.1", 27000);
	std::cout << NetProtocol.getMessage();

}

void CScene::simulate(float sec)
{
	vec3 position = Camera.GetPosition();
	auto descriptions = NetProtocol.getNearlyObjects(position.x, position.y, position.z,200.0);
	
	for (auto &description : descriptions)
	{
		if (!GraphicObjects[description.ObjectID].itInit())
		{
			GraphicObjects[description.ObjectID] = CreateGraphicObject(description.Model);
			GraphicObjects[description.ObjectID].setID(description.ObjectID);
			GraphicObjects[description.ObjectID].setPosition(vec3(description.x, description.y, description.z));
			GraphicObjects[description.ObjectID].setRotation(-description.Yaw);
			GraphicObjects[description.ObjectID].setInit();
			GraphicObjects[description.ObjectID].setAABB(vec3(description.AABB[0], description.AABB[1], description.AABB[2]));
		}
		else
		{
			GraphicObjects[description.ObjectID].setPosition(vec3(description.x, description.y, description.z));
			GraphicObjects[description.ObjectID].setRotation(-description.Yaw);
		}
	}
	Objects = GraphicObjects.size();
	bool Forward = GetAsyncKeyState(VK_UP);
	bool Back = GetAsyncKeyState(VK_DOWN);
	bool Left = GetAsyncKeyState(VK_LEFT);
	bool Right = GetAsyncKeyState(VK_RIGHT);

	if (GetAsyncKeyState(0x31) & 1)
	{
		std::cout << "1" << std::endl;
		AABBOptimization = !AABBOptimization;
		AABB = AABBOptimization;
	}

	if (GetAsyncKeyState(0x32) & 1)
	{
		std::cout << "2" << std::endl;
		CRenderManager::Instance().changeAABBRenderMode();
	}

	float dForward = (int(Forward) - int(Back))*sec;
	float dRigth = (int(Right) - int(Left))*sec;

	GetCursorPos(&newPossition);
	float dHorizAngle = oldPossition.x - newPossition.x;
	float dVertAngle = oldPossition.y - newPossition.y;
	oldPossition = newPossition;

	Camera.MoveOXZ(dForward, dRigth);
	Camera.Rotate(dHorizAngle, dVertAngle);
}

CCamera * CScene::getCamera(void)
{
	return &Camera;
}

void CScene::draw()
{
	// передаем в рендер‐менеджер используемую камеру
	CRenderManager::Instance().setCamera(Camera);
	// передаем в рендер‐менеджер используемый источник света
	CRenderManager::Instance().setLight(Light);
	ObjectCount = 0;
	// передаем все модели
	if (AABBOptimization)
	{ 
		for (auto &object : GraphicObjects)
		{
			mat4 PVM = Camera.GetProjectionMatrix() * Camera.GetViewMatrix() * object.second.getModelMatrix();
			vec3 aabb = object.second.getAABB();
			std::vector<glm::vec4> aabb_vertices = {
				glm::vec4(+aabb.x, +aabb.y, +aabb.z, 1.0),
				glm::vec4(+aabb.x, +aabb.y, -aabb.z, 1.0),
				glm::vec4(+aabb.x, -aabb.y, +aabb.z, 1.0),
				glm::vec4(+aabb.x, -aabb.y, -aabb.z, 1.0),
				glm::vec4(-aabb.x, +aabb.y, +aabb.z, 1.0),
				glm::vec4(-aabb.x, +aabb.y, -aabb.z, 1.0),
				glm::vec4(-aabb.x, -aabb.y, +aabb.z, 1.0),
				glm::vec4(-aabb.x, -aabb.y, -aabb.z, 1.0)
			};
			bool itVisible = false;
			for (auto &aabb_vertex : aabb_vertices)
			{
				aabb_vertex = PVM * aabb_vertex;
				aabb_vertex /= aabb_vertex.w;
				if (((aabb_vertex.x < 1) && (aabb_vertex.x > -1)) &&
					((aabb_vertex.y < 1) && (aabb_vertex.y > -1)) &&
					((aabb_vertex.z < 1) && (aabb_vertex.z > -1)))
				{
					itVisible = true;
					break;
				}
			}
			if (itVisible)
			{
				CRenderManager::Instance().addToRenderQueue(object.second);
			}
		}
	}
	else
	{
		for (auto &object : GraphicObjects)
		{
			CRenderManager::Instance().addToRenderQueue(object.second);
		}
	}
	ObjectCount = CRenderManager::Instance().getObjectCount();
}

CGraphicObject CScene::CreateGraphicObject(std::string name)
{
	CGraphicObject tempGraphicObject;
	CMaterial tempMaterial;
	float temp[3];
	

	pugi::xml_node resources = resources_description.child("Resources");
	pugi::xml_node models = resources.child("Models");
	pugi::xml_node  model = models.find_child_by_attribute("id", name.c_str());

	pugi::xml_attribute mesh = model.child("Mesh").attribute("path");
	char mesh_name[128];
	sprintf_s(mesh_name, "%s", mesh.value());
	tempGraphicObject.setMesh(CResourceManager::Instance().LoadMesh(mesh_name));

	pugi::xml_node material = model.child("Material");

	pugi::xml_attribute texture = material.child("Texture").attribute("path");
	char texture_name[128];
	sprintf_s(texture_name, "%s", texture.value());
	tempGraphicObject.setTexture(CResourceManager::Instance().LoadTexture(texture_name));

	pugi::xml_node phong = material.child("PhongParameters");

	pugi::xml_attribute diffuse = phong.attribute("diffuse");
	sscanf_s(diffuse.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec4 vec = vec4(temp[0], temp[1], temp[2], 0);
	tempMaterial.setDiffuse(vec);

	pugi::xml_attribute ambient = phong.attribute("ambient");
	sscanf_s(ambient.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec = vec4(temp[0], temp[1], temp[2], 0);
	tempMaterial.setAmbient(vec);

	pugi::xml_attribute specular = phong.attribute("specular");
	sscanf_s(specular.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec = vec4(temp[0], temp[1], temp[2], 0);
	tempMaterial.setSpecular(vec);

	pugi::xml_attribute shininess = phong.attribute("shininess");
	sscanf_s(shininess.value(), "%f", &temp[0]);
	tempMaterial.setShininess(temp[0]);

	tempGraphicObject.setMaterial(tempMaterial);
	return tempGraphicObject;
}

CLight CScene::CreateLight()
{
	CLight tempLight;

	float temp[3];

	pugi::xml_node resources = resources_description.child("Resources");
	pugi::xml_node light = resources.child("Light");
	
	pugi::xml_attribute direction = light.child("Direction").attribute("vector");
	sscanf_s(direction.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec4 vec = vec4(temp[0], temp[1], temp[2], 0);
	tempLight.setPosition(vec);

	pugi::xml_node phong = light.child("PhongParameters");

	pugi::xml_attribute diffuse = phong.attribute("diffuse");
	sscanf_s(diffuse.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec = vec4(temp[0], temp[1], temp[2], 0);
	tempLight.setDiffuse(vec);

	pugi::xml_attribute ambient = phong.attribute("ambient");
	sscanf_s(ambient.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec = vec4(temp[0], temp[1], temp[2], 0);
	tempLight.setAmbient(vec);

	pugi::xml_attribute specular = phong.attribute("specular");
	sscanf_s(specular.value(), "%f %f %f", &temp[0], &temp[1], &temp[2]);
	vec = vec4(temp[0], temp[1], temp[2], 0);
	tempLight.setSpecular(vec);

	return tempLight;
}
