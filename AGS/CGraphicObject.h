#pragma once
#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CMaterial.h"

using namespace glm;

class CGraphicObject
{
private:
	// уникальный идентификатор объекта
	int ID;
	// используемый меш (индекс в менеджере ресурсов)
	int meshID;
	int textureID;
	// используемый материал
	CMaterial material;
	// матрица модели (задает позицию и ориентацию)
	mat4 modelMatrix;
public:
	// конструктор по умолчанию
	CGraphicObject(void) {};
	// установка внутренних полей
	void setID(int ID);
	void setMesh(int meshID);
	void setTexture(int textureID);
	void setMaterial(CMaterial& material);
	void setPosition(vec3 position);
	void setRotation(float degree);
	// получение различных параметров
	int getMesh(void);
	int getTexture(void);
	int getID(void);
	CMaterial& getMaterial(void);
	mat4 getModelMatrix(void);
	// оператор сравнения
	bool operator==(CGraphicObject &a);
	bool operator!=(CGraphicObject &a);
};

