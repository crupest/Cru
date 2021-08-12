#pragma once
#include "cru/platform/gui/UiApplication.hpp"

#ifdef CRU_PLATFORM_WINDOWS
#ifdef CRU_PLATFORM_BOOTSTRAP_EXPORT_API
#define CRU_PLATFORM_BOOTSTRAP_API __declspec(dllexport)
#else
#define CRU_PLATFORM_BOOTSTRAP_API __declspec(dllimport)
#endif
#else
#define CRU_PLATFORM_BOOTSTRAP_API
#endif

namespace cru::platform::boostrap {
CRU_PLATFORM_BOOTSTRAP_API cru::platform::gui::IUiApplication*
CreateUiApplication();
}
