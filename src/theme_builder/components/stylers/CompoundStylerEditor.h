#pragma once
#include "StylerEditor.h"
#include "cru/common/ClonablePtr.h"
#include "cru/ui/components/PopupButton.h"
#include "cru/ui/controls/FlexLayout.h"
#include "cru/ui/style/Styler.h"

namespace cru::theme_builder::components::stylers {
class CompoundStylerEditorChild : public ui::components::Component {
 public:
  explicit CompoundStylerEditorChild(std::unique_ptr<StylerEditor>&& editor);
  ~CompoundStylerEditorChild() override;

 public:
  ui::controls::Control* GetRootControl() override { return &container_; }

  StylerEditor* GetStylerEditor() { return styler_editor_.get(); }

  IEvent<std::nullptr_t>* RemoveEvent() { return &remove_event_; }

 private:
  ui::controls::FlexLayout container_;
  ui::controls::Button remove_button_;
  ui::controls::TextBlock remove_button_text_;
  std::unique_ptr<StylerEditor> styler_editor_;

  Event<std::nullptr_t> remove_event_;
};

class CompoundStylerEditor : public StylerEditor {
 public:
  CompoundStylerEditor();
  ~CompoundStylerEditor() override;

 public:
  ClonablePtr<ui::style::CompoundStyler> GetValue();
  void SetValue(ui::style::CompoundStyler* styler, bool trigger_change = true);
  void SetValue(const ClonablePtr<ui::style::CompoundStyler>& styler,
                bool trigger_change = true) {
    SetValue(styler.get(), trigger_change);
  }

  ClonablePtr<ui::style::Styler> GetStyler() override { return GetValue(); }

  IEvent<std::nullptr_t>* ChangeEvent() { return &change_event_; }

 private:
  ui::controls::FlexLayout children_container_;
  std::vector<std::unique_ptr<CompoundStylerEditorChild>> children_;
  ui::components::PopupMenuTextButton add_child_button_;

  Event<std::nullptr_t> change_event_;
};
}  // namespace cru::theme_builder::components::stylers