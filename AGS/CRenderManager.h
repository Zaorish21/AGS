#pragma once
#include "CShader.h"
#include "CCamera.h"
#include "CLight.h"
#include "CGraphicObject.h"
#include "CMesh.h"
#include "СResourceManager.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

struct SRenderManagerObjectState {
	// предыдущее состояние объекта (в том числе его цвет и матрица модели)
	CGraphicObject GraphicObject;
	// признак необходимости обновить буфер UBO потому, что обновились глобальные данные,
	// такие как, например, матрица наблюдения (то есть камера)
	bool UBOIndexUpdate = true;
	// индекс буфера UBO
	GLuint UBOIndex;
};

struct PerSceneBlock
{
	glm::mat4 ProjectionMatrix;
	glm::vec4 lAmbient;
	glm::vec4 lDiffuse;
	glm::vec4 lSpecular;
	glm::vec4 lPosition;
};

struct PerObjectBlock
{
	glm::mat4 ModelViewMatrix;
	glm::vec4 mAmbient;
	glm::vec4 mDiffuse;
	glm::vec4 mSpecular;
};

class CRenderManager
{
private:
	// используемые шейдеры (и их идентификаторы)
	enum { DIFFUSE_TEXTURE_SHADER_ID = 0, SKYBOX_SHADER_ID, AABB_SHADER_ID, MAX_SHADER_ID };
	CShader Shaders[MAX_SHADER_ID];
	// камера
	CCamera Camera;
	// источник света
	CLight Light;
	// все графические объекты переданные для отрисовки
	vector<CGraphicObject> GraphicObjects;
	// предыдущее состояние графических объектов
	// в качестве ключа выступает идентификатор объекта
	map<int, SRenderManagerObjectState> RenderManagerObjectStates;
	// индекс буфера UBO для данных, общих для всех объектов
	GLuint PerSceneUBOIndex;
	// переменные для вывода скайбокса (индекс меша и кубической текстуры)
	unsigned int SkyBoxMeshId;
	unsigned int SkyBoxTextureId;
	// переменные для вывода ААВВ (индекс меша и режим вывода ААВВ)
	unsigned int AABBMeshId;
	bool AABBRenderEnable = false;
private:
	// конструктор по умолчанию (объявлен приватным)
	// в результате нельзя создать ни одного объекта данного класса вне самого класса
	CRenderManager() {};
	// конструктора копирования нет
	CRenderManager(const CRenderManager& root) = delete;
	// оператора присваивания нет
	CRenderManager& operator=(const CRenderManager&) = delete;
public:
	// количество обновлений буферов UBO (для статистики)
	static int UBOUpdateCount;
	// Статик‐метод для получения экземпляра менеджера ресурса.
	// Всегда будет возвращена ссылка на объект хранящийся в единственном экземпляре.
	static CRenderManager& Instance()
	{
		static CRenderManager RenderManager;
		return RenderManager;
	}
	// инициализация объекта RenderManager, выполняется после инициализации OpenGL:
	// загрузка шейдеров, нужных мешей и текстур для SkyBox’а или ААВВ
	void init(void);
	// Начало вывода очередного кадра
	void start(void);
	// установка используемой камеры
	void setCamera(CCamera& Camera);
	// установка используемого источника света
	void setLight(CLight& Light);
	// добавление в очередь рендеринга очередного объекта для вывода
	void addToRenderQueue(CGraphicObject& GraphicObject);
	// завершение вывода кадра
	void finish(void);
	// получение количества объектов, выведенных в данном кадре
	int getObjectCount(void);
	// смена режима вывода ААВВ
	void changeAABBRenderMode(void);
private:
	// вывод SkyBox'а
	void SkyBoxRender(void);
	// вывод всех объектов в нормальном режиме
	void ObjectsRender(void);
	// вывод ААВВ всех объектов
	void AABBRender(void);
	// создание буфера, общего для всех объектов сцены
	GLuint createPerSceneBlock(void);
	// создание буфера, индивидуального для одного указанного объекта
	GLuint createPerObjectBlock(CGraphicObject& GraphicObject);
	// обновление буфера, общего для всех объектов сцены
	void updatePerSceneBlock(int UBOindex);
	// обновление буфера, индивидуального для одного указанного объекта
	void updatePerObjectBlock(int UBOindex, CGraphicObject& GraphicObject);
};