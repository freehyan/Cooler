#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <list>
#include <vector>
#include "GraphicsCommon.h"


namespace Cooler
{
	class CShader
	{
	public:
		enum EShaderType
		{
			VERTEX_SHADER = 0,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			COMPUTE_SHADER
		};

	public:
		CShader();
		~CShader();

		bool addShader(EShaderType vShaderType, const std::vector<std::string>& vShaderName);
		bool compileShader();
		void enable();
		void disable();

	private:
		bool __readShaderFile(const std::string& vShaderFileName, std::string& voShaderData);

		void __setMatUniformValuefv(const char *vName, unsigned int vCount, float* vMatrix);

		void __setMatUniformValue(const char *name, float Matrix[16]);
		void __setMat3UniformValue(const char *name, float Matrix[12]);

		void __setFloatUniformValue(const char* name, GLfloat v0);
		void __setFloatUniformValue(const char* name, GLfloat v0, GLfloat v1);
		void __setFloatUniformValue(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
		void __setFloatUniformValue(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void __setFloatUniformValue3v(const char* vName, GLsizei vCount, const GLfloat* vValues);

		void __setIntUniformValue(const char *name, GLint v0);
		void __setIntUniformValue(const char *name, GLint v0, GLint v1);
		void __setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2);
		void __setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2, GLint v3);

		void __setUIntUniformValue(const char *name, GLuint v0);

		void __setHandleui64UniformValue(const char* name, GLuint64 v0);

	private:
		GLuint m_ShaderProgram;
		std::list<GLuint> m_ShaderObjectList;

	friend class CEffect;
	};
}