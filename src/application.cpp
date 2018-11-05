#include "application.h"

#include "exception.h"
#include "timer.h"
#include "ui/window.h"
#include "ui/cursor.h"
#include "graph/graph.h"
#include "ui/animations/animation.h"

namespace cru {
    constexpr auto god_window_class_name = L"GodWindowClass";
    constexpr int invoke_later_message_id = WM_USER + 2000;


    LRESULT GodWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        const auto app = Application::GetInstance();

        if (app)
        {
            const auto result = app->GetGodWindow()->HandleGodWindowMessage(hWnd, uMsg, wParam, lParam);
            if (result.has_value())
                return result.value();
            else
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
        else
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    GodWindow::GodWindow(Application* application) : application_(application)
    {
        const auto h_instance = application->GetInstanceHandle();

        god_window_class_ = std::make_unique<ui::WindowClass>(god_window_class_name, GodWndProc, h_instance);

        hwnd_ = CreateWindowEx(0,
            god_window_class_name,
            L"", 0,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr, nullptr, h_instance, nullptr
        );

        if (hwnd_ == nullptr)
            throw std::runtime_error("Failed to create window.");
    }

    GodWindow::~GodWindow()
    {
        ::DestroyWindow(hwnd_);
    }

    std::optional<LRESULT> GodWindow::HandleGodWindowMessage(HWND hwnd, int msg, WPARAM w_param, LPARAM l_param)
    {
        switch (msg)
        {
        case invoke_later_message_id:
        {
            const auto p_action = reinterpret_cast<std::function<void()>*>(w_param);
            (*p_action)();
            delete p_action;
            return 0;
        }
        case WM_TIMER:
        {
            const auto id = static_cast<UINT_PTR>(w_param);
            const auto action = application_->GetTimerManager()->GetAction(id);
            if (action.has_value())
            {
                (action.value().second)();
                if (!action.value().first)
                    Application::GetInstance()->GetTimerManager()->KillTimer(id);
                return 0;
            }
            break;
        }
        default:
            return std::nullopt;
        }
        return std::nullopt;
    }



    Application* Application::instance_ = nullptr;

    Application * Application::GetInstance() {
        return instance_;
    }

    namespace
    {
        CaretInfo GetSystemCaretInfo()
        {
            CaretInfo caret_info;
            caret_info.caret_blink_duration = std::chrono::milliseconds(::GetCaretBlinkTime());
            DWORD caret_width;
            if (!::SystemParametersInfoW(SPI_GETCARETWIDTH, 0 , &caret_width, 0))
                throw Win32Error(::GetLastError(), "Failed to get system caret width.");
            caret_info.half_caret_width = caret_width / 2.0f;
            return caret_info;
        }

        void LoadSystemCursor(HINSTANCE h_instance)
        {
            ui::cursors::arrow = std::make_shared<ui::Cursor>(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW)), false);
            ui::cursors::hand = std::make_shared<ui::Cursor>(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_HAND)), false);
            ui::cursors::i_beam = std::make_shared<ui::Cursor>(::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_IBEAM)), false);
        }
    }

    Application::Application(HINSTANCE h_instance)
        : h_instance_(h_instance) {

        if (instance_)
            throw std::runtime_error("A application instance already exists.");

        instance_ = this;

        window_manager_ = std::make_unique<ui::WindowManager>();
        graph_manager_ = std::make_unique<graph::GraphManager>();
        timer_manager_ = std::make_unique<TimerManager>();
        animation_manager_ = std::make_unique<ui::animations::details::AnimationManager>();

        god_window_ = std::make_unique<GodWindow>(this);

#ifdef CRU_DEBUG_LAYOUT
        debug_layout_resource_.out_border_brush = graph::CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::Crimson));
        debug_layout_resource_.margin_brush = graph::CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::LightCoral, 0.25f));
        debug_layout_resource_.padding_brush = graph::CreateSolidBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue, 0.25f));
#endif

        caret_info_ = GetSystemCaretInfo();

        LoadSystemCursor(h_instance);
    }

    Application::~Application()
    {
        animation_manager_.reset();
        instance_ = nullptr;
    }

    int Application::Run()
    {
        MSG msg;

        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    }

    void Application::Quit(const int quit_code) {
        ::PostQuitMessage(quit_code);
    }

    void InvokeLater(const std::function<void()>& action) {
        //copy the action to a safe place
        auto p_action_copy = new std::function<void()>(action);

        if (PostMessageW(Application::GetInstance()->GetGodWindow()->GetHandle(), invoke_later_message_id, reinterpret_cast<WPARAM>(p_action_copy), 0) == 0)
            throw Win32Error(::GetLastError(), "InvokeLater failed to post message.");
    }
}