#include "cru/ui/controls/button.hpp"
#include <memory>

#include "cru/platform/graph/brush.hpp"
#include "cru/platform/graph/graph_factory.hpp"
#include "cru/platform/native/cursor.hpp"
#include "cru/platform/native/native_window.hpp"
#include "cru/platform/native/ui_application.hpp"
#include "cru/ui/render/border_render_object.hpp"
#include "cru/ui/ui_manager.hpp"
#include "cru/ui/window.hpp"

namespace cru::ui::controls {
using platform::native::GetSystemCursor;
using platform::native::SystemCursor;

Button::Button() : click_detector_(this) {
  // const auto predefined_resource =
  //    UiManager::GetInstance()->GetPredefineResources();

  const auto factory = platform::graph::GraphFactory::GetInstance();
  border_style_.normal.brush = std::shared_ptr<platform::graph::Brush>(
      factory->CreateSolidColorBrush(Color::FromHex(0x00bfff)));
  border_style_.hover.brush = std::shared_ptr<platform::graph::Brush>(
      factory->CreateSolidColorBrush(Color::FromHex(0x47d1ff)));
  border_style_.press.brush = std::shared_ptr<platform::graph::Brush>(
      factory->CreateSolidColorBrush(Color::FromHex(0x91e4ff)));
  border_style_.press_cancel.brush = std::shared_ptr<platform::graph::Brush>(
      factory->CreateSolidColorBrush(Color::FromHex(0x91e4ff)));

  border_style_.normal.thickness = border_style_.hover.thickness =
      border_style_.press.thickness = border_style_.press_cancel.thickness =
          Thickness{3};

  border_style_.normal.corner_radius = border_style_.hover.corner_radius =
      border_style_.press.corner_radius =
          border_style_.press_cancel.corner_radius =
              render::CornerRadius{Point{10, 5}};

  render_object_.reset(new render::BorderRenderObject);
  render_object_->SetAttachedControl(this);
  render_object_->SetBorderEnabled(true);
  (*render_object_->GetBorderStyle()) = border_style_.normal;

  MouseEnterEvent()->Direct()->AddHandler([this](event::MouseEventArgs& args) {
    if (click_detector_.GetPressingButton() & trigger_button_) {
      SetState(ButtonState::Press);
    } else {
      SetState(ButtonState::Hover);
    }
  });

  MouseLeaveEvent()->Direct()->AddHandler([this](event::MouseEventArgs& args) {
    if (click_detector_.GetPressingButton() & trigger_button_) {
      SetState(ButtonState::Normal);
    } else {
      SetState(ButtonState::PressCancel);
    }
  });

  click_detector_.ClickBeginEvent()->AddHandler([this](MouseButton button) {
    if (button & trigger_button_) {
      SetState(ButtonState::Press);
    }
  });

  click_detector_.ClickEndEvent()->AddHandler([this](MouseButton button) {
    if (button & trigger_button_) {
      SetState(ButtonState::Hover);
    }
  });
}

render::RenderObject* Button::GetRenderObject() const {
  return render_object_.get();
}

void Button::OnChildChanged(Control* old_child, Control* new_child) {
  if (old_child != nullptr) render_object_->RemoveChild(0);
  if (new_child != nullptr)
    render_object_->AddChild(new_child->GetRenderObject(), 0);
}

void Button::OnStateChange(ButtonState oldState, ButtonState newState) {
  switch (newState) {
    case ButtonState::Normal:
      (*render_object_->GetBorderStyle()) = border_style_.normal;
      SetCursor(GetSystemCursor(SystemCursor::Arrow));
      break;
    case ButtonState::Hover:
      (*render_object_->GetBorderStyle()) = border_style_.hover;
      SetCursor(GetSystemCursor(SystemCursor::Hand));
      break;
    case ButtonState::Press:
      (*render_object_->GetBorderStyle()) = border_style_.press;
      SetCursor(GetSystemCursor(SystemCursor::Hand));
      break;
    case ButtonState::PressCancel:
      (*render_object_->GetBorderStyle()) = border_style_.press_cancel;
      SetCursor(GetSystemCursor(SystemCursor::Arrow));
      break;
  }
  GetWindow()->GetNativeWindow()->Repaint();
}
}  // namespace cru::ui::controls
