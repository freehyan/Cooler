#pragma once
#include <string>
#include "GraphicsInterface.h"
#include <GLFW/glfw3.h>

namespace Cooler
{
	class CDisplayWindow
	{
	public:
		CDisplayWindow();
		~CDisplayWindow();

		bool createWindow(const std::string& vWindowTitle, unsigned int vWindowWidth, unsigned int vWindowHeight);
		GLFWwindow* fetchWindowInstance() const { _ASSERTE(m_pWindowInstance); return m_pWindowInstance; }
		int windowShouldClose();
		
		inline const unsigned int getWindowWidth() const { return m_Width; }
		inline const unsigned int getWindowHeight() const { return m_Height; }

		inline void setWindowWidth(const int vWidth) { m_Width = vWidth; }
		inline void setWindowHeight(const int vHeight) { m_Height = vHeight; }
		
	private:
		GLFWwindow* m_pWindowInstance;
		GLuint m_Width, m_Height;
	};
}