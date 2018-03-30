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
	// ������������ ��� (������ � ��������� ��������)
	int meshID;
	// ������������ ��������
	CMaterial material;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
public:
	// ����������� �� ���������
	CGraphicObject(void) {};
	// ��������� ���������� �����
	void setMesh(int meshID);
	void setMaterial(CMaterial& material);
	void setPosition(vec3 position);
	void setRotation(float degree);
	// ��������� ��������� ����������
	int getMesh(void);
	CMaterial& getMaterial(void);
	mat4 getModelMatrix(void);
};

