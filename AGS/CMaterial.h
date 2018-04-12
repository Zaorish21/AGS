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
	// различные составляющие материала
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	GLfloat shininess;
public:
	// конструктор по умолчанию
	CMaterial(void) {};
	// задание различных параметров источника света
	void setAmbient(vec4 &ambient);
	void setDiffuse(vec4 &diffuse);
	void setSpecular(vec4 &specular);
	void setShininess(float shininess);
	// получение различных параметров источника света
	vec4 &getAmbient();
	vec4 &getDiffuse();
	vec4 &getSpecular();
	float &getShininess();

	bool operator==(CMaterial &a);
	bool operator!=(CMaterial &a);
};