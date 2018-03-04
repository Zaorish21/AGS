#pragma once

#include <windows.h>
#include "stdio.h"

#include <iostream>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
class CCamera
{
		// ������� ��������
	mat4 ProjectionMatrix;
	// ������� ����������
	mat4 ViewMatrix;
public:
	// ����������� �� ���������
	CCamera(void);
	// ����������
	~CCamera(void);
	// ��������� ������� ��������
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	// ��������� ������� ��������
	mat4 GetProjectionMatrix(void);
	// �������� ������� ������ (����������)
	mat4 GetViewMatrix(void);
	// ����������� ������ � ����� ���������� � �������������� ��������� oXZ
	void MoveOXZ(float dForward, float dRight);
	// �������� � �������������� � ������������ ���������
	void Rotate(float dHorizAngle, float dVertAngle);
	// �����������/��������
	void Zoom(float dR);
};
