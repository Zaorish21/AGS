#include <cstdio>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"
#include "CCamera.h"
using namespace glm;

// используемый шейдер (пока только один)
CShader		Shader;
CCamera		Camera;

LARGE_INTEGER oldValue, newValue, frequency;
double simulationTimePassed;

void DrawCube(CShader &shader) {
	// переменные для вывода объекта (прямоугольника из двух треугольников)
	static GLuint VAO_Index = 0; // индекс VAO-буфера
	static GLuint VBO_Index = 0; // индекс VBO-буфера
	static int VertexCount = 0; // количество вершин
	static bool init = true;
	if (init) {
		// создание и заполнение VBO
		glGenBuffers(1, &VBO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		GLfloat Verteces[] = {
			// передняя грань
			-0.5, +0.5, +0.5,
			-0.5, -0.5, +0.5,
			+0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			-0.5, -0.5, +0.5,
			+0.5, -0.5, +0.5,
			// задняя грань
			+0.5, +0.5, -0.5,
			+0.5, -0.5, -0.5,
			-0.5, +0.5, -0.5,
			-0.5, +0.5, -0.5,
			+0.5, -0.5, -0.5,
			-0.5, -0.5, -0.5,
			// правая грань
			+0.5, -0.5, +0.5,
			+0.5, -0.5, -0.5,
			+0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			+0.5, -0.5, -0.5,
			+0.5, +0.5, -0.5,
			// левая грань
			-0.5, +0.5, +0.5,
			-0.5, +0.5, -0.5,
			-0.5, -0.5, +0.5,
			-0.5, -0.5, +0.5,
			-0.5, +0.5, -0.5,
			-0.5, -0.5, -0.5,
			// верхняя грань
			-0.5, +0.5, -0.5,
			-0.5, +0.5, +0.5,
			+0.5, +0.5, -0.5,
			+0.5, +0.5, -0.5,
			-0.5, +0.5, +0.5,
			+0.5, +0.5, +0.5,
			// нижняя грань
			-0.5, -0.5, +0.5,
			-0.5, -0.5, -0.5,
			+0.5, -0.5, +0.5,
			+0.5, -0.5, +0.5,
			-0.5, -0.5, -0.5,
			+0.5, -0.5, -0.5
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(Verteces), Verteces, GL_STATIC_DRAW);
		// создание VAO
		glGenVertexArrays(1, &VAO_Index);
		glBindVertexArray(VAO_Index);
		// заполнение VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		int k = shader.GetAttribLocation((char*)"vPosition");
		glVertexAttribPointer(k, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(k);
		// "отвязка" буфера VAO, чтоб случайно не испортить
		glBindVertexArray(0);
		// указание количество вершин
		VertexCount = 6 * 6;
		init = false;
	}
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

void DrawCubeIn(vec4 Position, vec4 Color, mat4 ViewMatrix)
{
	mat4 ModelMatrix = mat4(
		vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		Position);

	mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
	Shader.SetUniform("ModelViewMatrix", ModelViewMatrix);

	Shader.SetUniform("Color", Color);

	DrawCube(Shader);
}

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void) {
	// отчищаем буфер цвета
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// активируем шейдер
	Shader.Activate();

	// получаем матрицу проекции
	mat4 ProjectionMatrix;
	ProjectionMatrix = Camera.GetProjectionMatrix();
	// устанавливаем матрицу проекции
	Shader.SetUniform("ProjectionMatrix", ProjectionMatrix);

	// получаем матрицу наблюдения
	mat4 ViewMatrix = Camera.GetViewMatrix();

	vec4 Color = vec4(1.0, 0.0, 1.0, 1.0);
	vec4 Pos = vec4(0.0, 0.0, 0.0, 1.0);
	DrawCubeIn(Pos, Color, ViewMatrix);

	Color = vec4(1.0, 0.0, 0.0, 1.0);
	Pos = vec4(3.0, 0.0, 0.0, 1.0);
	DrawCubeIn(Pos, Color, ViewMatrix);

	Color = vec4(1.0, 0.7, 0.0, 1.0);
	Pos = vec4(0.0, -3.0, 0.0, 1.0);
	DrawCubeIn(Pos, Color, ViewMatrix);

	Color = vec4(0.0, 0.0, 0.0, 1.0);
	Pos = vec4(3.0, 2.5, 3.0, 1.0);
	DrawCubeIn(Pos, Color, ViewMatrix);

	Color = vec4(0.0, 1.0, 1.0, 1.0);
	Pos = vec4(3.0, 0, 3.0, 1.0);
	DrawCubeIn(Pos, Color, ViewMatrix);

	glutSwapBuffers();
};

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Camera.SetProjectionMatrix(35.0, (float)w / h, 1, 100);
};

// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	QueryPerformanceCounter(&newValue);
	QueryPerformanceFrequency(&frequency);
	simulationTimePassed = (double)(newValue.QuadPart - oldValue.QuadPart) / frequency.QuadPart;
	oldValue = newValue;

	//	ПЕРЕРИСОВАТЬ ОКНО
	glutPostRedisplay();
};

int main(int argc, char **argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(4, 6);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300, 100);
	// устанавливаем размер окна
	glutInitWindowSize(800, 600);
	// создание окна
	glutCreateWindow("laba_01");

	//	инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return 0;
	}
	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));

	// загрузка шейдера
	Shader.LoadVertexShader((char*)"SHADER\\Example.vsh");
	Shader.LoadFragmentShader((char*)"SHADER\\Example.fsh");
	Shader.Link();
	// инициализация объекта для вывода

	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(Display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(Reshape);
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(Simulation);
	// основной цикл обработки сообщений ОС
	glutMainLoop();
	return 0;
};