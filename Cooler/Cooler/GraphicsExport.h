#ifdef COOLER_COMMON_EXPORTS
#define COOLER_COMMON_DLL_EXPORT __declspec(dllexport)
#else
#define COOLER_COMMON_DLL_EXPORT __declspec(dllimport)
#endif