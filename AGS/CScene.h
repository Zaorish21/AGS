#pragma once
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>
#include <vector>

#include "pugixml.hpp"

#include "CShader.h"
#include "CCamera.h"
#include "CLight.h"
#include "CGraphicObject.h"
#include "СResourceManager.h"
#include "CRenderManager.h"

#pragma pack(push,1)
struct SNetworkHader {
	// номер транзакции
	unsigned short transactionID;
	// номер пакета среди группы пакетов одного запроса/ответа
	unsigned short frameNumber;
	// всего пакетов в группе пакетов одного запроса/ответа
	unsigned short frameCount;
	// длина оставшейся части данных текущего пакета (включая funcID)
	unsigned int dataLen;
	// код запроса/ответа в соответствии с протоколом
	unsigned char funcID;
};

#pragma pack(push,1)
struct SGameObjectDescription {
	// идентификатор объекта
	unsigned int ObjectID;
	// тип объекта
	unsigned char ObjectType;
	// позиция объекта (x, y, z)
	float x, y, z;
	// угол поворота в градусах по часовой стрелке вокруг оси oY
	float Yaw;
	// имя модели (в файле Resources.xml)
	char Model[16];
	// зарезервированные поля
	char Reserved[59];
};

class CScene
{
private:
	// используемый шейдер (пока только один)
	CShader Shader;
	// камера
	CCamera Camera;
	// источник света
	CLight Light;
	// все графические объекты
	std::map<int, CGraphicObject> GraphicObjects;
	// xml‐документ в котором приводится описание всех объектов
	// чтобы не загружать xml‐файл каждый раз, он загружается на этапе инициализации
	pugi::xml_document resources_description;
	POINT newPossition, oldPossition;
	SOCKET s;
	int transID = 0;
public:
	// Инициализация сцены
	void init(void);
	// Симуляция сцены
	void simulate(float sec);
	// Вывод всей сцены
	void draw(void);
	// Получение камеры для модификации её в некоторых функциях обратного вызова
	// например: reshape или mouseWheel
	CCamera* getCamera(void);
private:
	// создание объекта, используя описание модели из файла "Resources.xml"
	CGraphicObject CreateGraphicObject(std::string name);
	CLight CreateLight();
};