#include "CMesh.h"

int CMesh::Load(const char * filename)
{
	std::FILE *file;
	errno_t error = fopen_s(&file, filename, "rb");
	char f[128] = "";
	float x, y, z;
	std::vector<glm::vec3> vertexVec;
	std::vector<glm::vec2> texCoordVec;
	std::vector<glm::vec3> NormVec;
	std::vector<Face> FaceVec;
	if (file)
	{
		while (!feof(file))
		{
			fscanf_s(file, "%s", f, _countof(f));
			if (strcmp(f, "v") == 0)
			{
				float x, y, z;
				fscanf_s(file, "%f %f %f", &x, &y, &z);
				vertexVec.push_back(glm::vec3(x, y, z));
				continue;
			}
			if (strcmp(f, "vt") == 0)
			{
				float s, t;
				fscanf_s(file, "%f %f", &s, &t);
				texCoordVec.push_back(glm::vec2(s, t));
				continue;
			}
			if (strcmp(f, "vn") == 0)
			{
				float x, y, z;
				fscanf_s(file, "%f %f %f", &x, &y, &z);
				NormVec.push_back(glm::vec3(x, y, z));
				continue;
			}
			if (strcmp(f, "f") == 0)
			{
				Face face;
				fscanf_s(file, "%i/%i/%i", &face.Indeces[0][0], &face.Indeces[0][1], &face.Indeces[0][2]);
				fscanf_s(file, "%i/%i/%i", &face.Indeces[1][0], &face.Indeces[1][1], &face.Indeces[1][2]);
				fscanf_s(file, "%i/%i/%i", &face.Indeces[2][0], &face.Indeces[2][1], &face.Indeces[2][2]);
				FaceVec.push_back(face);
			}
		}
		fclose(file);
		size_t FaceCount = FaceVec.size();
		VertexCount = FaceCount * 3;
		Vertex *VertexArr = new Vertex[VertexCount];
		for (int i = 0; i < FaceCount; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				VertexArr[i * 3 + j].vPosition[0] = vertexVec[FaceVec[i].Indeces[j][0] - 1].x;
				VertexArr[i * 3 + j].vPosition[1] = vertexVec[FaceVec[i].Indeces[j][0] - 1].y;
				VertexArr[i * 3 + j].vPosition[2] = vertexVec[FaceVec[i].Indeces[j][0] - 1].z;

				VertexArr[i * 3 + j].vTexCoord[0] = texCoordVec[FaceVec[i].Indeces[j][1] - 1].x;
				VertexArr[i * 3 + j].vTexCoord[1] = texCoordVec[FaceVec[i].Indeces[j][1] - 1].y;

				VertexArr[i * 3 + j].vNormal[0] = NormVec[FaceVec[i].Indeces[j][2] - 1].x;
				VertexArr[i * 3 + j].vNormal[1] = NormVec[FaceVec[i].Indeces[j][2] - 1].y;
				VertexArr[i * 3 + j].vNormal[2] = NormVec[FaceVec[i].Indeces[j][2] - 1].z;
			}
		}
		// генерируем VBO
		glGenBuffers(1, &VBO_Index);
		// указываем, что сгенерированный буфер хранит в себе массив данных
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		// загружаем вершины
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VertexCount, VertexArr, GL_STATIC_DRAW);
		// генерируем VAO
		glGenVertexArrays(1, &VAO_Index);
		// начинаем работу с созданным VAO
		glBindVertexArray(VAO_Index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Index);
		// указываем свойства аттрибутов шейдера
		// позиция вершины
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, vPosition)));
		// нормаль вершины
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, vNormal)));
		// текстурная координата вершины
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, vTexCoord)));
		// включаем аттрибуты
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		// отвязываем VAO
		glBindVertexArray(0);
		delete[] VertexArr;
	}
	return 0;
}

void CMesh::Render(void)
{
	glBindVertexArray(VAO_Index);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
	glBindVertexArray(0);
}