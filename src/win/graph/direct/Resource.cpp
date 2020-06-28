#include "cru/win/graph/direct/Resource.hpp"

#include "cru/win/graph/direct/Factory.hpp"

namespace cru::platform::graph::win::direct {
DirectGraphResource::DirectGraphResource(DirectGraphFactory* factory)
    : factory_(factory) {
  Expects(factory);
}

IGraphFactory* DirectGraphResource::GetGraphFactory() { return factory_; }
}  // namespace cru::platform::graph::win::direct