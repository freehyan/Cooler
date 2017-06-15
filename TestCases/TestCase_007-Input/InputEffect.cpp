#include "InputEffect.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include "../../Cooler/Cooler/ProductFactory.h"
#include "../../Cooler/Cooler/GraphicsInterface.h"
#include "../../Cooler/Cooler/InputTransformer.h"

Cooler::CProductFactory<CInputEffect> theCreater("INPUT_EFFECT");

CInputEffect::CInputEffect()
{
}

CInputEffect::~CInputEffect()
{
}

//***********************************************************
//FUNCTION:
void CInputEffect::_initEffectV()
{
	__initSceneData();
	m_pInputTransformer = Cooler::fetchInputTransformer();
	m_pInputTransformer->setTranslationVec(glm::vec3(0.0f, -5.0f, -50.0f));
	m_pInputTransformer->setScale(0.1f);
//	m_pInputTransformer->setTranslationVec(glm::vec3(0.0f, 0.0f, -2.2f));
//	m_pInputTransformer->setRotationVec(glm::vec3(glm::pi<float>()*0.35f, 0.0f, 0.0f));
//	m_pInputTransformer->setMotionMode(Cooler::SCameraMotionType::FIRST_PERSON);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

//***********************************************************
//FUNCTION::
void CInputEffect::_renderEffectV()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	__inputPass();
}

//***********************************************************
//FUNCTION::
void CInputEffect::__inputPass()
{
	_ASSERTE(glGetError() == GL_NO_ERROR);
	_enableShader("INPUT_SHADER");
		
	glm::mat4 ViewMatrix = m_pInputTransformer->getModelViewMat();

 	_updateShaderUniform("uViewMatrix", ViewMatrix);
 
	glm::mat4 ModelMatrix;// = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));;
 	_updateShaderUniform("uModelMatrix", ModelMatrix);
	
	glm::mat4 ProjectionMatrix = glm::perspective(3.14f * 0.25f, 1280 / (float)720, 0.1f, 1000.0f);
 	_updateShaderUniform("uProjectionMatrix", ProjectionMatrix);

	Cooler::graphicsRenderModel("CHESS");
	//glBindVertexArray(m_VAO);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	_disableShader("INPUT_SHADER");
	_ASSERTE(glGetError() == GL_NO_ERROR);
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