#include "TweakBar.h"
#include <GLFW/glfw3.h>

using namespace Cooler;

CAntTweakBar::CAntTweakBar() : m_pTwBar(nullptr)
{
	
}

CAntTweakBar::~CAntTweakBar()
{
}

//***********************************************************
//FUNCTION:
void Cooler::CAntTweakBar::createTweakBar()
{
	TwInit(TW_OPENGL, NULL);
	//TwInit(TW_OPENGL_CORE, NULL);
	m_pTwBar = TwNewBar("TweakBar");
}

//***********************************************************
//FUNCTION:
void Cooler::CAntTweakBar::renderTweakBar()
{
	TwDraw();
}

//***********************************************************
//FUNCTION:
bool Cooler::CAntTweakBar::__TweakBarMouseCB(int vButton, int vAction)
{
	TwMouseButtonID MouseButton;

	if (vButton == GLFW_MOUSE_BUTTON_LEFT)
		MouseButton = TW_MOUSE_LEFT;
	else if (vButton == GLFW_MOUSE_BUTTON_MIDDLE)
		MouseButton = TW_MOUSE_MIDDLE;
	else
		MouseButton = TW_MOUSE_RIGHT;

	TwMouseAction MouseAction = (vAction == GLFW_PRESS) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;

	return (TwMouseButton(MouseAction, MouseButton) == 1);
}

//***********************************************************
//FUNCTION:
bool Cooler::CAntTweakBar::__TweakBarMotionCB(int vPosX, int vPosY)
{
	return (TwMouseMotion(vPosX, vPosY) == 1);
}
