#pragma once
#include <windows.h>
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Face {
	int Indeces[3][3];
};

struct Vertex {
	// геометрические координаты вершины
	GLfloat vPosition[3];
	// нормали к поверхности в данной точке
	GLfloat vNormal[3];
	// текстурные координаты вершины
	GLfloat vTexCoord[2];
};

class CMesh
{
private:
	GLuint VAO_Index; // индекс VAO‐буфера
	GLuint VBO_Index; // индекс VBO‐буфера
	int VertexCount; // количество вершин
public:
	// конструктор
	CMesh(void) {};
	// загрузка меша из внешнего obj‐файла
	int Load(const char* filename);
	// вывод модели (используется текущий шейдер)
	void Render(void);
};