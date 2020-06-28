#include "cru/win/native/UiApplication.hpp"

#include "../DebugLogger.hpp"
#include "cru/common/Logger.hpp"
#include "cru/platform/Check.hpp"
#include "cru/win/graph/direct/Factory.hpp"
#include "cru/win/native/Cursor.hpp"
#include "cru/win/native/Exception.hpp"
#include "cru/win/native/GodWindow.hpp"
#include "cru/win/native/InputMethod.hpp"
#include "cru/win/native/Window.hpp"
#include "GodWindowMessage.hpp"
#include "Timer.hpp"
#include "WindowManager.hpp"

namespace cru::platform::native {
std::unique_ptr<IUiApplication> CreateUiApplication() {
  return std::make_unique<win::WinUiApplication>();
}
}  // namespace cru::platform::native

namespace cru::platform::native::win {
WinUiApplication* WinUiApplication::instance = nullptr;

WinUiApplication::WinUiApplication() {
  instance = this;

  instance_handle_ = ::GetModuleHandleW(nullptr);
  if (!instance_handle_)
    throw Win32Error("Failed to get module(instance) handle.");

  log::Logger::GetInstance()->AddSource(
      std::make_unique<::cru::platform::win::WinDebugLoggerSource>());

  graph_factory_ =
      std::make_unique<cru::platform::graph::win::direct::DirectGraphFactory>();

  god_window_ = std::make_unique<GodWindow>(this);
  timer_manager_ = std::make_unique<TimerManager>(god_window_.get());
  window_manager_ = std::make_unique<WindowManager>(this);
  cursor_manager_ = std::make_unique<WinCursorManager>();
  input_method_manager_ = std::make_unique<WinInputMethodManager>(this);
}

WinUiApplication::~WinUiApplication() { instance = nullptr; }

int WinUiApplication::Run() {
  MSG msg;
  while (GetMessageW(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  for (const auto& handler : quit_handlers_) handler();

  return static_cast<int>(msg.wParam);
}

void WinUiApplication::RequestQuit(const int quit_code) {
  ::PostQuitMessage(quit_code);
}

void WinUiApplication::AddOnQuitHandler(std::function<void()> handler) {
  quit_handlers_.push_back(std::move(handler));
}

void WinUiApplication::InvokeLater(std::function<void()> action) {
  // copy the action to a safe place
  auto p_action_copy = new std::function<void()>(std::move(action));

  if (::PostMessageW(GetGodWindow()->GetHandle(), invoke_later_message_id,
                     reinterpret_cast<WPARAM>(p_action_copy), 0) == 0)
    throw Win32Error(::GetLastError(), "InvokeLater failed to post message.");
}

long long WinUiApplication::SetTimeout(std::chrono::milliseconds milliseconds,
                                       std::function<void()> action) {
  return gsl::narrow<long long>(timer_manager_->CreateTimer(
      static_cast<UINT>(milliseconds.count()), false, std::move(action)));
}

long long WinUiApplication::SetInterval(std::chrono::milliseconds milliseconds,
                                        std::function<void()> action) {
  return gsl::narrow<long long>(timer_manager_->CreateTimer(
      static_cast<UINT>(milliseconds.count()), true, std::move(action)));
}

void WinUiApplication::CancelTimer(long long id) {
  if (id < 0) return;
  timer_manager_->KillTimer(static_cast<UINT_PTR>(id));
}

std::vector<INativeWindow*> WinUiApplication::GetAllWindow() {
  const auto&& windows = window_manager_->GetAllWindows();
  std::vector<INativeWindow*> result;
  for (const auto w : windows) {
    result.push_back(static_cast<INativeWindow*>(w));
  }
  return result;
}

std::shared_ptr<INativeWindowResolver> WinUiApplication::CreateWindow(
    INativeWindow* parent) {
  WinNativeWindow* p = nullptr;
  if (parent != nullptr) {
    p = CheckPlatform<WinNativeWindow>(parent, GetPlatformId());
  }
  return (new WinNativeWindow(this, window_manager_->GetGeneralWindowClass(),
                              WS_OVERLAPPEDWINDOW, p))
      ->GetResolver();
}

cru::platform::graph::IGraphFactory* WinUiApplication::GetGraphFactory() {
  return graph_factory_.get();
}

ICursorManager* WinUiApplication::GetCursorManager() {
  return cursor_manager_.get();
}

IInputMethodManager* WinUiApplication::GetInputMethodManager() {
  return input_method_manager_.get();
}
}  // namespace cru::platform::native::win