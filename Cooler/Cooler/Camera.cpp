#include "Camera.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include "ICallBack.h"
#include <iostream>

using namespace Cooler;
//extern bool Cooler::g_Keys[1024];

//***********************************************************
//FUNCTION:
Cooler::CCamera::CCamera(const glm::vec3& vPos, const glm::vec3& vCenter, const glm::vec3& vUp)
{
	m_CameraPos = vPos;
	m_CameraCenter = vCenter;
	m_CameraUp = vUp;
	m_WorldUp = vUp;

	m_CameraDirection = glm::normalize(m_CameraCenter - m_CameraPos);
	m_Camera2CenterLength = glm::length((m_CameraCenter - m_CameraPos));
	m_CameraRight = glm::normalize(glm::cross(m_CameraDirection, m_WorldUp));

	m_Pitch = asin(m_CameraDirection.y);
	m_Yaw = asin(m_CameraDirection.z / cos(m_Pitch));

	//Magic number
	m_Sensitivity = 0.03f;
	m_CameraSpeed = 5.0f;
}

CCamera::~CCamera()
{
}

//***********************************************************
//FUNCTION::
void Cooler::CCamera::initPerspective(const glm::vec3& vPerspective, double vWidth, double vHeight)
{
	m_CameraFOV = vPerspective.x;
	m_Near = vPerspective.y;
	m_Far = vPerspective.z;
	m_CameraWidth = vWidth;
	m_CameraHeight = vHeight;
}

//***********************************************************
//FUNCTION:
void Cooler::CCamera::updateCamera(GLfloat vDeltaTime)
{
// 	if (g_Keys[GLFW_KEY_W])
// 		__processMovement(FORWARD, vDeltaTime);
// 	if (g_Keys[GLFW_KEY_S]) 
// 		__processMovement(BACKWARD, vDeltaTime);
// 	if (g_Keys[GLFW_KEY_D]) 
// 		__processMovement(RIGHT, vDeltaTime);
// 	if (g_Keys[GLFW_KEY_A]) 
// 		__processMovement(LEFT, vDeltaTime);
}

//***********************************************************
//FUNCTION:
void Cooler::CCamera::__processMovement(ECameraDirection vDirection, GLfloat vDeltaTime)
{
	GLfloat Velocity = m_CameraSpeed * vDeltaTime;
	if (vDirection == FORWARD) {
		m_CameraPos += Velocity * m_CameraDirection;
	}
	if (vDirection == BACKWARD) {
		m_CameraPos -= Velocity * m_CameraDirection;
	}
	if (vDirection == RIGHT) {
		m_CameraPos += Velocity * m_CameraRight;
	}
	if (vDirection == LEFT) {
		m_CameraPos -= Velocity * m_CameraRight;
	}
}

//***********************************************************
//FUNCTION:
void Cooler::CCamera::processRotate(double vXOffset, double vYOffset)
{
	vXOffset *= m_Sensitivity;
	vYOffset *= m_Sensitivity;
	
	m_Yaw += (GLfloat)glm::radians(vXOffset);
	m_Pitch += (GLfloat)glm::radians(vYOffset);

	if (m_Pitch > glm::radians(89.0f)) {
		m_Pitch = glm::radians(89.0f);
	}
	else if (m_Pitch < glm::radians(-89.0f)) {
		m_Pitch = glm::radians(-89.0f);
	}

	m_CameraDirection.x = cos(m_Pitch) * cos(m_Yaw);
	m_CameraDirection.y = sin(m_Pitch);
	m_CameraDirection.z = cos(m_Pitch) * sin(m_Yaw);

	m_CameraDirection = glm::normalize(m_CameraDirection);
}

//***********************************************************
//FUNCTION:
void Cooler::CCamera::processScroll(GLfloat vOffset, GLfloat vYOffset)
{
	if (m_CameraFOV >= 1.0f && m_CameraFOV <= 90.0f) {
		m_CameraFOV -= vYOffset;
	}
	if (m_CameraFOV < 1.0f) {
		m_CameraFOV = 1.0f;
	}
	else if (m_CameraFOV > 90.0f) {
		m_CameraFOV = 90.0f;
	}
}

//***********************************************************
//FUNCTION:
glm::mat4 Cooler::CCamera::getViewMatrix() const
{
	glm::vec3 Center = m_CameraPos + m_CameraDirection * m_Camera2CenterLength;
	return glm::lookAt(m_CameraPos, Center, m_CameraUp);
}

//***********************************************************
//FUNCTION::
glm::mat4 Cooler::CCamera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_CameraFOV), (float)m_CameraWidth / (float)m_CameraHeight, m_Near, m_Far);
}

//***********************************************************
//FUNCTION:
void Cooler::CCamera::__updateCameraVector()
{
	m_CameraRight = glm::normalize(glm::cross(m_CameraDirection, m_WorldUp));
	m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraDirection));
}
