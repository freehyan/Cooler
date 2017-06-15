#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Cooler
{
//	extern bool g_Keys[1024];
	class CInputCallbacks;

	void __setInputCallbacksGLFW(GLFWwindow* vWindow, CInputCallbacks* vInputCallbacks);
	static void __GLFWKeyBoardCB(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMods);
	static void __GLFWMouseCB(GLFWwindow* vWindow, int vButton, int vAction, int vMode);
	static void __GLFWMotionCB(GLFWwindow* vWindow, double vXPos, double vYPos);
	void __GLFWReshapeCB(GLFWwindow* vWindow, int vWidth, int vHeight);

//	void __GLFWKeyBoardCB(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMode);
//	void __GLFWCursorCB(GLFWwindow* vWindow, double vXPos, double vYPos);
//	void __GLFWScrollCB(GLFWwindow* window, double vXOffset, double vYOffset);
	void APIENTRY glDebugOutput(GLenum vSource, GLenum vType, GLuint vId, GLenum vSeverity, GLsizei vLength, const GLchar *vMessage, const void *vUserParam);
}