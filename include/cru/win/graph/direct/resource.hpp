#pragma once
#include "../../win_pre_config.hpp"

#include "cru/platform/graph/resource.hpp"

#include <string_view>

namespace cru::platform::graph::win::direct {
class DirectGraphFactory;

class DirectResource : public Object, public virtual INativeResource {
 public:
  static constexpr std::string_view k_platform_id = "Windows Direct";

 protected:
  DirectResource() = default;

 public:
  CRU_DELETE_COPY(DirectResource)
  CRU_DELETE_MOVE(DirectResource)

  ~DirectResource() override = default;

 public:
  std::string_view GetPlatformId() const final { return k_platform_id; }
};

class DirectGraphResource : public DirectResource,
                            public virtual IGraphResource {
 protected:
  // Param factory can't be null.
  explicit DirectGraphResource(DirectGraphFactory* factory);

 public:
  CRU_DELETE_COPY(DirectGraphResource)
  CRU_DELETE_MOVE(DirectGraphResource)

  ~DirectGraphResource() override = default;

 public:
  IGraphFactory* GetGraphFactory() final;

 public:
  DirectGraphFactory* GetDirectFactory() const { return factory_; }

 private:
  DirectGraphFactory* factory_;
};
}  // namespace cru::platform::graph::win::direct
