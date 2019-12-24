#pragma once
#include "../resource.hpp"

#include <chrono>
#include <functional>
#include <vector>

namespace cru::platform::graph {
struct IGraphFactory;
}

namespace cru::platform::native {
struct INativeWindow;
struct ICursorManager;

// The entry point of a ui application.
struct IUiApplication : public virtual INativeResource {
 public:
  static IUiApplication* GetInstance() { return instance; }

 protected:
  static IUiApplication* instance;

 public:
  // Block current thread and run the message loop. Return the exit code when
  // message loop gets a quit message (possibly posted by method RequestQuit).
  virtual int Run() = 0;

  // Post a quit message with given quit code.
  virtual void RequestQuit(int quit_code) = 0;

  virtual void AddOnQuitHandler(const std::function<void()>& handler) = 0;

  virtual void InvokeLater(const std::function<void()>& action) = 0;
  virtual unsigned long SetTimeout(std::chrono::milliseconds milliseconds,
                                   const std::function<void()>& action) = 0;
  virtual unsigned long SetInterval(std::chrono::milliseconds milliseconds,
                                    const std::function<void()>& action) = 0;
  virtual void CancelTimer(unsigned long id) = 0;

  virtual std::vector<INativeWindow*> GetAllWindow() = 0;
  virtual INativeWindow* CreateWindow(INativeWindow* parent) = 0;

  virtual cru::platform::graph::IGraphFactory* GetGraphFactory() = 0;

  virtual ICursorManager* GetCursorManager() = 0;
};
}  // namespace cru::platform::native