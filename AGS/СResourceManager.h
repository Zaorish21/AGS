#pragma once
#pragma once
#include <windows.h>
#include "stdio.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "CMesh.h"
#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CResourceManager
{
private:
	// вектор для хранения всех мешей
	std::vector<CMesh*> meshes;
	// map для хранения соответствия между именем запрашиваемого файла
	// и индексом в контейнере meshes
	std::map <std::string, int> meshes_index;
private:
	// конструктор по умолчанию (объявлен приватным)
	// в результате нельзя создать ни одного объекта данного класса вне самого класса
	CResourceManager() {};
	// конструктора копирования нет
	CResourceManager(const CResourceManager& root) = delete;
	// оператора присваивания нет
	CResourceManager& operator=(const CResourceManager&) = delete;
public:
	// Статик‐метод для получения экземпляра менеджера ресурса.
	// Всегда будет возвращена ссылка на статичный объект,
	// хранящийся в единственном экземпляре.
	static CResourceManager& Instance()
	{
		static CResourceManager ResourceManager;
		return ResourceManager;
	}
	// Загрузка одного меша.
	// Возвращаемое значение: индекс меша в менеджере ресурсов
	// Осуществляется проверка на попытку загрузки ранее загруженного меша
	int LoadMesh(std::string filename);
	// Получение меша по его индексу.
	// Если такого меша нет (не действительный индекс) возвращается nullptr
	CMesh* GetMesh(int index);
};

