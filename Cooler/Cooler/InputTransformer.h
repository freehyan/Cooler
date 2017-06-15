#pragma once
#include <glm/glm.hpp>
#include "PlatformContext.h"
#include "GraphicsExport.h"

namespace Cooler
{
	/// Camera motion mode.
	struct SCameraMotionType {
		enum Enum {
			ORBITAL = 0,		///< Camera orbits the world origin
			FIRST_PERSON = 1,	///< Camera moves as in a 3D, first-person shooter
			PAN_ZOOM = 2,		///< Camera pans and zooms in 2D
			DUAL_ORBITAL = 3	///< Two independent orbital transforms
		};
	};

	/// Camera transform mode.
	struct SCameraXformType {
		enum Enum {
			MAIN = 0,		///< Default transform
			SECONDARY = 1, ///< Secondary transform
			COUNT = 2		///< Number of transforms
		};
	};

	class COOLER_COMMON_DLL_EXPORT CInputTransformer
	{
	public:
		CInputTransformer();
		~CInputTransformer();

		glm::mat4 getModelViewMat(SCameraXformType::Enum xform = SCameraXformType::MAIN);
		inline const glm::mat4& getRotationMat(SCameraXformType::Enum xform = SCameraXformType::MAIN) { return m_xforms[xform].m_rotateMat; }
		inline const glm::mat4& getTranslationMat(SCameraXformType::Enum xform = SCameraXformType::MAIN) { return m_xforms[xform].m_translateMat; }
		inline const glm::mat4& getScaleMat(SCameraXformType::Enum xform = SCameraXformType::MAIN) { return m_xforms[xform].m_scaleMat; }

		void setMotionMode(SCameraMotionType::Enum mode) { m_motionMode = mode; }
		void setScreenSize(int32_t width, int32_t height) { m_width = width; m_height = height; }
		void setTranslationVec(const glm::vec3& vec,SCameraXformType::Enum xform = SCameraXformType::MAIN) { m_xforms[xform].m_translate = vec; }
		void setRotationVec(const glm::vec3& vec, SCameraXformType::Enum xform = SCameraXformType::MAIN) { m_xforms[xform].m_rotate = vec; }
		void setScale(const float scale, SCameraXformType::Enum xform = SCameraXformType::MAIN) { m_xforms[xform].m_scale = scale; }

		bool processPointer(SInputDeviceType::Enum vDevice, SPointerActionType::Enum vAction, uint32_t vModifiers, int32_t vCount, CPointerEvent* vPoints);
		bool processKey(uint32_t code, SKeyActionType::Enum action);
		void update(float deltaTime);

	private:
		SCameraMotionType::Enum m_motionMode; //相机的旋转模式
		bool m_touchDown;
		uint8_t m_maxPointsCount;
		enum ControlMode { TRANSLATE = 0, ROTATE, SECONDARY_ROTATE, ZOOM } m_mode;
		
		glm::vec2 m_firstInput;
		glm::vec2 m_lastInput;

		int8_t m_xVel_kb, m_zVel_kb; //FPS模式下上下左右移动
		
		int8_t m_zVel_mouse;
		float m_xVel_gp;
		float m_zVel_gp;
	
		int32_t m_width; //窗口宽高
		int32_t m_height;

	private:
		void __reset();
		void __updateMats(SCameraXformType::Enum xform);
		void __rotationYawPitchRoll(glm::mat4& voMatrix, const float vYaw, const float vPitch, const float vRoll);

		class Transform {
		public:
			glm::vec3 m_translateVel;
			glm::vec3 m_rotateVel;
			float m_maxRotationVel;
			float m_maxTranslationVel;

			glm::vec3 m_translate;
			glm::vec3 m_rotate;
			float m_scale, m_dscale;

			glm::mat4 m_translateMat;
			glm::mat4 m_rotateMat;
			glm::mat4 m_scaleMat;
		};

		Transform m_xforms[SCameraXformType::COUNT];
	};
}