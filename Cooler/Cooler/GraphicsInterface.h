#pragma once
#include <string>
#include <boost/any.hpp>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>
#include <AntTweakBar/AntTweakBar.h>
#include "GraphicsExport.h"

namespace Cooler
{
	class CBaseProduct;
	class CTexture;
	class CCamera;
	class CInputTransformer;
	class CFBO;

	COOLER_COMMON_DLL_EXPORT void initEngine();
	COOLER_COMMON_DLL_EXPORT void runEngine();

	COOLER_COMMON_DLL_EXPORT	CInputTransformer* fetchInputTransformer();
	COOLER_COMMON_DLL_EXPORT	TwBar* fetchTweakBar();
	COOLER_COMMON_DLL_EXPORT  CFBO* fetchFBO(const std::string& vFBOName);

	COOLER_COMMON_DLL_EXPORT CBaseProduct* getOrCreateProduct(const std::string& vSig);
	COOLER_COMMON_DLL_EXPORT CBaseProduct* createProduct(const std::string& vSig);
	COOLER_COMMON_DLL_EXPORT void outputWarning(const std::string& vSource, const std::string& vFile, int vLine, const std::string& vDescription);
	COOLER_COMMON_DLL_EXPORT void outputGraphicsError(const std::string& vFileName, unsigned int vLine, const std::string& vError);
	COOLER_COMMON_DLL_EXPORT glm::mat4 fectchCameraViewMatrix(const std::string& vCameraName);
	COOLER_COMMON_DLL_EXPORT glm::mat4 fetchCameraProjectionMatrix(const std::string& vCameraName);
	COOLER_COMMON_DLL_EXPORT void graphicsRenderModel(const std::string& vModelName);

	CTexture* graphicsFindTexture(const std::string& vTexName);
	CCamera* graphicsFindCamera(const std::string& vCameraName);

	GLenum graphicsCheckError_(const char *file, int line);

	template<class T>
	bool graphicsAnyCast(const boost::any& vSrc, T& voResult)
	{
		try
		{
			voResult = boost::any_cast<T>(vSrc);
			return true;
		}
		catch (boost::bad_any_cast e)
		{
#ifdef _DEBUG
			std::cout << "Bad boost::any_cast." << std::endl;
#endif
			return false;
		}
	}

	template<class T>
	T interpretString2GraphicsEnum(const std::string& vInput)
	{
		boost::any ValueHolder;
		if ("TEXTURE_1D" == vInput) { ValueHolder = GL_TEXTURE_1D; return boost::any_cast<T>(ValueHolder); }
		if ("TEXTURE_2D" == vInput) { ValueHolder = GL_TEXTURE_2D; return boost::any_cast<T>(ValueHolder); }
		if ("TEXTURE_3D" == vInput) { ValueHolder = GL_TEXTURE_3D; return boost::any_cast<T>(ValueHolder); }

		if ("GL_REPEAT" == vInput) { ValueHolder = GL_REPEAT; return boost::any_cast<T>(ValueHolder); }
		if ("GL_CLAMP_TO_EDGE" == vInput) { ValueHolder = GL_CLAMP_TO_EDGE; return boost::any_cast<T>(ValueHolder); }

		if ("GL_LINEAR"  == vInput) { ValueHolder = GL_LINEAR;  return boost::any_cast<T>(ValueHolder); }
		if ("GL_NEAREST" == vInput) { ValueHolder = GL_NEAREST; return boost::any_cast<T>(ValueHolder); }

		if ("GL_RGBA32F" == vInput) { ValueHolder = GL_RGBA32F; return boost::any_cast<T>(ValueHolder); }
		if ("GL_RGBA" == vInput)	{ ValueHolder = GL_RGBA; return boost::any_cast<T>(ValueHolder); }

// 		if ("TEXTURE_1D" == vInput) { ValueHolder = GL_TEXTURE_1D; voResult = boost::any_cast<T>(ValueHolder); return true; }
// 		if ("TEXTURE_2D" == vInput) { ValueHolder = GL_TEXTURE_2D; voResult = boost::any_cast<T>(ValueHolder); return true; }
// 		if ("TEXTURE_3D" == vInput) { ValueHolder = GL_TEXTURE_3D; voResult = boost::any_cast<T>(ValueHolder); return true; }

		ValueHolder = GL_INVALID_ENUM;
		return false;
	};

	static glm::mat4 mat4FromaiMatrix4x4(const aiMatrix4x4& vAssimpMat4)
	{
		glm::mat4 retMatrix;

		float Value[16];

		Value[0] = vAssimpMat4.a1; Value[1] = vAssimpMat4.b1; Value[2] = vAssimpMat4.c1; Value[3] = vAssimpMat4.d1;
		Value[4] = vAssimpMat4.a2; Value[5] = vAssimpMat4.b2; Value[6] = vAssimpMat4.c2; Value[7] = vAssimpMat4.d2;
		Value[8] = vAssimpMat4.a3; Value[9] = vAssimpMat4.b3; Value[10] = vAssimpMat4.c3; Value[11] = vAssimpMat4.d3;
		Value[12] = vAssimpMat4.a4; Value[13] = vAssimpMat4.b4; Value[14] = vAssimpMat4.c4; Value[15] = vAssimpMat4.d4;

		retMatrix = glm::make_mat4(Value);
		return retMatrix;
	};

	static glm::mat4 mat4FromaiMatrix3x3(const aiMatrix3x3& vAssimpMat3)
	{
		glm::mat4 retMatrix;

		float Value[16];

		Value[0] = vAssimpMat3.a1; Value[1] = vAssimpMat3.b1; Value[2] = vAssimpMat3.c1; Value[3] = 0.0;
		Value[4] = vAssimpMat3.a2; Value[5] = vAssimpMat3.b2; Value[6] = vAssimpMat3.c2; Value[7] = 0.0;
		Value[8] = vAssimpMat3.a3; Value[9] = vAssimpMat3.b3; Value[10] = vAssimpMat3.c3; Value[11] = 0.0;
		Value[12] = 0.0;			   Value[13] = 0.0;			  Value[14] = 0.0;			 Value[15] = 1.0;

		retMatrix = glm::make_mat4(Value);
		return retMatrix;
	};
}