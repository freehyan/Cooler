#include "Shader.h"
#include <iostream>
#include <fstream>

using namespace Cooler;

CShader::CShader()
{
	m_ShaderProgram = glCreateProgram();
}

CShader::~CShader()
{
	glDeleteProgram(m_ShaderProgram);
}

//***********************************************************
//FUNCTION::
bool Cooler::CShader::addShader(EShaderType vShaderType, const std::vector<std::string>& vShaderName)
{
	std::string ShaderData;

	for (unsigned int i=0; i<vShaderName.size(); i++)
	{
		std::string TempStr;
		if (!__readShaderFile(vShaderName[i], TempStr))
			return false;
		ShaderData += TempStr;
	}

	GLuint ShaderObject;
	switch (vShaderType)
	{
	case VERTEX_SHADER:
		ShaderObject = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GEOMETRY_SHADER:
		ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case COMPUTE_SHADER:
		ShaderObject = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		break;
	}

	const GLchar *pShaderData = ShaderData.c_str();
	const GLint Length = ShaderData.length();
	glShaderSource(ShaderObject, 1, &pShaderData, &Length);
	glCompileShader(ShaderObject);

	GLint Success;
	GLchar ErrorLog[1024];
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(ShaderObject, 1024, NULL, ErrorLog);

		std::string ErrorFileName;
		for (unsigned int i=0; i<vShaderName.size(); i++)
			ErrorFileName += vShaderName[i] + " ";
	
		fprintf(stderr, "Error compiling shader '%s' : \n '%s' \n", ErrorFileName.c_str(), ErrorLog);
		return false;
	}

	m_ShaderObjectList.push_back(ShaderObject);
	glAttachShader(m_ShaderProgram, ShaderObject);
	return true;
}

//***********************************************************
//FUNCTION::
bool Cooler::CShader::compileShader()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program : \n '%s' \n", ErrorLog);
		return false;
	}

	glValidateProgram(m_ShaderProgram);
	glGetProgramiv(m_ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 1024, NULL, ERROR);
		fprintf(stderr, "Error validating shader program : \n'%s' \n", ErrorLog);
		return false;
	}

	for (std::list<GLuint>::iterator it = m_ShaderObjectList.begin(); it != m_ShaderObjectList.end(); it++)
	{
		glDeleteShader(*it);
	}
	m_ShaderObjectList.clear();

	return Success;
}

//***********************************************************
//FUNCTION::
void Cooler::CShader::enable()
{
	glUseProgram(m_ShaderProgram);
}

//***********************************************************
//FUNCTION::
void Cooler::CShader::disable()
{
	glUseProgram(0);
}

//***********************************************************
//FUNCTION::
bool Cooler::CShader::__readShaderFile(const std::string& vShaderFileName, std::string& voShaderData)
{
	bool Success = false;
	std::ifstream InFile(vShaderFileName.c_str());

	if (InFile.is_open())
	{
		std::string LineStr;
		while (getline(InFile, LineStr))
		{
			voShaderData.append(LineStr.c_str());
			voShaderData.append("\n");
		}
		Success = true;
	}
	return Success;
}

//***********************************************************
//FUNCTION:
void Cooler::CShader::__setMatUniformValuefv(const char *vName, unsigned int vCount, float* vMatrix)
{
	GLint key = glGetUniformLocation(m_ShaderProgram, vName);
	if (key >= 0)
		glUniformMatrix4fv(key, vCount, GL_FALSE, vMatrix);
	else
		printf("MatUniformValue %s not found\n", vName);
}

//******************************************************************
//FUNCTION:
void CShader::__setMatUniformValue(const char *name, float Matrix[16])
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniformMatrix4fv(key, 1, GL_FALSE, Matrix);
	else
		printf("MatUniformValue %s not found\n", name);
}

//***********************************************************
//FUNCTION::
void CShader::__setMat3UniformValue(const char *name, float Matrix[12])
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniformMatrix3fv(key, 1, GL_FALSE, Matrix);
	else
		printf("MatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setFloatUniformValue(const char *name, GLfloat v0)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform1f(key, v0);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform2f(key, v0, v1);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform3f(key, v0, v1, v2);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform4f(key, v0, v1, v2, v3);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setIntUniformValue(const char *name, GLint v0)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform1i(key, v0);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setIntUniformValue(const char *name, GLint v0, GLint v1)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform2i(key, v0, v1);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform3i(key, v0, v1, v2);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2, GLint v3)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform4i(key, v0, v1, v2, v3);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setFloatUniformValue3v(const char* vName, GLsizei vCount, const GLfloat* vValues)
{
	GLint Key;
	Key = glGetUniformLocation(m_ShaderProgram, vName);

	if (Key >= 0)
		glUniform3fv(Key, vCount, vValues);
	else
		std::cout << "Float uniform value " << vName << " not found" << std::endl;
}

//***********************************************************
//FUNCTION::
void CShader::__setUIntUniformValue(const char *name, GLuint v0)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniform1ui(key, v0);
	else
		printf("UIntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::__setHandleui64UniformValue(const char* name, GLuint64 v0)
{
	GLint key;
	key = glGetUniformLocation(m_ShaderProgram, name);
	if (key >= 0)
		glUniformHandleui64NV(key, v0);
	else
		printf("Handleui64UniformValue %s not found\n", name);
}
