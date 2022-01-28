#pragma once
#include "Factory.hpp"

namespace cru::platform::graphics::win::direct {
// Represents a window render target.
class CRU_WIN_GRAPHICS_DIRECT_API D2DWindowRenderTarget : public Object {
 public:
  D2DWindowRenderTarget(gsl::not_null<DirectGraphicsFactory*> factory,
                        HWND hwnd);

  CRU_DELETE_COPY(D2DWindowRenderTarget)
  CRU_DELETE_MOVE(D2DWindowRenderTarget)

  ~D2DWindowRenderTarget() override = default;

 public:
  graphics::win::direct::DirectGraphicsFactory* GetDirectFactory() const {
    return factory_;
  }

  ID2D1DeviceContext* GetD2D1DeviceContext() {
    return d2d1_device_context_.Get();
  }

  void SetDpi(float x, float y);

  // Resize the underlying buffer.
  void ResizeBuffer(int width, int height);

  // Present the data of the underlying buffer to the window.
  void Present();

 private:
  void CreateTargetBitmap();

 private:
  DirectGraphicsFactory* factory_;
  HWND hwnd_;
  Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1_device_context_;
  Microsoft::WRL::ComPtr<IDXGISwapChain1> dxgi_swap_chain_;
  Microsoft::WRL::ComPtr<ID2D1Bitmap1> target_bitmap_;
};
}  // namespace cru::platform::graphics::win::direct
