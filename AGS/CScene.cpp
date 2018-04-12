#include "CScene.h"

void CScene::init(void)
{
	pugi::xml_parse_result result = resources_description.load_file("Resources.xml");

	Light = CreateLight();

	// временная локальная переменная
	CGraphicObject TempGraphicObject;
	// первый объект
	TempGraphicObject = CreateGraphicObject("house_1_bl");
	TempGraphicObject.setPosition(vec3(0, 0, 0));
	TempGraphicObject.setRotation(0);
	TempGraphicObject.setID(GraphicObjects.size());
	GraphicObjects.push_back(TempGraphicObject);
	// второй объект
	TempGraphicObject = CreateGraphicObject("light");
	TempGraphicObject.setPosition(vec3(-6.5, -0.55, 3));
	TempGraphicObject.setRotation(-90);
	TempGraphicObject.setID(GraphicObjects.size());
	GraphicObjects.push_back(TempGraphicObject);
	// третий объект
	TempGraphicObject = CreateGraphicObject("light");
	TempGraphicObject.setPosition(vec3(+6.5, -0.55, 3));
	TempGraphicObject.setRotation(-90);
	TempGraphicObject.setID(GraphicObjects.size());
	GraphicObjects.push_back(TempGraphicObject);
	// четвертый объект
	TempGraphicObject = CreateGraphicObject("ambul");
	TempGraphicObject.setPosition(vec3(+2.5, -1.7, 5.2));
	TempGraphicObject.setRotation(0);
	TempGraphicObject.setID(GraphicObjects.size());
	GraphicObjects.push_back(TempGraphicObject);

	TempGraphicObject = CreateGraphicObject("ambul");
	TempGraphicObject.setPosition(vec3(+5.5, -1.7, 12.2));
	TempGraphicObject.setRotation(0);
	TempGraphicObject.setID(GraphicObjects.size());
	GraphicObjects.push_back(TempGraphicObject);
}

void CScene::simulate(float sec)
{
	
	bool Forward = GetAsyncKeyState(VK_UP);
	bool Back = GetAsyncKeyState(VK_DOWN);
	bool Left = GetAsyncKeyState(VK_LEFT);
	bool Right = GetAsyncKeyState(VK_RIGHT);

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
	// передаем все модели
	for (auto it = GraphicObjects.begin(); it < GraphicObjects.end(); it++) {
		CRenderManager::Instance().addToRenderQueue(*it);
	}
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
