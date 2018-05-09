#pragma once
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>
#include <vector>
#include <iostream>
#include "pugixml.hpp"

#include "CShader.h"
#include "CCamera.h"
#include "CLight.h"
#include "CGraphicObject.h"
#include "СResourceManager.h"
#include "CRenderManager.h"

#include "Data.h"
#include "CNetProtocol.h"

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
	CNetProtocol NetProtocol;
	bool AABBOptimization = true;
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