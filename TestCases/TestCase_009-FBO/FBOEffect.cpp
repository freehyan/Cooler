#include "FBOEffect.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include "../../Cooler/Cooler/ProductFactory.h"
#include "../../Cooler/Cooler/GraphicsInterface.h"
#include "../../Cooler/Cooler/InputTransformer.h"

Cooler::CProductFactory<CInputEffect> theCreater("FBO_EFFECT");

CInputEffect::CInputEffect() : m_pDeferredFBO(nullptr), m_pInputTransformer(nullptr)
{
}

CInputEffect::~CInputEffect()
{
}

//***********************************************************
//FUNCTION:
void CInputEffect::_initEffectV()
{
// 	std::cout << gluErrorString(glGetError()) << std::endl;
// 	_ASSERTE(glGetError() == GL_NO_ERROR);
	__initSceneData();
	m_pInputTransformer = Cooler::fetchInputTransformer();
	m_pInputTransformer->setTranslationVec(glm::vec3(0.0f, -5.0f, -50.0f));
	m_pInputTransformer->setScale(0.1f);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	m_pDeferredFBO = dynamic_cast<CDeferredFBO*>(Cooler::fetchFBO("DEFERRED_FBO"));
	_ASSERTE(m_pDeferredFBO);
//	_ASSERTE(glGetError() == GL_NO_ERROR);
}

//***********************************************************
//FUNCTION::
void CInputEffect::_renderEffectV()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	__geometryPass();
	__showTexturePass();
}

//***********************************************************
//FUNCTION::
void CInputEffect::__geometryPass()
{
	_ASSERTE(glGetError() == GL_NO_ERROR);
	m_pDeferredFBO->openFBO();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glm::mat4 ModelMatrix;// = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));;
	glm::mat4 ViewMatrix = m_pInputTransformer->getModelViewMat();
	glm::mat4 ProjectionMatrix = glm::perspective(3.14f * 0.25f, 1280 / (float)720, 0.1f, 1000.0f);

	glm::mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));
	_enableShader("GEOMETRY_SHADER");

	_updateShaderUniform("uMVPMatrix", MVPMatrix);
	_updateShaderUniform("uModelMatrix", ModelMatrix);
	_updateShaderUniform("uNormalMatrix", NormalMatrix);

	Cooler::graphicsRenderModel("CHESS");
	_disableShader("GEOMETRY_SHADER");

	glDisable(GL_DEPTH_TEST);
	
	m_pDeferredFBO->closeFBO();
	_ASSERTE(glGetError() == GL_NO_ERROR);
}

//***********************************************************
//FUNCTION:
void CInputEffect::__showTexturePass()
{
	_ASSERTE(glGetError() == GL_NO_ERROR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_enableShader("COPYTEX_SHADER");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pDeferredFBO->m_NormalTex);
	__renderScreenSizeQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	_disableShader("COPYTEX_SHADER");
	_ASSERTE(glGetError() == GL_NO_ERROR);
}

//***********************************************************
//FUNCTION:
void CInputEffect::__renderScreenSizeQuad()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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