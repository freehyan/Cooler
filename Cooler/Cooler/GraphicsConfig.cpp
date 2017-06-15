#include "GraphicsConfig.h"
#include <boost/tokenizer.hpp>
#include <glm/glm.hpp>
#include "GraphicsMicro.h"
#include "XmlParser.h"

using namespace Cooler;

CGraphicsConfig::CGraphicsConfig(void) : m_pParentConfig(nullptr)
{
}

CGraphicsConfig::CGraphicsConfig(const CGraphicsConfig& vConfig)
{
	m_Type = vConfig.m_Type;
	m_Name = vConfig.m_Name;
	m_pParentConfig = vConfig.m_pParentConfig;
	m_AttributesMap = vConfig.m_AttributesMap;

	for (unsigned int i = 0; i < vConfig.m_SubConfigSet.size(); i++)
	{
		CGraphicsConfig *pConfig = vConfig.m_SubConfigSet[i]->cloneConfigV();
		pConfig->m_pParentConfig = this;
		std::shared_ptr<CGraphicsConfig> t(pConfig);
		m_SubConfigSet.push_back(t);
	}
}

CGraphicsConfig::~CGraphicsConfig(void)
{
}

//***********************************************************
//FUNCTION:
void Cooler::CGraphicsConfig::setType(const std::string& vType)
{
	_ASSERT(!vType.empty());
	m_Type = boost::algorithm::to_upper_copy(vType);
}

//******************************************************************
//FUNCTION:
void CGraphicsConfig::setName(const std::string& vName)
{
	_ASSERT(!vName.empty());
	m_Name = boost::algorithm::to_upper_copy(vName);
}

//***********************************************************
//FUNCTION:
CGraphicsConfig* Cooler::CGraphicsConfig::cloneConfigV() const 
{
	return new CGraphicsConfig(*this);
}

//******************************************************************
//FUNCTION:
bool CGraphicsConfig::isAttributeExisted(const std::string& vAttributeName) const
{
	std::map<std::string, boost::any>::const_iterator itr = m_AttributesMap.find(vAttributeName);

	if (itr != m_AttributesMap.end()) return true;

	return false;
}

//******************************************************************
//FUNCTION:
void CGraphicsConfig::addSubConfig(CGraphicsConfig* vConfig)
{
	if (!vConfig) return;

	std::shared_ptr<CGraphicsConfig> t(vConfig->cloneConfigV());
	t->m_pParentConfig = this;
	m_SubConfigSet.push_back(t);
}

//******************************************************************
//FUNCTION:
void CGraphicsConfig::setAttribute(const std::string& vAttributeName, boost::any vAttributeValue)
{
	m_AttributesMap.insert(std::make_pair(vAttributeName, vAttributeValue));
}

//******************************************************************
//FUNCTION:
CGraphicsConfig* CGraphicsConfig::fetchSubConfigByName(const std::string& vName) const
{
	for (auto iter : m_SubConfigSet)
	{
		if (iter->m_Name == vName) 
			return iter.get();
	}

	return nullptr;
}

//******************************************************************
//FUNCTION:
CGraphicsConfig* CGraphicsConfig::fetchSubConfigAt(unsigned int vIndex) const
{
	return (vIndex >= m_SubConfigSet.size()) ? nullptr : m_SubConfigSet[vIndex].get();
}
