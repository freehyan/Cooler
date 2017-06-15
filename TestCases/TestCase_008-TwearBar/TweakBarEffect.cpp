#include "TweakBarEffect.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include "../../Cooler/Cooler/ProductFactory.h"
#include "../../Cooler/Cooler/GraphicsInterface.h"
#include "../../Cooler/Cooler/InputTransformer.h"
#include <AntTweakBar/AntTweakBar.h>

Cooler::CProductFactory<CInputEffect> theCreater("TWEAKBAR_EFFECT");

CInputEffect::CInputEffect() : m_WireFrame(0)
{
}

CInputEffect::~CInputEffect()
{
}

//***********************************************************
//FUNCTION:
void CInputEffect::_initEffectV()
{
	TwBar* pBar = Cooler::fetchTweakBar();
	_ASSERTE(pBar);
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.
 	TwAddVarRW(pBar, "wire", TW_TYPE_BOOL32, &m_WireFrame, " label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");
 	TwAddVarRW(pBar, "bgColor", TW_TYPE_COLOR3F, &m_BgColor, " label='Background color' ");
 
	__initSceneData();
	m_pInputTransformer = Cooler::fetchInputTransformer();
	m_pInputTransformer->setTranslationVec(glm::vec3(0.0f, 0.0f, -2.2f));
	m_pInputTransformer->setRotationVec(glm::vec3(glm::pi<float>()*0.35f, 0.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);
}

//***********************************************************
//FUNCTION::
void CInputEffect::_renderEffectV()
{
	__inputPass();
}

//***********************************************************
//FUNCTION::
void CInputEffect::__inputPass()
{
	glClearColor(m_BgColor[0], m_BgColor[1], m_BgColor[2], 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	_enableShader("TWEAKBAR_SHADER");
	
	glm::mat4 ModelMatrix;
	_updateShaderUniform("uModelMatrix", ModelMatrix);
	
	glm::mat4 ViewMatrix = m_pInputTransformer->getModelViewMat();
 	_updateShaderUniform("uViewMatrix", ViewMatrix);

	glm::mat4 ProjectionMatrix = glm::perspective(3.14f * 0.25f, 1280 / (float)720, 0.1f, 1000.0f);
 	_updateShaderUniform("uProjectionMatrix", ProjectionMatrix);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	_disableShader("TWEAKBAR_SHADER");
	
}

//***********************************************************
//FUNCTION:
void CInputEffect::__initSceneData()
{	
	GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//***********************************************************
//FUNCTION:
void CInputEffect::_destoryEffectV()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}