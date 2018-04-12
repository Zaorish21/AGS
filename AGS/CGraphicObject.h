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
	// ���������� ������������� �������
	int ID;
	// ������������ ��� (������ � ��������� ��������)
	int meshID;
	int textureID;
	// ������������ ��������
	CMaterial material;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
public:
	// ����������� �� ���������
	CGraphicObject(void) {};
	// ��������� ���������� �����
	void setID(int ID);
	void setMesh(int meshID);
	void setTexture(int textureID);
	void setMaterial(CMaterial& material);
	void setPosition(vec3 position);
	void setRotation(float degree);
	// ��������� ��������� ����������
	int getMesh(void);
	int getTexture(void);
	int getID(void);
	CMaterial& getMaterial(void);
	mat4 getModelMatrix(void);
	// �������� ���������
	bool operator==(CGraphicObject &a);
	bool operator!=(CGraphicObject &a);
};

