#include "Data.h"
#include "Simulation.h"

using namespace glm;



// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void)
{
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
	mat4 ProjectionMatrix = Camera.GetProjectionMatrix();
	// устанавливаем матрицу проекции
	Shader.SetUniform("ProjectionMatrix", ProjectionMatrix);
	// получаем матрицу наблюдения
	mat4 ViewMatrix = Camera.GetViewMatrix();
	// ВЫВОДИМ ПЕРВУЮ МОДЕЛЬ:
	// формируем матрицу модели (единичная матрица)
	// модель располагается в точке (3,0,0) без поворота
	mat4 ModelMatrix1 = mat4(
		vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		vec4(0, 0, 0, 1)); // 4‐ый столбец: позиция объекта (начала координат)
						   // устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix1 = ViewMatrix * ModelMatrix1;
	Shader.SetUniform("ModelViewMatrix", ModelViewMatrix1);
	// устанавливаем uniform‐переменную отвечающую за цвет объекта(фрагментов)
	vec4 Color1 = vec4(1.0, 0.0, 0.0, 1.0);
	Shader.SetUniform("Color", Color1);
	// вывод объекта
	CMesh* mesh = CResourceManager::Instance().GetMesh(MeshId[0]);
	if (mesh != nullptr) mesh->Render();
	// ВЫВОДИМ ВТОРУЮ МОДЕЛЬ:
	// формируем матрицу модели (единичная матрица)
	// модель располагается в точке (3,0,0) без поворота
	mat4 ModelMatrix2 = mat4(
		vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		vec4(10, 0, 0, 1)); // 4‐ый столбец: позиция объекта (начала координат)
							// устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix2 = ViewMatrix * ModelMatrix2;
	Shader.SetUniform("ModelViewMatrix", ModelViewMatrix2);
	// устанавливаем uniform‐переменную отвечающую за цвет объекта(фрагментов)
	vec4 Color2 = vec4(0.0, 0.0, 1.0, 1.0);
	Shader.SetUniform("Color", Color2);
	// вывод объекта
	mesh = CResourceManager::Instance().GetMesh(MeshId[1]);
	if (mesh != nullptr) mesh->Render();
	// ВЫВОДИМ ТРЕТЬЮ МОДЕЛЬ:
	// формируем матрицу модели (единичная матрица)
	// модель располагается в точке (3,0,0) без поворота
	mat4 ModelMatrix3 = mat4(
		vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		vec4(-10, -0.8, 0, 1)); // 4‐ый столбец: позиция объекта (начала координат)
								// устанавливаем матрицу наблюдения модели
	mat4 ModelViewMatrix3 = ViewMatrix * ModelMatrix3;
	Shader.SetUniform("ModelViewMatrix", ModelViewMatrix3);
	// устанавливаем uniform‐переменную отвечающую за цвет объекта(фрагментов)
	vec4 Color3 = vec4(0.0, 0.0, 0.0, 1.0);
	Shader.SetUniform("Color", Color3);
	// вывод объекта
	mesh = CResourceManager::Instance().GetMesh(MeshId[2]);
	if (mesh != nullptr) mesh->Render();
	// смена переднего и заднего буферов
	glutSwapBuffers();
};

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Camera.SetProjectionMatrix(radians(35.0), (float)w / h, 1, 100);
};

void MouseWheel(int wheel, int direction, int x, int y)
{
	Camera.Zoom(direction);
}


// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	QueryPerformanceCounter(&newValue);
	QueryPerformanceFrequency(&frequency);
	simulationTimePassed = (double)(newValue.QuadPart - oldValue.QuadPart) / frequency.QuadPart;
	oldValue = newValue;
	CameraSimulation(simulationTimePassed);
	
	glutPostRedisplay();
};


void Close()
{
	Camera.Saving();
}


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
	ShaderInit();
	// инициализация объекта для вывода
	MeshesInit();
	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(Display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(Reshape);
	glutMouseWheelFunc(MouseWheel);
	
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(Simulation);
	// основной цикл обработки сообщений ОС
	glutCloseFunc(Close);
	glutMainLoop();
	
	return 0;
};