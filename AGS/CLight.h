#pragma once

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class CLight
{
	// различные составляющие источника света
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
public:
	// конструктор по умолчанию
	CLight(void) {};
	// задание различных параметров источника света
	void setPosition(vec4 &position);
	void setAmbient(vec4 &ambient);
	void setDiffuse(vec4 &diffuse);
	void setSpecular(vec4 &specular);
	// получение различных параметров источника света
	vec4 &getPosition();
	vec4 &getAmbient();
	vec4 &getDiffuse();
	vec4 &getSpecular();

	bool operator==(CLight &a);
	bool operator!=(CLight &a);

};