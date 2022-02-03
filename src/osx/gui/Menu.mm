#import "MenuPrivate.h"

#include "cru/common/platform/osx/Convert.hpp"

#import <AppKit/NSApplication.h>

namespace cru::platform::gui::osx {
using platform::osx::Convert;

namespace {
std::unique_ptr<OsxMenu> application_menu = nullptr;
}

namespace details {
OsxMenuItemPrivate::OsxMenuItemPrivate(OsxMenuItem* d) {
  d_ = d;
  sub_menu_ = new OsxMenu(d->GetUiApplication());
  handler_ = [[CruOsxMenuItemClickHandler alloc] init:this];
}

OsxMenuItemPrivate::~OsxMenuItemPrivate() { delete sub_menu_; }

void OsxMenuItemPrivate::AttachToNative(NSMenuItem* native_menu_item, bool check_submenu) {
  Expects(sub_menu_);

  menu_item_ = native_menu_item;
  [native_menu_item setTarget:handler_];
  [native_menu_item setAction:@selector(handleClick)];
  if (check_submenu && [native_menu_item hasSubmenu]) {
    sub_menu_->p_->AttachToNative([native_menu_item submenu]);
  }
}

OsxMenuPrivate::OsxMenuPrivate(OsxMenu* d) { d_ = d; }

OsxMenuPrivate::~OsxMenuPrivate() {
  for (auto item : items_) {
    delete item;
  }
}

void OsxMenuPrivate::AttachToNative(NSMenu* native_menu) {
  menu_ = native_menu;

  auto item_count = [native_menu numberOfItems];
  for (int i = 0; i < item_count; i++) {
    auto native_item = [native_menu itemAtIndex:i];
    auto item = new OsxMenuItem(d_->GetUiApplication());
    item->p_->SetParentMenu(d_);
    item->p_->AttachToNative(native_item, true);
    items_.push_back(item);
  }
}
}

OsxMenuItem::OsxMenuItem(IUiApplication* ui_application) : OsxGuiResource(ui_application) {
  p_ = new details::OsxMenuItemPrivate(this);
}

OsxMenuItem::~OsxMenuItem() { delete p_; }

String OsxMenuItem::GetTitle() { return Convert((CFStringRef)[p_->menu_item_ title]); }

void OsxMenuItem::SetTitle(String title) { [p_->menu_item_ setTitle:(NSString*)Convert(title)]; }

bool OsxMenuItem::IsEnabled() { return [p_->menu_item_ isEnabled]; }

void OsxMenuItem::SetEnabled(bool enabled) { [p_->menu_item_ setEnabled:enabled]; }

IMenu* OsxMenuItem::GetParentMenu() { return p_->parent_menu_; }

IMenu* OsxMenuItem::GetSubmenu() { return p_->sub_menu_; }

void OsxMenuItem::SetOnClickHandler(std::function<void()> handler) {
  p_->on_click_handler_ = std::move(handler);
}

OsxMenu* OsxMenu::CreateOrGetApplicationMenu(IUiApplication* ui_application) {
  if (application_menu) {
    return application_menu.get();
  }

  application_menu.reset(new OsxMenu(ui_application));
  application_menu->p_->AttachToNative([NSApp mainMenu]);

  return application_menu.get();
}

OsxMenu::OsxMenu(IUiApplication* ui_application) : OsxGuiResource(ui_application) {
  p_ = new details::OsxMenuPrivate(this);
}

OsxMenu::~OsxMenu() { delete p_; }

IMenuItem* OsxMenu::GetItemAt(int index) {
  if (index < 0 || index >= p_->items_.size()) {
    return nullptr;
  }

  return p_->items_[index];
}

int OsxMenu::GetItemCount() { return p_->items_.size(); }

IMenuItem* OsxMenu::CreateItemAt(int index) {
  if (index < 0) index = 0;
  if (index > p_->items_.size()) index = p_->items_.size();

  auto native_item = [[NSMenuItem alloc] init];
  [p_->menu_ insertItem:native_item atIndex:index];

  auto item = new OsxMenuItem(GetUiApplication());
  item->p_->SetParentMenu(this);
  item->p_->AttachToNative(native_item, false);
  p_->items_.insert(p_->items_.begin() + index, item);

  return item;
}

void OsxMenu::RemoveItemAt(int index) {
  if (index < 0 || index >= p_->items_.size()) {
    return;
  }

  auto item = p_->items_[index];
  [p_->menu_ removeItem:item->p_->GetNative()];
  p_->items_.erase(p_->items_.begin() + index);

  delete item;
}
}

@implementation CruOsxMenuItemClickHandler {
  cru::platform::gui::osx::details::OsxMenuItemPrivate* p_;
}

- (id)init:(cru::platform::gui::osx::details::OsxMenuItemPrivate*)p {
  p_ = p;
  return self;
}

- (void)handleClick:(id)sender {
  if (p_->GetOnClickHandler()) {
    p_->GetOnClickHandler()();
  }
}

@end
