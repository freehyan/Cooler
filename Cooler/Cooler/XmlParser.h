#pragma once
#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/any.hpp>
#include "Singleton.h"
#include "GraphicsCommon.h"

namespace Cooler
{
	class CGraphicsConfig;

	class CXmlParser : public CSingleton<CXmlParser>
	{
	public:
		~CXmlParser(void);

		void parseXmlFile(const std::string& vFileName, CGraphicsConfig* voConfigInfo);

	protected:
		CXmlParser(void);

	private:
		void __preDefineTypeMap();
		void __extractXmlAttributes(const boost::property_tree::ptree& vPropertyTree, CGraphicsConfig* voConfig);

		std::map<std::string, EDataType> m_PreDefinedTypeMap;

	friend class CSingleton<CXmlParser>;
	};
}