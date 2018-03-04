#pragma once

#include <windows.h>
#include "stdio.h"

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

// ����� ��� ������ � ��������
class CShader
{
private:

	// ��������� ������
	GLuint	Vsh;
	// ����������� ������
	GLuint	Fsh;
	// ��������� ��������� (������)
	GLuint	Program;

	std::map<std::string, int> UniformVariables;

public:
	int LoadShader(char *VertexFileName, int type, bool DebugOutput = true);
	// ��������� ��������� ������
	int LoadVertexShader(char *VertexFileName, bool DebugOutput = true);
	// ��������� ����������� ������
	int LoadFragmentShader(char *FragmentFileName, bool DebugOutput = true);
	// ���������� ��������� ���������
	int Link(bool DebugOutput = true);

	//	�������������� ������ (������� �������)
	void Activate(void);
	//	��������� ������ (������������ ������� ��������� ���������)
	static void	Deactivate(void);

	// ��������� ������� �������-����������
	int GetAttribLocation(char *name)
	{
		return glGetAttribLocation(Program, name);
	};

	// ������ ������� �� 4-� ��������� � uniform-����������
	void SetUniform(std::string name, vec4 &value);
	void SetUniform(std::string name, mat4 &value);
};