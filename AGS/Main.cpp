#include <cstdio>

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CShader.h"
using namespace glm;

// используемый шейдер (пока только один)
CShader		Shader;




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
	ProjectionMatrix = perspective(radians(35.0), 800.0 / 600.0, 0.1, 200.0);
	// устанавливаем матрицу проекции
	Shader.SetUniform("ProjectionMatrix", ProjectionMatrix);
	// получаем матрицу наблюдения
	mat4 ViewMatrix;
	// позиция камеры (0, 5, 10)
	vec3 Eye = vec3(0.0, 5.0, 10.0);
	// точка, в которую направлена камера ‐ (0, 0, 0);
	vec3 Center = vec3(0, 0, 0);
	// примерный вектор "вверх" (0, 1, 0)
	vec3 Up = vec3(0, 1.0, 0);
	// матрица камеры
	ViewMatrix = lookAt(Eye, Center, Up);
	// ВЫВОДИМ ПЕРВУЮ МОДЕЛЬ:
	// формируем матрицу модели - модель располагается в точке (3,0,0) без поворота
	mat4 ModelMatrix1 = mat4(
		vec4(1, 0, 0, 0), // 1-ый столбец: направление оси ox
		vec4(0, 1, 0, 0), // 2-ой столбец: направление оси oy
		vec4(0, 0, 1, 0), // 3-ий столбец: направление оси oz
		vec4(3, 0, 0, 50)); // 4-ый столбец: позиция объекта (начала координат)
						   // устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix1 = ViewMatrix * ModelMatrix1;
	Shader.SetUniform("ModelViewMatrix", ModelViewMatrix1);
	// устанавливаем uniform-переменную отвечающую за цвет объекта (фрагментов)
	vec4 Color1 = vec4(1.0, 0.0, 0.0, 1.0);
	Shader.SetUniform("Color", Color1);
	// вывод объекта
	DrawCube(Shader);
	glutSwapBuffers();
};

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
};

// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
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