#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class CMaterial
{
protected:
	// ��������� ������������ ���������
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	GLfloat shininess;
public:
	// ����������� �� ���������
	CMaterial(void) {};
	// ������� ��������� ���������� ��������� �����
	void setAmbient(vec4 &ambient);
	void setDiffuse(vec4 &diffuse);
	void setSpecular(vec4 &specular);
	void setShininess(float shininess);
	// ��������� ��������� ���������� ��������� �����
	vec4 &getAmbient();
	vec4 &getDiffuse();
	vec4 &getSpecular();
	float &getShininess();

	bool operator==(CMaterial &a);
	bool operator!=(CMaterial &a);
};