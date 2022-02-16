#pragma once
#include "Exception.h"
#include "Resource.h"

#include "cru/common/Format.h"
#include "cru/common/String.h"

#include <memory>
#include <type_traits>

namespace cru::platform {
template <typename TTarget>
TTarget* CheckPlatform(IPlatformResource* resource,
                       const String& target_platform) {
  if (resource == nullptr) return nullptr;
  const auto result = dynamic_cast<TTarget*>(resource);
  if (result == nullptr) {
    throw UnsupportPlatformException(Format(
        u"Try to convert resource to target platform failed. Platform id of "
        "resource to convert: {} . Target platform id: {} .",
        resource->GetPlatformId(), target_platform));
  }
  return result;
}

template <typename TTarget, typename TSource>
std::shared_ptr<TTarget> CheckPlatform(const std::shared_ptr<TSource>& resource,
                                       const String& target_platform) {
  if (resource == nullptr) return nullptr;
  static_assert(std::is_base_of_v<IPlatformResource, TSource>,
                "TSource must be a subclass of INativeResource.");
  const auto result = std::dynamic_pointer_cast<TTarget>(resource);
  if (result == nullptr) {
    throw UnsupportPlatformException(Format(
        u"Try to convert resource to target platform failed. Platform id of "
        "resource to convert: {} . Target platform id: {} .",
        resource->GetPlatformId(), target_platform));
  }
  return result;
}
}  // namespace cru::platform
