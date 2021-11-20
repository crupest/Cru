#pragma once
#include "Component.hpp"
#include "cru/common/Base.hpp"
#include "cru/ui/controls/Button.hpp"
#include "cru/ui/controls/Control.hpp"
#include "cru/ui/controls/FlexLayout.hpp"
#include "cru/ui/controls/Popup.hpp"
#include "cru/ui/controls/TextBlock.hpp"

#include <vector>

namespace cru::ui::components {
class MenuItem : public Component {
 public:
  MenuItem();
  explicit MenuItem(String text);

  CRU_DELETE_COPY(MenuItem)
  CRU_DELETE_MOVE(MenuItem)

  ~MenuItem();

 public:
  controls::Control* GetRootControl() override { return container_; }

  void SetText(String text);

 private:
  controls::Button* container_;
  controls::TextBlock* text_;
};

class Menu : public Component {
 public:
  Menu();

  CRU_DELETE_COPY(Menu)
  CRU_DELETE_MOVE(Menu)

  ~Menu();

 public:
  controls::Control* GetRootControl() override { return container_; }

  gsl::index GetItemCount() const {
    return static_cast<gsl::index>(items_.size());
  }

  void AddItem(Component* component) { AddItem(component, GetItemCount()); }
  void AddItem(Component* component, gsl::index index);
  Component* RemoveItem(gsl::index index);

  void AddTextItem(String text) {
    AddTextItem(std::move(text), GetItemCount());
  }
  void AddTextItem(String text, gsl::index index);

 private:
  controls::FlexLayout* container_;
  std::vector<Component*> items_;
};

class PopupMenu : public Component {
 public:
  explicit PopupMenu(controls::Control* attached_control = nullptr);

  CRU_DELETE_COPY(PopupMenu)
  CRU_DELETE_MOVE(PopupMenu)

  ~PopupMenu();

 public:
  controls::Control* GetRootControl() override;

  controls::Popup* GetPopup() { return popup_; }
  Menu* GetMenu() { return menu_; }

  void SetPosition(const Point& position);
  void Show();

 private:
  controls::Control* attached_control_;

  controls::Popup* popup_;
  Menu* menu_;
};
}  // namespace cru::ui::components
