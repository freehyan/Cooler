#include "ShaderEffect.h"
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <GLM/glm.hpp>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Cooler/Cooler/ProductFactory.h"
#include "../../Cooler/Cooler/GraphicsInterface.h"

Cooler::CProductFactory<CShaderEffect> theCreater("MODEL_EFFECT");

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
	GLuint textureID;
	glGenTextures(1, &textureID);
	int texWidth, texHeight;
	unsigned char *image = SOIL_load_image("../awesomeface.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_FRAMEBUFFER, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}

//***********************************************************
//FUNCTION::
void CShaderEffect::_renderEffectV()
{
	__ModelPass();
}

//***********************************************************
//FUNCTION::
void CShaderEffect::__ModelPass()
{
	_enableShader("MODEL_SHADER");


	_disableShader("MODEL_SHADER");
}

//***********************************************************
//FUNCTION:
void CShaderEffect::_destoryEffectV()
{

}