#include "ShaderEffect.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/ProductFactory.h"

Cooler::CProductFactory<CShaderEffect> theCreater("SHADER_EFFECT");

CShaderEffect::CShaderEffect()
{
}

CShaderEffect::~CShaderEffect()
{
}

//***********************************************************
//FUNCTION:
void CShaderEffect::_initEffectV()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
	GLfloat Vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f, // Right 
		0.0f,  0.5f, 0.0f  // Top   
	};

	glGenVertexArrays(1, &g_VAO);
	glGenBuffers(1, &g_VBO);
	glBindVertexArray(g_VAO);	 // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW ：数据不会或几乎不会改变; GL_DYNAMIC_DRAW：数据会被改变很多; GL_STREAM_DRAW ：数据每次绘制时都会改变。

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//***********************************************************
//FUNCTION::
void CShaderEffect::_renderEffectV()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	__triagnlePass();
}

//***********************************************************
//FUNCTION::
void CShaderEffect::__triagnlePass()
{
	_enableShader("TRIANGLE_SHADER");
	glBindVertexArray(g_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	_disableShader("TRIANGLE_SHADER");
}