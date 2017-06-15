#include "DisplayWindow.h"
#include "GraphicsMicro.h"


using namespace Cooler;

CDisplayWindow::CDisplayWindow() : m_pWindowInstance(nullptr), m_Width(0), m_Height(0)
{
}

CDisplayWindow::~CDisplayWindow()
{
	if (!m_pWindowInstance)
	{
		glfwDestroyWindow(m_pWindowInstance);
		m_pWindowInstance = nullptr;
	}
}

//***********************************************************
//FUNCTION::
bool Cooler::CDisplayWindow::createWindow(const std::string& vWindowTitle, unsigned int vWindowWidth, unsigned int vWindowHeight)
{
	_ASSERTE(!vWindowTitle.empty() && vWindowWidth > 0 && vWindowHeight > 0);
	m_Width = vWindowWidth;
	m_Height = vWindowHeight;

	if (!glfwInit()) return false;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用 Core-profile 核心模式
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); //调试输出上下文,造成OpenGL运行缓慢,发布代码时需注释;
#endif
	m_pWindowInstance = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowTitle.c_str(), nullptr, nullptr);
	if (!m_pWindowInstance)
	{
		Cooler::outputWarning(__EXCEPTION_SITE__,  "Create window instance fail.");
		glfwTerminate();
		return false;
	}
	glfwSetWindowPos(m_pWindowInstance, 100, 100);
	return true;
}

//******************************************************************
//FUNCTION:
int CDisplayWindow::windowShouldClose()
{
	return glfwWindowShouldClose(m_pWindowInstance);
}
