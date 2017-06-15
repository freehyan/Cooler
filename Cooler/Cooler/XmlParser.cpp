#include "XmlParser.h"
#include <boost/tokenizer.hpp>
#include <glm/glm.hpp>
#include "GraphicsConfig.h"
#include "GraphicsMicro.h"

using namespace Cooler;

CXmlParser::CXmlParser(void)
{
	__preDefineTypeMap();
}

CXmlParser::~CXmlParser(void)
{
}

//***********************************************************
//FUNCTION:
void CXmlParser::parseXmlFile(const std::string& vFileName, CGraphicsConfig* voConfig)
{
	_ASSERT(!vFileName.empty() && voConfig);

	boost::property_tree::ptree Ptree;
	boost::property_tree::read_xml(vFileName, Ptree, boost::property_tree::xml_parser::no_comments | boost::property_tree::xml_parser::trim_whitespace);

	__extractXmlAttributes(Ptree, voConfig);
}

//***********************************************************
//FUNCTION::
void Cooler::CXmlParser::__preDefineTypeMap()
{
	m_PreDefinedTypeMap.insert(std::make_pair("int", EDataType::GRAPHICS_INT));
	m_PreDefinedTypeMap.insert(std::make_pair("float", EDataType::GRAPHICS_FLOAT));
	m_PreDefinedTypeMap.insert(std::make_pair("double", EDataType::GRAPHICS_DOUBLE));
	m_PreDefinedTypeMap.insert(std::make_pair("bool", EDataType::GRAPHICS_BOOL));
	m_PreDefinedTypeMap.insert(std::make_pair("string", EDataType::GRAPHICS_STRING));
	m_PreDefinedTypeMap.insert(std::make_pair("vecstring", EDataType::GRAPHICS_VECSTRING));
	m_PreDefinedTypeMap.insert(std::make_pair("vec2", EDataType::GRAPHICS_VEC2));
	m_PreDefinedTypeMap.insert(std::make_pair("vec3", EDataType::GRAPHICS_VEC3));
	m_PreDefinedTypeMap.insert(std::make_pair("vec4", EDataType::GRAPHICS_VEC4));
	m_PreDefinedTypeMap.insert(std::make_pair("subconfig", EDataType::GRAPHICS_SUBCONFIG));
}

//***********************************************************
//FUNCTION::
void Cooler::CXmlParser::__extractXmlAttributes(const boost::property_tree::ptree& vPropertyTree, CGraphicsConfig* voConfig)
{
	for (auto iter : vPropertyTree)
	{
		std::string Prefix = iter.first;
		if (Prefix == "<xmlattr>") continue;

		std::string TempTypeStr = Prefix + "." + "<xmlattr>" + "." + "type";
		std::string Type = vPropertyTree.get<std::string>(TempTypeStr, "sub");
		std::string TempNameStr = Prefix + "." + "<xmlattr>" + "." + "name";
		std::string AttributeName = vPropertyTree.get<>(TempNameStr, "");

		_ASSERT(m_PreDefinedTypeMap.find(Type) != m_PreDefinedTypeMap.end());

		std::string PropertyKeyFullName = iter.first;

		switch (m_PreDefinedTypeMap.find(Type)->second)
		{
		case EDataType::GRAPHICS_INT:
			voConfig->setAttribute(PropertyKeyFullName, iter.second.get_value<int>());
			break;
		case EDataType::GRAPHICS_FLOAT:
			voConfig->setAttribute(PropertyKeyFullName, iter.second.get_value<float>());
			break;
		case EDataType::GRAPHICS_DOUBLE:
			voConfig->setAttribute(PropertyKeyFullName, iter.second.get_value<double>());
			break;
		case EDataType::GRAPHICS_BOOL:
			voConfig->setAttribute(PropertyKeyFullName, iter.second.get_value<bool>());
			break;
		case EDataType::GRAPHICS_STRING:
			voConfig->setAttribute(PropertyKeyFullName, iter.second.data());
			break;
		case EDataType::GRAPHICS_VECSTRING:
		{
			std::string Str = iter.second.get_value<std::string>();
			boost::char_separator<char> Sep(" ");

			boost::tokenizer<boost::char_separator<char>> Token(Str, Sep);
			std::vector<std::string> Vec;
			for (auto TokenIter : Token)
				Vec.push_back(TokenIter);

			voConfig->setAttribute(PropertyKeyFullName, Vec);
			break;
		}
		case EDataType::GRAPHICS_VEC2:
		{
			std::string Str = iter.second.get_value<std::string>();
			boost::char_separator<char> Sep(" ");
						 
			boost::tokenizer<boost::char_separator<char>> Token(Str, Sep);
			std::vector<float> Vec;
			for (auto TokenIter : Token)
			{
				std::string Temp = TokenIter;
				Vec.push_back((float)std::atof(Temp.c_str()));
			}
						 
			glm::vec2 TokenValue;
			for (unsigned int i = 0; i < Vec.size(); i++)
			{
				TokenValue[i] = Vec[i];
			}
						 
			voConfig->setAttribute(PropertyKeyFullName, TokenValue);
			break;
		}
		case EDataType::GRAPHICS_VEC3:
		{
			std::string Str = iter.second.get_value<std::string>();
			boost::char_separator<char> Sep(" ");
						 
			boost::tokenizer<boost::char_separator<char>> Token(Str, Sep);
			std::vector<float> Vec;
			for (auto TokenIter : Token)
			{
				std::string Temp = TokenIter;
				Vec.push_back((float)std::atof(Temp.c_str()));
			}
						 
			glm::vec3 TokenValue;
			for (unsigned int i = 0; i < Vec.size(); i++)
			{
				TokenValue[i] = Vec[i];
			}
						 
			voConfig->setAttribute(PropertyKeyFullName, TokenValue);
			break;
		}
		case EDataType::GRAPHICS_VEC4:
		{
			std::string Str = iter.second.get_value<std::string>();
			boost::char_separator<char> Sep(" ");
						 
			boost::tokenizer<boost::char_separator<char>> Token(Str, Sep);
			std::vector<float> Vec;
			for (auto TokenIter : Token)
			{
				std::string Temp = TokenIter;
				Vec.push_back((float)std::atof(Temp.c_str()));
			}
						 
			glm::vec4 TokenValue;
			for (unsigned int i = 0; i < Vec.size(); i++)
			{
				TokenValue[i] = Vec[i];
			}
						 
			voConfig->setAttribute(PropertyKeyFullName, TokenValue);
			break;
		}
		case EDataType::GRAPHICS_SUBCONFIG:
		{
			std::string SubConfigName = AttributeName;
			std::shared_ptr<CGraphicsConfig> pSubConfig(new CGraphicsConfig);
			if (SubConfigName.empty())
				SubConfigName = iter.second.get_value<std::string>();

			pSubConfig->setName(SubConfigName);
			pSubConfig->setType(Prefix);
			__extractXmlAttributes(iter.second, pSubConfig.get());
			voConfig->addSubConfig(pSubConfig.get());
			break;
		}
		default:
			break;
		}
	}
}