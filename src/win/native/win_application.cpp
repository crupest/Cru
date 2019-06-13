#include "cru/win/native/win_application.hpp"

#include "cru/win/exception.hpp"
#include "cru/win/graph/win_graph_factory.hpp"
#include "cru/win/native/god_window.hpp"
#include "cru/win/native/win_native_window.hpp"
#include "god_window_message.hpp"
#include "timer.hpp"
#include "window_manager.hpp"

#include <VersionHelpers.h>
#include <cassert>

namespace cru::win::native {
namespace {
WinApplication* instance = nullptr;
}
}  // namespace cru::win::native

namespace cru::platform::native {
IUiApplication* IUiApplication::CreateInstance() {
  auto& i = ::cru::win::native::instance;  // avoid long namespace prefix
  assert(i == nullptr);
  i = new win::native::WinApplication(::GetModuleHandleW(nullptr));
  return i;
}

IUiApplication* IUiApplication::GetInstance() {
  return ::cru::win::native::instance;
}
}  // namespace cru::platform::native

namespace cru::win::native {
WinApplication* WinApplication::GetInstance() { return instance; }

WinApplication::WinApplication(HINSTANCE h_instance) : h_instance_(h_instance) {
  assert(instance == nullptr);

  if (!::IsWindows8OrGreater())
    throw std::runtime_error("Must run on Windows 8 or later.");

  const auto graph_factory = platform::graph::IGraphFactory::CreateInstance();
  graph_factory->SetAutoDelete(true);

  god_window_ = std::make_shared<GodWindow>(this);
  timer_manager_ = std::make_shared<TimerManager>(god_window_.get());
  window_manager_ = std::make_shared<WindowManager>(this);
}

WinApplication::~WinApplication() { instance = nullptr; }

int WinApplication::Run() {
  MSG msg;
  while (GetMessageW(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  for (const auto& handler : quit_handlers_) handler();

  if (auto_delete_) delete this;

  return static_cast<int>(msg.wParam);
}

void WinApplication::Quit(const int quit_code) { ::PostQuitMessage(quit_code); }

void WinApplication::AddOnQuitHandler(const std::function<void()>& handler) {
  quit_handlers_.push_back(handler);
}

void WinApplication::InvokeLater(const std::function<void()>& action) {
  // copy the action to a safe place
  auto p_action_copy = new std::function<void()>(action);

  if (PostMessageW(GetGodWindow()->GetHandle(), invoke_later_message_id,
                   reinterpret_cast<WPARAM>(p_action_copy), 0) == 0)
    throw Win32Error(::GetLastError(), "InvokeLater failed to post message.");
}

unsigned long WinApplication::SetTimeout(std::chrono::milliseconds milliseconds,
                                         const std::function<void()>& action) {
  return static_cast<unsigned long>(timer_manager_->CreateTimer(
      static_cast<UINT>(milliseconds.count()), false, action));
}

unsigned long WinApplication::SetInterval(
    std::chrono::milliseconds milliseconds,
    const std::function<void()>& action) {
  return static_cast<unsigned long>(timer_manager_->CreateTimer(
      static_cast<UINT>(milliseconds.count()), true, action));
}

void WinApplication::CancelTimer(unsigned long id) {
  timer_manager_->KillTimer(static_cast<UINT_PTR>(id));
}

std::vector<platform::native::INativeWindow*> WinApplication::GetAllWindow() {
  const auto&& windows = window_manager_->GetAllWindows();
  std::vector<platform::native::INativeWindow*> result;
  for (const auto w : windows) {
    result.push_back(static_cast<platform::native::INativeWindow*>(w));
  }
  return result;
}

platform::native::INativeWindow* WinApplication::CreateWindow(
    platform::native::INativeWindow* parent) {
  WinNativeWindow* p = nullptr;
  if (parent != nullptr) {
    p = dynamic_cast<WinNativeWindow*>(parent);
    assert(p);
  }
  return new WinNativeWindow(this, window_manager_->GetGeneralWindowClass(),
                             WS_OVERLAPPEDWINDOW, p);
}
}  // namespace cru::win::native