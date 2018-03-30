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
	// используемый меш (индекс в менеджере ресурсов)
	int meshID;
	// используемый материал
	CMaterial material;
	// матрица модели (задает позицию и ориентацию)
	mat4 modelMatrix;
public:
	// конструктор по умолчанию
	CGraphicObject(void) {};
	// установка внутренних полей
	void setMesh(int meshID);
	void setMaterial(CMaterial& material);
	void setPosition(vec3 position);
	void setRotation(float degree);
	// получение различных параметров
	int getMesh(void);
	CMaterial& getMaterial(void);
	mat4 getModelMatrix(void);
};

