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
		// матрица проекции
	mat4 ProjectionMatrix;
	// матрица наблюдения
	mat4 ViewMatrix;
public:
	// конструктор по умолчанию
	CCamera(void);
	// деструктор
	~CCamera(void);
	// установка матрицы проекции
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	// получение матрицы проекции
	mat4 GetProjectionMatrix(void);
	// получить матрицу камеры (наблюдения)
	mat4 GetViewMatrix(void);
	// передвинуть камеру и точку наблюдения в горизонтальной плоскости oXZ
	void MoveOXZ(float dForward, float dRight);
	// вращение в горизонтальной и вертикальной плоскости
	void Rotate(float dHorizAngle, float dVertAngle);
	// приближение/удаление
	void Zoom(float dR);
};
