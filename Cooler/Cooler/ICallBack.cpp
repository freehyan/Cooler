#include "ICallBack.h"
#include "GraphicsCommon.h"
#include "ResourceExplorer.h"
#include "GraphicsInterface.h"
#include "GraphicsMicro.h"
#include "PlatformContext.h"
#include "Render.h"
#include "DisplayWindow.h"
#include "InputTransformer.h"
#include "TweakBar.h"

using namespace Cooler;
static CInputCallbacks* s_Callbacks = nullptr;
static int32_t s_LastButton = 0;

//***********************************************************
//FUNCTION:
void Cooler::__setInputCallbacksGLFW(GLFWwindow* vWindow, CInputCallbacks* vInputCallbacks)
{
	_ASSERTE(vInputCallbacks && vWindow);
	s_Callbacks = vInputCallbacks;
	glfwSetKeyCallback(vWindow, __GLFWKeyBoardCB);
	glfwSetMouseButtonCallback(vWindow, __GLFWMouseCB);
	glfwSetCursorPosCallback(vWindow, __GLFWMotionCB);
	glfwSetFramebufferSizeCallback(vWindow, __GLFWReshapeCB);
}

//***********************************************************
//FUNCTION:
void Cooler::__GLFWKeyBoardCB(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMods)
{
	bool handled = false;

	if (s_Callbacks)
	{
		SKeyActionType::Enum nvact;
		if (vAction == GLFW_PRESS)
			nvact = SKeyActionType::DOWN;
		else if (vAction == GLFW_REPEAT)
			nvact = SKeyActionType::REPEAT;
		else
			nvact = SKeyActionType::UP;
		handled = s_Callbacks->keyInput(vKey, nvact);
	}

// 	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
// 		glfwSetWindowShouldClose(vWindow, GL_TRUE);
}

//***********************************************************
//FUNCTION:
void Cooler::__GLFWMouseCB(GLFWwindow* vWindow, int vButton, int vAction, int vMode)
{
	if (s_Callbacks)
	{
		double x, y;
		glfwGetCursorPos(vWindow, &x, &y);
		CPointerEvent p;
		p.m_x = (float)x;
		p.m_y = (float)y;
		if (vButton == GLFW_MOUSE_BUTTON_LEFT)
			p.m_id = SMouseButton::LEFT;
		else if (vButton == GLFW_MOUSE_BUTTON_MIDDLE)
			p.m_id = SMouseButton::MIDDLE;
		else
			p.m_id = SMouseButton::RIGHT;
		s_LastButton = p.m_id;
		p.m_device = SInputDeviceType::MOUSE;
		s_Callbacks->pointerInput(p.m_device, vAction ? SPointerActionType::DOWN : SPointerActionType::UP, 0, 1, &p);
		
		CAntTweakBar* pTweakBar = IRender::getInstance()->fetchAntTweakBar();
		if (pTweakBar) pTweakBar->__TweakBarMouseCB(vButton, vAction);	
	}
}

//***********************************************************
//FUNCTION:
void Cooler::__GLFWMotionCB(GLFWwindow* vWindow, double vXPos, double vYPos)
{
	if (s_Callbacks)
	{
		CPointerEvent p;
		p.m_x = (float)vXPos;
		p.m_y = (float)vYPos;
		p.m_id = s_LastButton;
		p.m_device = SInputDeviceType::MOUSE;
		s_Callbacks->pointerInput(p.m_device, SPointerActionType::MOTION, 0, 1, &p);
		CAntTweakBar* pTweakBar = IRender::getInstance()->fetchAntTweakBar();
		if (pTweakBar) pTweakBar->__TweakBarMotionCB(vXPos, vYPos);
	}
}

//***********************************************************
//FUNCTION:
void Cooler::__GLFWReshapeCB(GLFWwindow* vWindow, int vWidth, int vHeight)
{
	IRender::getInstance()->fetchDisplayWindow()->setWindowWidth(vWidth);
	IRender::getInstance()->fetchDisplayWindow()->setWindowHeight(vHeight);
	IRender::getInstance()->fetchInputTransformer()->setScreenSize(vWidth, vHeight);
	TwWindowSize(vWidth, vHeight);
}

//***********************************************************
//FUNCTION::
// void Cooler::__GLFWKeyBoardCB(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMode)
// {
// 	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
// 		glfwSetWindowShouldClose(vWindow, GL_TRUE);
// 
// 	if (vAction == GLFW_PRESS) {
// 		g_Keys[vKey] = GL_TRUE;
// 	}
// 	if (vAction == GLFW_RELEASE) {
// 		g_Keys[vKey] = GL_FALSE;
// 	}
// }

//***********************************************************
//FUNCTION::
// void Cooler::__GLFWCursorCB(GLFWwindow* vWindow, double vXPos, double vYPos)
// {
// 	CCamera* pCamera = graphicsFindCamera("MAIN_CAMERA");
// 
// 	if (g_FirstCursor) {
// 		g_LastCursorX = vXPos;
// 		g_LastCursorY = vYPos;
// 		g_FirstCursor = false;
// 	}
// 
// 	//计算前后两帧的鼠标位移量
// 	GLfloat xOffset = (vXPos - g_LastCursorX);
// 	GLfloat yOffset = (g_LastCursorY - vYPos);  //注意是相反的，因为鼠标坐标原点在窗口左上角
// 	g_LastCursorX = vXPos;
// 	g_LastCursorY = vYPos;
// 	pCamera->processRotate(xOffset, yOffset);
// }
// 
// 
// //***********************************************************
// //FUNCTION::
// void Cooler::__GLFWScrollCB(GLFWwindow* window, double vXOffset, double vYOffset)
// {
// 	CCamera* pCamera = graphicsFindCamera("MAIN_CAMERA");
// 	pCamera->processScroll(vXOffset, vYOffset);
// }

//***********************************************************
//FUNCTION:
void APIENTRY Cooler::glDebugOutput(GLenum vSource, GLenum vType, GLuint vId, GLenum vSeverity, GLsizei vLength, const GLchar *vMessage, const void *vUserParam)
{
	if (vId == 131169 || vId == 131185 || vId == 131218 || vId == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << vId << "): " << vMessage << std::endl;

	switch (vSource)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (vType)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (vSeverity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
}