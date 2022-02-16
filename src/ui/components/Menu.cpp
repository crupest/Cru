#include "cru/ui/components/Menu.h"
#include <functional>
#include "cru/platform/gui/Window.h"
#include "cru/ui/ThemeManager.h"
#include "cru/ui/controls/Button.h"
#include "cru/ui/controls/Control.h"
#include "cru/ui/controls/FlexLayout.h"
#include "cru/ui/controls/Popup.h"
#include "cru/ui/controls/TextBlock.h"
#include "cru/ui/helper/ClickDetector.h"
#include "cru/ui/host/WindowHost.h"
#include "cru/ui/style/StyleRuleSet.h"

namespace cru::ui::components {
MenuItem::MenuItem() {
  container_.SetChild(&text_);
  container_.GetStyleRuleSet()->SetParent(
      ThemeManager::GetInstance()->GetResourceStyleRuleSet(u"menuitem.style"));
  container_.ClickEvent()->AddHandler([this](const helper::ClickEventArgs&) {
    if (this->on_click_) this->on_click_();
  });
}

MenuItem::MenuItem(String text) : MenuItem() { SetText(std::move(text)); }

MenuItem::~MenuItem() {}

void MenuItem::SetText(String text) { text_.SetText(std::move(text)); }

Menu::Menu() { container_.SetFlexDirection(controls::FlexDirection::Vertical); }

Menu::~Menu() {
  for (auto item : items_) {
    delete item;
  }
}

void Menu::AddItemAt(Component* item, gsl::index index) {
  Expects(index >= 0 && index <= GetItemCount());

  items_.insert(items_.cbegin() + index, item);
  container_.AddChildAt(item->GetRootControl(), index);
}

Component* Menu::RemoveItemAt(gsl::index index) {
  Expects(index >= 0 && index < GetItemCount());

  Component* item = items_[index];

  items_.erase(items_.cbegin() + index);
  container_.RemoveChildAt(index);

  return item;
}

void Menu::ClearItems() {
  for (auto item : items_) {
    item->DeleteIfDeleteByParent();
  }

  items_.clear();

  container_.ClearChildren();
}

void Menu::AddTextItemAt(String text, gsl::index index,
                         std::function<void()> on_click) {
  MenuItem* item = new MenuItem(std::move(text));
  item->SetOnClick([this, index, on_click = std::move(on_click)] {
    on_click();
    if (on_item_click_) on_item_click_(index);
  });
  item->SetDeleteByParent(true);
  AddItemAt(item, index);
}

PopupMenu::PopupMenu(controls::Control* attached_control)
    : attached_control_(attached_control), popup_(attached_control) {
  menu_.SetOnItemClick([this](Index) { this->Close(); });
  popup_.AddChildAt(menu_.GetRootControl(), 0);
}

PopupMenu::~PopupMenu() {}

controls::Control* PopupMenu::GetRootControl() { return &popup_; }

void PopupMenu::SetPosition(const Point& position) {
  popup_.GetWindowHost()->GetNativeWindow()->SetClientRect(Rect{position, {}});
}

void PopupMenu::Show() {
  popup_.GetWindowHost()->RelayoutWithSize(Size::Infinate(), true);
  auto native_window = popup_.GetWindowHost()->GetNativeWindow();
  native_window->SetVisibility(platform::gui::WindowVisibilityType::Show);
  native_window->RequestFocus();
  native_window->SetToForeground();
}

void PopupMenu::Close() { popup_.GetWindowHost()->GetNativeWindow()->Close(); }
}  // namespace cru::ui::components
