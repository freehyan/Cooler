#include "InputTransformer.h"
#include <algorithm>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp> // glm::pi

using namespace Cooler;

#define NV_MIN_SCALE_PCT    0.035f
#define NV_MAX_SCALE_PCT    500.0f
#define NV_CLAMP_SCALE(s)   std::min(std::max((s), NV_MIN_SCALE_PCT), NV_MAX_SCALE_PCT)

CInputTransformer::CInputTransformer() : 
	m_motionMode(SCameraMotionType::ORBITAL),
	m_touchDown(false),
	m_mode(TRANSLATE),
	m_lastInput(0, 0),
	m_width(1),
	m_height(1),
	m_maxPointsCount(0)
{
	__reset();	// so that everything related to the view goes in one place.
}

CInputTransformer::~CInputTransformer()
{
}

//***********************************************************
//FUNCTION:
void Cooler::CInputTransformer::__reset()
{
	m_xVel_kb = 0;
	m_zVel_kb = 0;
	m_zVel_mouse = 0;
	m_xVel_gp = 0;
	m_zVel_gp = 0;
	for (int32_t i = 0; i < (int32_t)SCameraXformType::COUNT; i++) {
		m_xforms[i].m_translateVel = glm::vec3(0.0f, 0.0f, 0.0f);
		m_xforms[i].m_rotateVel = glm::vec3(0.0f, 0.0f, 0.0f);
		m_xforms[i].m_translate = glm::vec3(0.0f, 0.0f, 0.0f);
		m_xforms[i].m_rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		m_xforms[i].m_scale = 1.0f;
		m_xforms[i].m_dscale = 1.0f;
		m_xforms[i].m_maxRotationVel = glm::pi<float>();
		m_xforms[i].m_maxTranslationVel = 5.0f;
	}
}

//***********************************************************
//FUNCTION:
glm::mat4 Cooler::CInputTransformer::getModelViewMat(SCameraXformType::Enum xform /*= SCameraXformType::MAIN*/)
{
	Transform& xf = m_xforms[xform];
	if (m_motionMode == SCameraMotionType::FIRST_PERSON) 
	{
		return xf.m_rotateMat * xf.m_translateMat * xf.m_scaleMat;
	}
	else
	{
		return xf.m_translateMat * xf.m_rotateMat * xf.m_scaleMat;
	}

// 	if (m_motionMode == SCameraMotionType::FIRST_PERSON) 
// 	{
// 		return xf.m_scaleMat * xf.m_translateMat * xf.m_rotateMat;
// 	}
// 	else
// 	{
// 		return xf.m_scaleMat * xf.m_rotateMat * xf.m_translateMat;
// 	}
}

//***********************************************************
//FUNCTION:
bool Cooler::CInputTransformer::processPointer(SInputDeviceType::Enum vDevice, SPointerActionType::Enum vAction, uint32_t vModifiers, int32_t vCount, CPointerEvent* vPoints)
{
	Transform& xfm = m_xforms[SCameraXformType::MAIN];
	Transform& xfs = m_xforms[SCameraXformType::SECONDARY];
	float x = vPoints[0].m_x;
	float y = vPoints[0].m_y;
	uint32_t button = vPoints[0].m_id;
	bool needsUpdate = false;

	if (vAction == SPointerActionType::UP)
	{
		// if count == 0, it's 'cancel' with no 'release' location
		if (vCount == 0)
		{

		}
		else
		{

		}
		m_touchDown = false;
		m_maxPointsCount = 0;
		// lock in scaling
		if (m_motionMode != SCameraMotionType::FIRST_PERSON)
		{
			xfm.m_scale = NV_CLAMP_SCALE(xfm.m_scale*xfm.m_dscale);
			xfm.m_dscale = 1.0f;
		}
	}//end up
 	else if (vAction == SPointerActionType::MOTION)
	{
 		if (m_touchDown) 
		{
 			if (m_motionMode == SCameraMotionType::FIRST_PERSON)
			{
 				xfm.m_rotate.x += ((y - m_lastInput.y) / (float)m_height) * xfm.m_maxRotationVel;
 				xfm.m_rotate.y += ((x - m_lastInput.x) / (float)m_width) * xfm.m_maxRotationVel;
 				needsUpdate = true;
 			}
 			else if (m_maxPointsCount == 1) 
			{
 				switch (m_mode)
				{
 				case ROTATE:
					xfm.m_rotate.x += ((y - m_lastInput.y) / (float)m_height) * xfm.m_maxRotationVel;
					xfm.m_rotate.y += ((x - m_lastInput.x) / (float)m_width) * xfm.m_maxRotationVel;
					needsUpdate = true;
 					break;
 				//case SECONDARY_ROTATE:
 				//	xfs.m_rotate.x += ((y - m_lastInput.y) / (float)m_height) * xfs.m_maxRotationVel;
 				//	xfs.m_rotate.y += ((x - m_lastInput.x) / (float)m_width) * xfs.m_maxRotationVel;
 				//	needsUpdate = true;
 				//	break;
  				case TRANSLATE:
  					xfm.m_translate.x += ((x - m_lastInput.x) / (float)m_width) * xfm.m_maxTranslationVel;
  					xfm.m_translate.y -= ((y - m_lastInput.y) / (float)m_height)* xfm.m_maxTranslationVel;
  					needsUpdate = true;
  					break;
 				case ZOOM:
				{
 					float dy = y - m_firstInput.y; // negative for moving up, positive moving down.
 					if (dy > 0) // scale up...
 						xfm.m_dscale = 1.0f + dy / (m_height / 16.0f);
 					else if (dy < 0) 
					{ // scale down...
 						xfm.m_dscale = 1.0f - (-dy / (m_height / 2));
 						xfm.m_dscale *= xfm.m_dscale; // accelerate the shrink...
 					}
 					xfm.m_dscale = NV_CLAMP_SCALE(xfm.m_dscale);
 					needsUpdate = true;
 					break;
 				}
 				default:
 					break;
 				}
 			}
 			else
			{ 
				// >1 points during this 'gesture'
//  				if (m_maxPointsCount == 2 && count == 2) {
//  					if (m_motionMode == NvCameraMotionType::DUAL_ORBITAL) {
//  						xfs.m_rotate.x += ((y - m_lastInput.y) / (float)m_height) * xfs.m_maxRotationVel;
//  						xfs.m_rotate.y += ((x - m_lastInput.x) / (float)m_width) * xfs.m_maxRotationVel;
//  						needsUpdate = true;
//  					}
//  					else if (m_motionMode != NvCameraMotionType::FIRST_PERSON) {
//  						// calc pinch dx,dy.
//  						float dx = points[0].m_x - points[1].m_x;
//  						float dy = points[0].m_y - points[1].m_y;
//  
//  						// first, handle setting scale values.
//  						// firstInput holds gesture-start dx,dy
//  						nv::vec2f curr = nv::vec2f(points[0].m_x - points[1].m_x, points[0].m_y - points[1].m_y);
//  						xfm.m_dscale = nv::length(curr) / nv::length(m_firstInput);
//  						xfm.m_dscale = NV_CLAMP_SCALE(xfm.m_dscale);
//  
//  						// second, handle translation, calc new center of pinch
//  						// lastInput handles prior move x/y centerpoint.
//  						curr = nv::vec2f(points[0].m_x - (dx / 2), points[0].m_y - (dy / 2));
//  						xfm.m_translate.x += ((curr.x - m_lastInput.x) / (float)m_width) * xfm.m_maxTranslationVel;
//  						xfm.m_translate.y -= ((curr.y - m_lastInput.y) / (float)m_height)* xfm.m_maxTranslationVel;
//  						LOGI("old center = %0.2f,%0.2f :: new center = %0.2f,%0.2f", m_lastInput.x, m_lastInput.y, curr.x, curr.y);
//  						LOGI("trans = %0.2f,%0.2f", xfm.m_translate.x, xfm.m_translate.y);
//  						m_lastInput = curr; // cache current center.
//  
//  						needsUpdate = true;
//  					}
//  				}
  			}
 		}
 	}//end motion
	else 
	{ 
		// down or extra_down or extra_up
		if (vAction == SPointerActionType::DOWN)
		{
			m_touchDown = true;
			m_maxPointsCount = 1;
			xfm.m_dscale = 1.0f; // for sanity reset to 1.

			if (m_motionMode == SCameraMotionType::PAN_ZOOM)
				m_mode = TRANSLATE;
			else
				m_mode = ROTATE;

 			if (m_motionMode != SCameraMotionType::FIRST_PERSON)
			{
 				if (vDevice == SInputDeviceType::MOUSE) 
				{
 					if (m_motionMode == SCameraMotionType::ORBITAL)
					{
 						if (button & SMouseButton::MIDDLE)
 							m_mode = ZOOM;
 						else if (button & SMouseButton::RIGHT)
 							m_mode = TRANSLATE;
 					}
 					else if (m_motionMode == SCameraMotionType::DUAL_ORBITAL)
					{
 						if (button & SMouseButton::LEFT)
 							m_mode = ROTATE;
 						else if (button & SMouseButton::RIGHT)
 							m_mode = SECONDARY_ROTATE;
 					}
 					else
					{ // PAN_ZOOM
 						if (button & SMouseButton::RIGHT)
 							m_mode = ZOOM;
 					}
 				}
 			}
 			m_firstInput = glm::vec2(vPoints[0].m_x, vPoints[0].m_y);
		}
		else if (vAction == SPointerActionType::EXTRA_DOWN)
		{
// 			m_maxPointsCount = (uint8_t)vCount; // cache max fingers.
// 			if ((m_motionMode != SCameraMotionType::FIRST_PERSON) && (m_maxPointsCount == 2)) {
// 				if (m_motionMode == SCameraMotionType::DUAL_ORBITAL)
// 					m_mode = SECONDARY_ROTATE;
// 				else
// 					m_mode = ZOOM;
// 				// cache starting distance across pinch
// 				float dx = vPoints[0].m_x - vPoints[1].m_x;
// 				float dy = vPoints[0].m_y - vPoints[1].m_y;
// 				m_firstInput = glm::vec2(dx, dy);
// 				// cache center of pinch.
// 				m_lastInput = glm::vec2(vPoints[0].m_x - (dx / 2), vPoints[0].m_y - (dy / 2));
// 			}
		}
		else 
		{
			// extra up.
		}
	}//end else

	if (m_motionMode == SCameraMotionType::FIRST_PERSON)
	{
		m_zVel_mouse = 0;
		if ((vCount > 1) || ((vDevice == SInputDeviceType::MOUSE) && (button & SMouseButton::MIDDLE) && m_touchDown))
			m_zVel_mouse = +1;
	}

	if ((m_maxPointsCount == 1) || (m_motionMode == SCameraMotionType::FIRST_PERSON) || (m_motionMode == SCameraMotionType::DUAL_ORBITAL)) 
	{
		m_lastInput = glm::vec2(x, y);
	}

	if (needsUpdate)
	{
		__updateMats(SCameraXformType::MAIN);
// 		if (m_motionMode == SCameraMotionType::DUAL_ORBITAL)
// 			__updateMats(SCameraXformType::SECONDARY);
	}

	return true;
}

//***********************************************************
//FUNCTION:
bool Cooler::CInputTransformer::processKey(uint32_t code, SKeyActionType::Enum action)
{
	if (m_motionMode == SCameraMotionType::FIRST_PERSON) {
		// FPS mode uses WASD for motion, so that UI can own arrows for focus control.
		switch (code) {
		case GLFW_KEY_W:
			if (action == SKeyActionType::UP) {
				if (m_zVel_kb == +1) // only turn our own value 'off'
					m_zVel_kb = 0;
			}
			else {
				m_zVel_kb = +1;
			}
			return true;
		case GLFW_KEY_S:
			if (action == SKeyActionType::UP) {
				if (m_zVel_kb == -1) // only turn our own value 'off'
					m_zVel_kb = 0;
			}
			else {
				m_zVel_kb = -1;
			}
			return true;

		case GLFW_KEY_A:
			if (action == SKeyActionType::UP) {
				if (m_xVel_kb == -1) // only turn our own value 'off'
					m_xVel_kb = 0;
			}
			else {
				m_xVel_kb = -1;
			}
			return true;
		case GLFW_KEY_D:
			if (action == SKeyActionType::UP) {
				if (m_xVel_kb == +1) // only turn our own value 'off'
					m_xVel_kb = 0;
			}
			else {
				m_xVel_kb = +1;
			}
			return true;
		}
	}

	return false;
}

//***********************************************************
//FUNCTION:
void Cooler::CInputTransformer::update(float deltaTime)
{
	if (m_motionMode == SCameraMotionType::DUAL_ORBITAL)
	{
// 		Transform& xfm = m_xforms[SCameraXformType::MAIN];
// 		Transform& xfs = m_xforms[SCameraXformType::SECONDARY];
// 		xfm.m_rotate += xfm.m_rotateVel*deltaTime;
// 		xfs.m_rotate += xfs.m_rotateVel*deltaTime;
// 		xfm.m_translate += xfm.m_translateVel * deltaTime;
// 
// 		__updateMats(SCameraXformType::MAIN);
// 		__updateMats(SCameraXformType::SECONDARY);
	}
	else
	{
		Transform& xf = m_xforms[SCameraXformType::MAIN];
		xf.m_rotate += xf.m_rotateVel*deltaTime;
		glm::vec3 transVel;
		if (m_motionMode == SCameraMotionType::FIRST_PERSON)
		{
			// obviously, this should clamp to [-1,1] for the mul, but we don't care for sample use.
			xf.m_translateVel.x = xf.m_maxTranslationVel * (m_xVel_kb + m_xVel_gp);
			xf.m_translateVel.z = xf.m_maxTranslationVel * (m_zVel_mouse + m_zVel_kb + m_zVel_gp);
			transVel = glm::vec3(glm::transpose(xf.m_rotateMat) * glm::vec4(-xf.m_translateVel.x, xf.m_translateVel.y, xf.m_translateVel.z, 0.0f));
		}
		else
		{
			transVel = xf.m_translateVel;
		}

		xf.m_translate += transVel * deltaTime;

		__updateMats(SCameraXformType::MAIN);
	}
}

//***********************************************************
//FUNCTION:
void Cooler::CInputTransformer::__updateMats(SCameraXformType::Enum xform)
{
	Transform& xf = m_xforms[xform];
	__rotationYawPitchRoll(xf.m_rotateMat, xf.m_rotate.y, xf.m_rotate.x, 0.0f);
	xf.m_translateMat = glm::translate(glm::mat4(1.0), xf.m_translate);
	float ScaleFactor = NV_CLAMP_SCALE(xf.m_scale*xf.m_dscale);
	xf.m_scaleMat = glm::scale(glm::mat4(1.0), glm::vec3(ScaleFactor));
}

//***********************************************************
//FUNCTION:YPR 注意外部需转换为弧度
void Cooler::CInputTransformer::__rotationYawPitchRoll(glm::mat4& voMatrix, const float vYaw, const float vPitch, const float vRoll)
{
	voMatrix = glm::mat4(1.0);
	glm::mat4 YawMatrix, PitchMatrix, RollMatrix;

	if (vRoll)
	{
		RollMatrix = glm::rotate(glm::mat4(1.0), vRoll, glm::vec3(0.0, 0.0, 1.0));
	}
	if (vPitch)
	{
		PitchMatrix = glm::rotate(glm::mat4(1.0), vPitch, glm::vec3(1.0, 0.0, 0.0));
	}
	if (vYaw)
	{
		YawMatrix = glm::rotate(glm::mat4(1.0), vYaw, glm::vec3(0.0, 1.0, 0.0));
	}

	voMatrix = YawMatrix * PitchMatrix * RollMatrix;
}