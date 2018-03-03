#pragma once

#include <windows.h>
#include "stdio.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
	void SetUniformVec4(char *name, vec4 value)
	{
		int	k = glGetUniformLocation(Program, name);
		if (k < 0) return;
		glUseProgram(Program);
		glUniform4fv(k, 1, value_ptr(value));
	}
};