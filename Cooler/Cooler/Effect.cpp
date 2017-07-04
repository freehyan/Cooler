#include "Effect.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "GraphicsCommon.h"
#include "GraphicsMicro.h"
#include "GraphicsInterface.h"
#include "GraphicsConfig.h"
#include "Shader.h"
#include "Texture.h"
#include "FBO.h"
#include "FBOManager.h"
#include "ResourceExplorer.h"

using namespace Cooler;

CEffect::CEffect() : m_pEffectConfig(nullptr), m_EffectState(EFFECT_ON), m_pCurrentShader(nullptr)
{
}

CEffect::~CEffect()
{
	_SAFE_DELETE(m_pEffectConfig);
	for (auto shader : m_ShaderMap)
		_SAFE_DELETE(shader.second);
}

//***********************************************************
//FUNCTION::
void Cooler::CEffect::enableEffect()
{
	m_EffectState = EFFECT_ON;
}

//***********************************************************
//FUNCTION::
void Cooler::CEffect::disableEffect()
{
	m_EffectState = EFFECT_OFF;
}

//***********************************************************
//FUNCTION::
bool Cooler::CEffect::initEffectWithConfig(const CGraphicsConfig* vEffectConfig)
{
	if (!vEffectConfig) return false;

	m_pEffectConfig = vEffectConfig->cloneConfigV();
	m_EffectSig = m_pEffectConfig->getName();
	
	if (m_pEffectConfig->isAttributeExisted(CONFIG_KEYWORD::EFFECT_STATE))
	{
		std::string Str = m_pEffectConfig->getAttribute<std::string>(CONFIG_KEYWORD::EFFECT_STATE);
		if (Str == "OFF")
			disableEffect();
		else
			enableEffect();
	}

	for (unsigned int i = 0; i < m_pEffectConfig->getSubConfigNum(); i++)
	{
		CGraphicsConfig* pShaderConfig = m_pEffectConfig->fetchSubConfigAt(i);
		_ASSERTE(pShaderConfig);

		if (!__initEffectShader(pShaderConfig)) return false;
	}

	return true;
}

//***********************************************************
//FUNCTION::
void Cooler::CEffect::loadEffect()
{
	if (m_EffectState == EFFECT_OFF) return;

	_renderEffectV();
}

//***********************************************************
//FUNCTION::
bool Cooler::CEffect::_enableShader(const std::string& vShaderName)
{
	if (m_ShaderMap.find(vShaderName) == m_ShaderMap.end()) return false;
	m_pCurrentShader = m_ShaderMap[vShaderName];
	m_pCurrentShader->enable();

	return true;
}

//***********************************************************
//FUNCTION::
bool Cooler::CEffect::_disableShader(const std::string& vShaderName)
{
	if (m_ShaderMap.find(vShaderName) == m_ShaderMap.end()) return false;

	m_ShaderMap[vShaderName]->disable();
	m_pCurrentShader = nullptr;
	return true;
}

//***********************************************************
//FUNCTION::
void Cooler::CEffect::_enableTexture(const std::string& vTextueName, unsigned int vTexUnitId)
{
	_ASSERTE(!vTextueName.empty() && vTexUnitId >= 0);
	CTexture* pTexture = graphicsFindTexture(vTextueName);
	glActiveTexture(vTexUnitId);
	pTexture->useTexture();
}

//***********************************************************
//FUNCTION::
void Cooler::CEffect::_disableTexture(const std::string& vTextueName, unsigned int vTexUnitId)
{
	_ASSERTE(!vTextueName.empty() && vTexUnitId >= 0);
	CTexture* pTexture = graphicsFindTexture(vTextueName);
	glActiveTexture(vTexUnitId);
	pTexture->banTexture();
}

//***********************************************************
//FUNCTION:
void Cooler::CEffect::_updateShaderUniform(const std::string& vUniformName, boost::any vValue)
{
	_ASSERT(!vUniformName.empty() && m_pCurrentShader);
	if (m_pCurrentShader)
	{
		const std::type_info& typeInfo = vValue.type();
		if (typeInfo == typeid(glm::mat4))
		{
			glm::mat4 mat = boost::any_cast<glm::mat4>(vValue);
			m_pCurrentShader->__setMatUniformValue(vUniformName.c_str(), glm::value_ptr(mat));
		}
		else if (typeInfo == typeid(glm::mat3))
		{
			glm::mat3 mat = boost::any_cast<glm::mat3>(vValue);
			m_pCurrentShader->__setMat3UniformValue(vUniformName.c_str(), glm::value_ptr(mat));
		}
		else if (typeInfo == typeid(glm::vec4))
		{
			glm::vec4 v = boost::any_cast<glm::vec4>(vValue);
			m_pCurrentShader->__setFloatUniformValue(vUniformName.c_str(), v[0], v[1], v[2], v[3]);
		}
		else if (typeInfo == typeid(glm::vec3))
		{
			glm::vec3 v = boost::any_cast<glm::vec3>(vValue);
			m_pCurrentShader->__setFloatUniformValue(vUniformName.c_str(), v[0], v[1], v[2]);
		}
		else if (typeInfo == typeid(glm::vec2))
		{
			glm::vec2 v = boost::any_cast<glm::vec2>(vValue);
			m_pCurrentShader->__setFloatUniformValue(vUniformName.c_str(), v[0], v[1]);
		}
		else if (typeInfo == typeid(float))
		{
			float v = boost::any_cast<float>(vValue);
			m_pCurrentShader->__setFloatUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(double))
		{
			double v = boost::any_cast<double>(vValue);
			m_pCurrentShader->__setFloatUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(bool))
		{
			bool v = boost::any_cast<bool>(vValue);
			m_pCurrentShader->__setIntUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(int))
		{
			int v = boost::any_cast<int>(vValue);
			m_pCurrentShader->__setIntUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(unsigned int))
		{
			unsigned int v = boost::any_cast<unsigned int>(vValue);
			m_pCurrentShader->__setUIntUniformValue(vUniformName.c_str(), v);
		}
		else if (typeInfo == typeid(GLuint64))
		{
			GLuint64 v = boost::any_cast<GLuint64>(vValue);
			m_pCurrentShader->__setHandleui64UniformValue(vUniformName.c_str(), v);
		}
		else
		{
			Cooler::outputWarning(__EXCEPTION_SITE__, vUniformName + " type is not defined");
		}
	}
}

//***********************************************************
//FUNCTION:
void Cooler::CEffect::_updateShaderUniformfv(const std::string& vUniformName, unsigned int vCount, float* vVlaue)
{
	_ASSERT(!vUniformName.empty() && m_pCurrentShader);
	m_pCurrentShader->__setMatUniformValuefv(vUniformName.c_str(), vCount, vVlaue);
}

//***********************************************************
//FUNCTION::
unsigned int Cooler::CEffect::_getShaderId(const std::string& vShaderName)
{
 	if (m_ShaderMap.find(vShaderName) == m_ShaderMap.end()) return 0;
 	m_pCurrentShader = m_ShaderMap[vShaderName];
 	return m_pCurrentShader->getShaderProgram();
}

//***********************************************************
//FUNCTION::
bool Cooler::CEffect::__initEffectShader(const CGraphicsConfig* vShaderConfig)
{
	if (!vShaderConfig) return false;

	CShader* pShader = new CShader;
	_ASSERTE(pShader);

	if (vShaderConfig->isAttributeExisted(CONFIG_KEYWORD::VERTEX_SHADER))
		pShader->addShader(CShader::EShaderType::VERTEX_SHADER, vShaderConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::VERTEX_SHADER));	

	if (vShaderConfig->isAttributeExisted(CONFIG_KEYWORD::FRAGMENT_SHADER))
		pShader->addShader(CShader::EShaderType::FRAGMENT_SHADER, vShaderConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::FRAGMENT_SHADER));

	if (vShaderConfig->isAttributeExisted(CONFIG_KEYWORD::GEOMETRY_SHADER))
		pShader->addShader(CShader::EShaderType::GEOMETRY_SHADER, vShaderConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::GEOMETRY_SHADER));

	if (vShaderConfig->isAttributeExisted(CONFIG_KEYWORD::COMPUTE_SHADER))
		pShader->addShader(CShader::EShaderType::COMPUTE_SHADER, vShaderConfig->getAttribute<std::vector<std::string>>(CONFIG_KEYWORD::COMPUTE_SHADER));
		
	if (!pShader->compileShader())  return false;

	std::string ShaderName = vShaderConfig->getName();
	m_ShaderMap.insert(std::make_pair(ShaderName, pShader));

	return true;
}