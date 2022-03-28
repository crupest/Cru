#include "StyleRuleSetEditor.h"
#include <algorithm>
#include <memory>
#include <optional>
#include "cru/common/Exception.h"
#include "cru/common/String.h"
#include "cru/ui/ThemeManager.h"
#include "cru/ui/controls/FlexLayout.h"
#include "cru/ui/model/IListChangeNotify.h"
#include "cru/ui/render/FlexLayoutRenderObject.h"
#include "cru/ui/style/Condition.h"
#include "cru/ui/style/Styler.h"

namespace cru::theme_builder::components {
using namespace cru::ui::controls;
StyleRuleSetEditor::StyleRuleSetEditor() {
  scroll_view_.SetChild(&container_);

  container_.SetFlexDirection(ui::render::FlexDirection::Vertical);
  container_.AddChild(&rules_layout_);
  container_.AddChild(&add_button_);

  rules_layout_.SetFlexDirection(ui::controls::FlexDirection::Vertical);

  add_button_.GetStyleRuleSet()->SetParent(
      ui::ThemeManager::GetInstance()->GetResourceStyleRuleSet(
          u"cru.theme_builder.icon-button.style"));
  add_button_.SetIconWithSvgPathDataStringResourceKey(u"icon.plus",
                                                      {0, 0, 16, 16});
  add_button_.SetPreferredSize({24, 24});
  add_button_.SetPadding(ui::Thickness(2));
  add_button_.SetIconFillColor(ui::colors::green);

  add_button_.ClickEvent()->AddSpyOnlyHandler([this] {
    auto rule_set = ui::style::StyleRule(ui::style::NoCondition::Create(),
                                         ui::style::CompoundStyler::Create({}));
    style_rule_set_->AddStyleRule(rule_set);
  });
}

StyleRuleSetEditor::~StyleRuleSetEditor() {}

void StyleRuleSetEditor::BindStyleRuleSet(
    std::shared_ptr<ui::style::StyleRuleSet> rule_set) {
  Expects(style_rule_set_ == nullptr && rule_set);
  style_rule_set_ = std::move(rule_set);
  UpdateView(style_rule_set_.get());
  style_rule_set_->ListChangeEvent()->AddHandler(
      [this](const ui::model::ListChange& change) {
        UpdateView(style_rule_set_.get(), change);
      });
}

Index StyleRuleSetEditor::IndexOfRuleEditor(StyleRuleEditor* editor) {
  auto iter =
      std::find_if(style_rule_editors_.cbegin(), style_rule_editors_.cend(),
                   [editor](const std::unique_ptr<StyleRuleEditor>& p) {
                     return p.get() == editor;
                   });
  return iter - style_rule_editors_.cbegin();
}

void StyleRuleSetEditor::UpdateView(
    ui::style::StyleRuleSet* style_rule_set,
    std::optional<ui::model::ListChange> change) {
  if (change) {
    switch (change->type) {
      case ui::model::ListChangeType::kItemAdd: {
        for (auto i = change->position; i < change->position + change->count;
             ++i) {
          const auto& rule = style_rule_set->GetStyleRule(i);
          auto style_rule_editor = std::make_unique<StyleRuleEditor>();
          style_rule_editor->SetValue(rule, false);
          style_rule_editor->RemoveEvent()->AddSpyOnlyHandler(
              [this, editor = style_rule_editor.get()] {
                style_rule_set_->RemoveStyleRule(IndexOfRuleEditor(editor));
              });
          style_rule_editor->ChangeEvent()->AddSpyOnlyHandler(
              [this, editor = style_rule_editor.get()]() {
                style_rule_set_->SetStyleRule(IndexOfRuleEditor(editor),
                                              editor->GetValue());
              });
          style_rule_editors_.insert(style_rule_editors_.cbegin() + i,
                                     std::move(style_rule_editor));
          rules_layout_.AddChildAt(style_rule_editors_.back()->GetRootControl(),
                                   i);
        }
        break;
      }
      case ui::model::ListChangeType::kItemRemove: {
        for (auto i = change->position; i < change->position + change->count;
             ++i) {
          style_rule_editors_.erase(style_rule_editors_.begin() + i);
        }
        break;
      }
      case ui::model::ListChangeType::kItemSet: {
        for (auto i = change->position; i < change->position + change->count;
             ++i) {
          const auto& rule = style_rule_set->GetStyleRule(i);
          style_rule_editors_[i]->SetValue(rule, false);
        }
        break;
      }
      case ui::model::ListChangeType::kItemMove: {
        throw Exception(u"Not supported now!");
        break;
      }
      case ui::model::ListChangeType::kClear: {
        style_rule_editors_.clear();
      }
    }
  } else {
    UpdateView(style_rule_set, ui::model::ListChange::Clear());
    UpdateView(style_rule_set,
               ui::model::ListChange::ItemAdd(0, style_rule_set->GetSize()));
  }
}
}  // namespace cru::theme_builder::components
