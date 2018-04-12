#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class CLight
{
	// ��������� ������������ ��������� �����
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
public:
	// ����������� �� ���������
	CLight(void) {};
	// ������� ��������� ���������� ��������� �����
	void setPosition(vec4 &position);
	void setAmbient(vec4 &ambient);
	void setDiffuse(vec4 &diffuse);
	void setSpecular(vec4 &specular);
	// ��������� ��������� ���������� ��������� �����
	vec4 &getPosition();
	vec4 &getAmbient();
	vec4 &getDiffuse();
	vec4 &getSpecular();

	bool operator==(CLight &a);
	bool operator!=(CLight &a);

};