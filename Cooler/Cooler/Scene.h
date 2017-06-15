#pragma once
#include <iostream>
#include <map>

namespace Cooler
{
	class CCamera;
	class CGraphicsConfig;

	class CScene 
	{
	public:
		CScene();
		~CScene();

		bool initScene();
		const CGraphicsConfig* getSceneConfig() const { _ASSERTE(m_pSceneConfig); return m_pSceneConfig; }
		CCamera* fecthCamera(const std::string& vCameraName);
		const CCamera* getCamera(const std::string& vCameraName) const;

	protected:
	//	virtual bool _onResourceCreatedV(const CGraphicsConfig* vConfig) override;


	private:
		std::map<std::string, CCamera*> m_pCameraSet;
		CGraphicsConfig* m_pSceneConfig;
	};
}