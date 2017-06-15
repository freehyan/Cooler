#pragma once
#include "Singleton.h"

namespace Cooler
{
	class IResourceManager;
	class CGraphicsConfig;
	class IResource;

	class CResourceExplorer : public CSingleton<CResourceExplorer>
	{
	public:
		virtual ~CResourceExplorer();
		
		void createAllResourceManager();
		void createResource(const CGraphicsConfig* vConfig) const;

		IResourceManager* findResourceManager(const std::string& vResourceType);

	protected:
		CResourceExplorer();
	
	private:
		std::map<std::string, IResourceManager*> m_ResourceManagerSet;

	friend class CSingleton<CResourceExplorer>;
	};
}