#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Resource.h"

namespace Cooler
{
	class CCamera : public IResource
	{
		enum ECameraDirection
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};

	public:
		CCamera(const glm::vec3& vPos, const glm::vec3& vCenter, const glm::vec3& vUp);
		virtual ~CCamera();

		void initPerspective(const glm::vec3& vPerspective, double vWidth, double vHeight);

		void updateCamera(GLfloat vDeltaTime);
		void processRotate(double vXOffset, double vYOffset);
		void processScroll(GLfloat vOffset, GLfloat vYOffset);

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

	private:
		void __updateCameraVector();
		void __processMovement(ECameraDirection vDirection, GLfloat vDeltaTime);
	
	private:
		int m_CameraWidth, m_CameraHeight;
		float m_Near, m_Far;
		glm::vec3 m_CameraPos, m_CameraCenter, m_CameraUp;

		glm::vec3 m_CameraDirection;
		GLfloat m_Camera2CenterLength;

		glm::vec3 m_CameraRight, m_WorldUp;
		GLfloat m_Yaw;			//Å·À­½Ç£¬Æ«º½½Ç
		GLfloat m_Pitch;		//¸©Ñö½Ç

		GLfloat m_CameraFOV;
		GLfloat m_CameraSpeed;	//ÉãÏñ»úÒÆ¶¯ËÙ	
		GLfloat m_Sensitivity;	//ÉãÏñ»úÐý×ªÁéÃô¶È
	};
}