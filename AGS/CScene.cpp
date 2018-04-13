#include "CScene.h"

void CScene::init(void)
{
	pugi::xml_parse_result result = resources_description.load_file("Resources.xml");

	Light = CreateLight();

	// временная локальная переменная
	CGraphicObject TempGraphicObject;
	// первый объект
	WSADATA wsa;
	
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d\n", WSAGetLastError());
	}
	else {
		printf("OK\n");
	}
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Could not create socket : %d\n", WSAGetLastError());
	}
	else {
		printf("Socket created.\n");
	}
	
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(27000);
	// подключение к серверу
	int connect_error = connect(s, (struct sockaddr *)&server, sizeof(server));
	if (connect_error == SOCKET_ERROR) {
		printf("Connection failed : %d\n", WSAGetLastError());
	}
	else {
		printf("Connection is established.\n");
	};

	// запрос приветственного сообщения (код запроса ‐ 0)
	SNetworkHader header;
	header.transactionID = transID; // номер транзакции
	header.frameNumber = 1; // первый пакет ...
	header.frameCount = 1; // ... из одного
	header.dataLen = 1; // отправляется один байт данных (код запроса)
	header.funcID = 0; // код запроса = 0 (запрос приветственного сообщения)
	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf("Data was not send: %d.\n", WSAGetLastError());
	}
	// считывание приветственного сообщения с сервера (код запроса ‐ 0)
	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);
	if (receive_data_count <= 0) {
		printf("Data was not received: %d.\n", WSAGetLastError());
	}
	else {
		char *str = new char[header.dataLen - 1];
		recv(s, str, header.dataLen - 1, 0);
		printf("%s\n", str);
		delete[] str;
	};

}

void CScene::simulate(float sec)
{
	SNetworkHader header;
	header.transactionID = ++transID; 
	header.frameNumber = 1; 
	header.frameCount = 1; 
	header.dataLen = 1; 
	header.funcID = 1; 
	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf("Data was not send: %d.\n", WSAGetLastError());
	}

	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);
	if (receive_data_count <= 0) {
		printf("Data was not received: %d.\n", WSAGetLastError());
	}
	else {
		int count;
		recv(s, reinterpret_cast<char*>(&count), sizeof(int), 0);
		SGameObjectDescription Description;
		CGraphicObject tempGraphicObject;
		for (int i = 0; i < count; i++)
		{
			recv(s, reinterpret_cast<char*>(&Description), sizeof(Description), 0);
			tempGraphicObject = CreateGraphicObject(Description.Model);
			tempGraphicObject.setID(Description.ObjectID);
			tempGraphicObject.setPosition(vec3(Description.x, Description.y, Description.z));
			tempGraphicObject.setRotation(-Description.Yaw);
			
			GraphicObjects[Description.ObjectID] = tempGraphicObject;
		}
	};

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
	for (auto it = GraphicObjects.begin(); it != GraphicObjects.end(); it++) {
		CRenderManager::Instance().addToRenderQueue(it->second);
	};

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
