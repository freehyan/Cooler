#pragma once
#include <AntTweakBar/AntTweakBar.h>

namespace Cooler
{
	class CAntTweakBar
	{
	public:
		CAntTweakBar();
		~CAntTweakBar();

		void createTweakBar();
		void renderTweakBar();

		bool __TweakBarMouseCB(int vButton, int vAction);
		bool __TweakBarMotionCB(int vPosX, int vPosY);

		TwBar* fetchTweakBar() const { _ASSERTE(m_pTwBar); return m_pTwBar; }

	private:
		

		TwBar* m_pTwBar;
	};
}