#pragma once
#include <string>

namespace Cooler
{
	const std::string OUTPUT_EVENT_2_SCREEN = "OUTPUT_2_SCREEN";
	const std::string OUTPUT_EVENT_2_FILE   = "OUTPUT_2_FILE";

	enum EEventLevel
	{
		DEBUG_LEVEL = 0,
		INFO_LEVEL = 1,
		WARNING_LEVEL = 2,
	};

	enum EDataType
	{
		GRAPHICS_UNKNOWN = 0,
		GRAPHICS_INT,
		GRAPHICS_FLOAT,
		GRAPHICS_DOUBLE,
		GRAPHICS_BOOL,
		GRAPHICS_STRING,
		GRAPHICS_VECSTRING,
		GRAPHICS_VEC2,
		GRAPHICS_VEC3,
		GRAPHICS_VEC4,
		GRAPHICS_SUBCONFIG
	};

	namespace DEFAULT_CONFIG
	{
		const std::string SCENE_PATH = "Scene.xml";
		const std::string EFFECT_PATH = "Effect.xml";
		const std::string RESOURCE_PATH = "Resource.xml";
		const std::string MODEL_PATH = ""; 

		const std::string DEFAULT_WIN_TITLE = "Default Title";
		const int DEFAULT_WIN_WIDTH = 1280;
		const int DEFAULT_WIN_HEIGHT = 720;
	}

	namespace CONFIG_KEYWORD
	{
		const std::string SCENE = "SCENE";

		const std::string EFFECT = "EFFECT";
		const std::string EFFECT_STATE = "EFFECT_STATE";

		const std::string WINDOW = "WINDOW";
		const std::string WINDOW_TITLE = "WIN_TITLE";
		const std::string WINDOW_WIDTH = "WIN_WIDTH";
		const std::string WINDOW_HEIGHT = "WIN_HEIGHT";
		const std::string WIN_TWEAKBAR = "WIN_TWEAKBAR";

		const std::string SHADER = "SHADER";
		const std::string VERTEX_SHADER = "VERTEX_SHADER";
		const std::string FRAGMENT_SHADER = "FRAGMENT_SHADER";
		const std::string GEOMETRY_SHADER = "GEOMETRY_SHADER";
		const std::string COMPUTE_SHADER = "COMPUTE_SHADER";

		const std::string TEXTURE = "TEXTURE";
		const std::string TEXTURE_PATH = "TEXTURE_PATH";
		const std::string TEXTURE_TARGET = "TEXTURE_TARGET";
		const std::string TEXTURE_SIZE = "TEXTURE_SIZE";
		const std::string TEXTURE_WRAP = "TEXTURE_WRAP";
		const std::string TEXTURE_FILTER = "TEXTURE_FILTER";
		const std::string TEXTURE_TYPE = "TEXTURE_TYPE";
		const std::string TEXTURE_FORMAT = "TEXTURE_FORMAT";
		const std::string TEXTURE_INTERNAL_FORMAT = "TEXTURE_INTERNAL_FORMAT";

		const std::string FRAMEBUFFER = "FRAMEBUFFER";
		const std::string FBO_SIG = "FBO_SIG";
		const std::string FBO_SIZE = "FBO_SIZE";
		const std::string FBO_STENCIL = "FBO_STENCIL";
		const std::string FBO_ATTACHED_TEXTURE = "FBO_ATTACHED_TEXTURE";
		const std::string FBO_ATTACHED_TEXTURE_0 = "FBO_ATTACHED_TEXTURE_0";
		const std::string FBO_ATTACHED_TEXTURE_1 = "FBO_ATTACHED_TEXTURE_1";
		const std::string FBO_ATTACHED_TEXTURE_2 = "FBO_ATTACHED_TEXTURE_2";

		const std::string CAMERA = "CAMERA";
		const std::string CAMERA_EYE = "EYE";
		const std::string CAMERA_CENTER = "CENTER";
		const std::string CAMERA_UP = "UP";
		const std::string CAMERA_PERSPECTIVE = "PERSPECTIVE";

		const std::string MODEL = "MODEL";
		const std::string MODEL_FILE = "MODEL_FILE";
	}
}