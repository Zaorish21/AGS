#pragma once



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

// КЛАСС ДЛЯ РАБОТЫ С ШЕЙДЕРОМ
class CShader
{
private:

	// вершинный шейдер
	GLuint	Vsh;
	// фрагментный шейдер
	GLuint	Fsh;
	// шейдерная программа (шейдер)
	GLuint	Program;

	std::map<std::string, int> UniformVariables;

public:
	int LoadShader(char *VertexFileName, int type, bool DebugOutput = true);
	// загрузить вершинный шейдер
	int LoadVertexShader(char *VertexFileName, bool DebugOutput = true);
	// загрузить фрагментный шейдер
	int LoadFragmentShader(char *FragmentFileName, bool DebugOutput = true);
	// слинковать шейдерную программу
	int Link(bool DebugOutput = true);

	//	активизировать шейдер (сделать текущим)
	void Activate(void);
	//	отключить шейдер (использовать нулевую шейдерную программу)
	static void	Deactivate(void);

	// получение индекса атрибут-переменной
	int GetAttribLocation(char *name)
	{
		return glGetAttribLocation(Program, name);
	};

	// запись вектора из 4-х комопнент в uniform-переменную
	void SetUniform(std::string name, GLfloat &value);
	void SetUniform(std::string name, int value);
	void SetUniform(std::string name, vec4 &value);
	void SetUniform(std::string name, mat4 &value);
};