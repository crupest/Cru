#include "cru/win/native/GodWindow.hpp"

#include "cru/common/Logger.hpp"
#include "cru/win/native/Exception.hpp"
#include "cru/win/native/UiApplication.hpp"
#include "cru/win/native/WindowClass.hpp"

namespace cru::platform::native::win {
constexpr auto god_window_class_name = L"GodWindowClass";

LRESULT CALLBACK GodWndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  const auto app = WinUiApplication::GetInstance();

  if (app) {
    LRESULT result;
    const auto handled = app->GetGodWindow()->HandleGodWindowMessage(
        hWnd, uMsg, wParam, lParam, &result);
    if (handled)
      return result;
    else
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  } else
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

GodWindow::GodWindow(WinUiApplication* application) {
  application_ = application;

  const auto h_instance = application->GetInstanceHandle();

  god_window_class_ = std::make_unique<WindowClass>(god_window_class_name,
                                                    GodWndProc, h_instance);

  hwnd_ = CreateWindowEx(0, god_window_class_name, L"", 0, CW_USEDEFAULT,
                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                         HWND_MESSAGE, nullptr, h_instance, nullptr);

  if (hwnd_ == nullptr)
    throw Win32Error(::GetLastError(), "Failed to create god window.");
}

GodWindow::~GodWindow() {
  if (!::DestroyWindow(hwnd_)) {
    // Although this could be "safely" ignore.
    log::TagWarn(log_tag, u"Failed to destroy god window.");
  }
}

bool GodWindow::HandleGodWindowMessage(HWND hwnd, UINT msg, WPARAM w_param,
                                       LPARAM l_param, LRESULT* result) {
  WindowNativeMessageEventArgs args(
      WindowNativeMessage{hwnd, msg, w_param, l_param});
  message_event_.Raise(args);

  if (args.IsHandled()) {
    *result = args.GetResult();
    return true;
  }

  return false;
}
}  // namespace cru::platform::native::win
