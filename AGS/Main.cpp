
#include "Data.h"
#include "CRenderManager.h"
#include "CTexture.h"
using namespace glm;

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void)
{
	// начинаем вывод нового кадра
	// в том числе очищаем список объектов для вывода, переданный в прошлом кадре
	CRenderManager::Instance().start();
	// выводим всю сцену на экран
	// т.е. передаем используемые камеру, источник света и все объекты
	Scene.draw();
	// завершаем вывод всей сцены
	// выводятся все ранее переданные объекты с учетом переданных настроек,
	// таких как: камера и источник света
	CRenderManager::Instance().finish();
	// смена переднего и заднего буферов
	FPScount++;
	seconds += simulationTimePassed;
	if (seconds > 1.0)
	{
		FPS = FPScount;
		FPScount = 0;
		seconds = 0;
		glutSetWindowTitle(currentState);
	}
	sprintf_s(currentState, 100, "FPS : [%d], UBO update: [%d]", FPS,CRenderManager::UBOUpdateCount);
	glutSwapBuffers();
};

// функция, вызываемая при изменении ра5змеров окна
void Reshape(int w, int h)
{
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Scene.getCamera()->SetProjectionMatrix(radians(35.0), (float)w / h, 1, 100);
};

void MouseWheel(int wheel, int direction, int x, int y)
{
	Scene.getCamera()->Zoom(direction);
}


// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	QueryPerformanceCounter(&newValue);
	QueryPerformanceFrequency(&frequency);
	simulationTimePassed = (double)(newValue.QuadPart - oldValue.QuadPart) / frequency.QuadPart;
	oldValue = newValue;
	Scene.simulate(simulationTimePassed);
	glutPostRedisplay();
};


void Close()
{
	Scene.getCamera()->Saving();
}


int main(int argc, char **argv)
{
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 3.3 без поддержки обратной совместимости)
	glutInitContextVersion(4, 2);
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
	ilInit();
	iluInit();
	ilutInit();
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));
	
	CRenderManager::Instance().init();
	Scene.init();
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