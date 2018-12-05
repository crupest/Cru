#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "pre.hpp"

#include "ui/control.hpp"

namespace cru::ui::controls
{
    class ToggleButton : public Control
    {
    public:
        static constexpr auto control_type = L"ToggleButton";

        static ToggleButton* Create()
        {
            return new ToggleButton();
        }

    protected:
        ToggleButton();

    public:
        ToggleButton(const ToggleButton& other) = delete;
        ToggleButton(ToggleButton&& other) = delete;
        ToggleButton& operator=(const ToggleButton& other) = delete;
        ToggleButton& operator=(ToggleButton&& other) = delete;
        ~ToggleButton() override = default;

        StringView GetControlType() const override final;

        bool IsPointInside(const Point& point) override;

        bool GetState() const
        {
            return state_;
        }

        void SetState(bool state);

        void Toggle();

        Event<events::ToggleEventArgs> toggle_event;

    protected:
        Size OnMeasureContent(const Size& available_size, const AdditionalMeasureInfo&) override;

    private:
        bool state_ = false;

        float current_circle_position_;

        Microsoft::WRL::ComPtr<ID2D1RoundedRectangleGeometry> frame_path_;
        Microsoft::WRL::ComPtr<ID2D1Brush> on_brush_;
        Microsoft::WRL::ComPtr<ID2D1Brush> off_brush_;
    };
}
