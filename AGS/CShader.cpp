#include "CShader.h"
bool fileF;

std::vector<std::string> LoadFile(char *FileName, int &size) {
	std::string str;
	std::vector<std::string> file;
	std::ifstream VertexFile(FileName);
	if (VertexFile) {
		while (std::getline(VertexFile, str))
		{
			std::cout << str << std::endl;
			file.push_back(str + "\n");
		}
		size = file.size();
		fileF = true;
	}
	else
	{
		std::cout << FileName << " dont found" << std::endl << std::endl;
		fileF = false;
	}
	return file;
}

char ** VecToCharPtr(std::vector<std::string> &vec)
{
	char **ptr = new char *[vec.size()];
	for (int i = 0; i < (uint)vec.size(); i++)
	{
		ptr[i] = (char*)vec[i].c_str();
	}
	return ptr;
}

int CShader::LoadShader(char *VertexFileName, int type, bool DebugOutput)
{
	int size;
	int Shader;
	int CompileResult;
	int InfoLogLength = 0;
	std::vector<std::string>  SourceVec = LoadFile(VertexFileName, size);
	if (!fileF) return 0;
	else
	{
		char** Source = VecToCharPtr(SourceVec);
		switch (type)
		{
		case 1:
			Shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case 2:
			Shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		}
		glShaderSource(Shader, size, Source, NULL);
		glCompileShader(Shader);
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &CompileResult);
		glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (DebugOutput)
		{
			std::cout << VertexFileName << std::endl << "Compile status: ";
			if (CompileResult)
			{
				std::cout << "Success;" << std::endl << std::endl;
			}
			else
			{
				std::cout << "Fail;" << std::endl;
				if (InfoLogLength > 0)
				{
					char *ErrorMessage = new char[InfoLogLength + 1];
					glGetShaderInfoLog(Shader, InfoLogLength, NULL, ErrorMessage);
					std::cout << ErrorMessage << std::endl << std::endl;
					delete ErrorMessage;
				}
			}
		}
		delete[] Source;
		return Shader;
	}
}

int CShader::LoadVertexShader(char * VertexFileName, bool DebugOutput)
{
	Vsh = LoadShader(VertexFileName, 1);
	return 0;
}

int CShader::LoadFragmentShader(char * FragmentFileName, bool DebugOutput)
{
	Fsh = LoadShader(FragmentFileName, 2);
	return 0;
}

int CShader::Link(bool DebugOutput)
{
	int LinkStatus;
	int InfoLogLength;
	Program = glCreateProgram();
	glAttachShader(Program, Vsh);
	glAttachShader(Program, Fsh);
	glLinkProgram(Program);
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkStatus);
	glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::cout << "Link status: ";
	if (LinkStatus)
	{
		std::cout << "Success!" << std::endl;
	}
	else
	{
		std::cout << "Fail;" << std::endl << std::endl;;
		if (InfoLogLength > 0)
		{
			char *ErrorMessage = new char[InfoLogLength + 1];
			glGetProgramInfoLog(Program, InfoLogLength, NULL, ErrorMessage);
			std::cout << ErrorMessage << std::endl << std::endl;;
		}
	}
	return 0;
}

void CShader::SetUniform(std::string name, GLfloat &value)
{
	int uniformID;
	auto variable = UniformVariables.find(name);
	if (variable != UniformVariables.end())
	{
		uniformID = variable->second;
	}
	else
	{
		uniformID = glGetUniformLocation(Program, name.c_str());
		UniformVariables.insert(std::pair<std::string, int>(name, uniformID));
	}
	if (uniformID < 0) return;
	glUseProgram(Program);
	glUniform1f(uniformID, value);
}


void CShader::SetUniform(std::string name, vec4 &value)
{
	int uniformID;
	auto variable = UniformVariables.find(name);
	if (variable != UniformVariables.end())
	{
		uniformID = variable->second;
	}
	else
	{
		uniformID = glGetUniformLocation(Program, name.c_str());
		UniformVariables.insert(std::pair<std::string, int>(name, uniformID));
	}
	if (uniformID < 0) return;
	glUseProgram(Program);
	glUniform4fv(uniformID, 1, value_ptr(value));
}

void CShader::SetUniform(std::string name, mat4 &value)
{
	int uniformID;
	auto variable = UniformVariables.find(name);
	if (variable != UniformVariables.end())
	{
		uniformID = variable->second;
	}
	else
	{
		uniformID = glGetUniformLocation(Program, name.c_str());
		UniformVariables.insert(std::pair<std::string, int>(name, uniformID));
	}
	if (uniformID < 0) return;
	glUseProgram(Program);
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, value_ptr(value));
}

void CShader::SetUniform(std::string name, int value)
{
	int uniformID;
	auto variable = UniformVariables.find(name);
	if (variable != UniformVariables.end())
	{
		uniformID = variable->second;
	}
	else
	{
		uniformID = glGetUniformLocation(Program, name.c_str());
		UniformVariables.insert(std::pair<std::string, int>(name, uniformID));
	}
	if (uniformID < 0) return;
	glUseProgram(Program);
	glUniform1i(uniformID, value);
}

void CShader::Activate(void)
{
	glUseProgram(Program);
}

void CShader::Deactivate(void)
{
	glUseProgram(0);
}
