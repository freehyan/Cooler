#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/any.hpp>
#include "GraphicsExport.h"
#include "BaseProduct.h"

namespace Cooler
{
	class CGraphicsConfig : public CBaseProduct
	{
	public:
		CGraphicsConfig(void);
		CGraphicsConfig(const CGraphicsConfig& vConfig);

		virtual ~CGraphicsConfig(void);

		void addSubConfig(CGraphicsConfig* vConfig);
		void setName(const std::string& vName);
		void setType(const std::string& vType);
		void setAttribute(const std::string& vAttributeName, boost::any vAttributeValue);
		bool isAttributeExisted(const std::string& vAttributeName) const;
		const std::string& getName() const { return m_Name; }
		const std::string& getType() const { return m_Type; }

		const CGraphicsConfig* getParentConfig() const { return m_pParentConfig; }
		CGraphicsConfig* fetchSubConfigByName(const std::string& vName) const;
		CGraphicsConfig* fetchSubConfigAt(unsigned int vIndex) const;
		unsigned int getSubConfigNum() const { return m_SubConfigSet.size(); }
		unsigned int getAttributeNum() const { return m_AttributesMap.size(); }

		virtual CGraphicsConfig* cloneConfigV() const;

		template<class T>
		void getVecAttribute(const std::string& vAttributeName, std::vector<T>& voAttribute)
		{
			std::map<std::string, boost::any>::const_iterator itr;
			for (itr = m_AttributesMap.begin(); itr != m_AttributesMap.end(); itr++)
			{
				if (itr->first == vAttributeName)
				{
					T Value = boost::any_cast<T>(itr->second);
					voAttribute.push_back(Value);
				}
			}
		}

		template<class T>
		T getAttribute(const std::string& vAttributeName) const
		{
			std::map<std::string, boost::any>::const_iterator itr = m_AttributesMap.find(vAttributeName);
			_ASSERT(itr != m_AttributesMap.end());
			T Value;
			bool Success;
			try
			{
				Value = boost::any_cast<T>(itr->second);
				Success = true;
			}
			catch (boost::bad_any_cast e)
			{
#ifdef _DEBUG
				std::cout << "Bad boost::any_cast." << std::endl;
#endif
				Success = false;
			}
			_ASSERT(Success);
			return Value;
		}

	private:
		std::map<std::string, boost::any> m_AttributesMap;
		std::vector<std::shared_ptr<CGraphicsConfig>> m_SubConfigSet;
		CGraphicsConfig* m_pParentConfig;
		std::string m_Name;
		std::string m_Type;

	friend class CConfigParser;
	};
}