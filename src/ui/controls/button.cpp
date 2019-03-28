#include "button.hpp"

#include "ui/render/border_render_object.hpp"
#include "ui/ui_manager.hpp"

namespace cru::ui::controls {
Button::Button() {
  const auto predefined_resource =
      UiManager::GetInstance()->GetPredefineResources();
  render_object_.reset(new render::BorderRenderObject(
      predefined_resource->button_normal_border_brush));
  render_object_->SetEnabled(true);
  render_object_->SetBorderWidth(Thickness{3});
  render_object_->SetCornerRadius(render::CornerRadius{Point{10, 5}});
}

render::RenderObject* Button::GetRenderObject() const { return render_object_.get(); }

void Button::OnChildChanged(Control* old_child, Control* new_child) {
  if (old_child != nullptr) render_object_->RemoveChild(0);
  if (new_child != nullptr)
    render_object_->AddChild(new_child->GetRenderObject(), 0);
}
}  // namespace cru::ui::controls