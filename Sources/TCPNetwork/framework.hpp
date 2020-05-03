#pragma once

#if defined(__DLL_IMPORT)
# define API_DECLSPEC __declspec(dllimport)
#else
# define API_DECLSPEC __declspec(dllexport)
#endif
