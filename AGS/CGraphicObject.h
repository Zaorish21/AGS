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
	bool init;
	// ������������ ��������
	CMaterial material;
	// ������� ������ (������ ������� � ����������)
	mat4 modelMatrix;
	vec3 AABB;
public:
	// ����������� �� ���������
	CGraphicObject(void) { init = false; };
	// ��������� ���������� �����
	void setID(int ID);
	void setMesh(int meshID);
	void setInit() { init = true; };
	void setTexture(int textureID);
	void setMaterial(CMaterial& material);
	void setPosition(vec3 position);
	void setRotation(float degree);
	void setAABB(vec3 AABB);

	int getMesh(void);
	int getTexture(void);
	int getID(void);
	CMaterial& getMaterial(void);
	mat4 getModelMatrix(void);
	vec3 getAABB();

	bool itInit() { return init; };
	
	bool operator==(CGraphicObject &a);
	bool operator!=(CGraphicObject &a);
};

