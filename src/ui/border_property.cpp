#include "border_property.h"

#include "graph/graph.h"

namespace cru::ui
{
    BorderProperty::BorderProperty(): BorderProperty(graph::CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::Black)))
    {

    }

    BorderProperty::BorderProperty(Microsoft::WRL::ComPtr<ID2D1Brush> brush): brush_(std::move(brush))
    {

    }
}