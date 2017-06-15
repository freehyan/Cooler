#include "ModelEffect.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <GLM/glm.hpp>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Cooler/Cooler/ProductFactory.h"
#include "../../Cooler/Cooler/GraphicsInterface.h"

Cooler::CProductFactory<CModelEffect> theCreater("MODEL_EFFECT");

CModelEffect::CModelEffect()
{
}

CModelEffect::~CModelEffect()
{
}

//***********************************************************
//FUNCTION:
void CModelEffect::_initEffectV()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

//***********************************************************
//FUNCTION::
void CModelEffect::_renderEffectV()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	__ModelPass();
}

//***********************************************************
//FUNCTION::
void CModelEffect::__ModelPass()
{
	_enableShader("MODEL_SHADER");

	glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
	_updateShaderUniform("uModelMatrix", ModelMatrix);

	glm::mat4 ViewMatrix = Cooler::fectchCameraViewMatrix("MAIN_CAMERA");
	_updateShaderUniform("uViewMatrix", ViewMatrix);

	glm::mat4 ProjectionMatrix = Cooler::fetchCameraProjectionMatrix("MAIN_CAMERA");
	_updateShaderUniform("uProjectionMatrix", ProjectionMatrix);

	Cooler::graphicsRenderModel("CHESS");

	_disableShader("MODEL_SHADER");
}

//***********************************************************
//FUNCTION:
void CModelEffect::_destoryEffectV()
{

}