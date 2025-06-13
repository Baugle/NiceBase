#pragma execution_character_set("utf-8")  
#include "Renderer.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Base::Renderer
{

    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    HWND                    g_hWnd = nullptr;

    bool g_ShowMenu = false;
    WNDPROC oWndProc = nullptr;

	HWND GetHWNDFromSwapChain(IDXGISwapChain* pSwapChain)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);
		return desc.OutputWindow;
	}

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {

        // 按下 F5 切换菜单状态（放在前面处理）
        if (msg == WM_KEYUP && wParam == VK_F5) {
            g_ShowMenu = !g_ShowMenu;
            return 0; // 阻止此消息继续传递
        }

        // 菜单开启时，拦截输入
        if (g_ShowMenu)
        {
            // ImGui 自己处理的消息（例如输入框、按钮等）
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
                return true;

            // 显式拦截所有输入（防止穿透到底层窗口 CS2）
            switch (msg)
            {
            case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_CHAR:
                return true; // 阻止这些消息继续传递
            }
        }
        // 默认处理
        return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
    }


    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
    }

    bool InitRenderer(IDXGISwapChain* pSwapChain)
    {
        if (!pSwapChain) return false;
        g_pSwapChain = pSwapChain;
        g_hWnd = GetHWNDFromSwapChain(pSwapChain);
        oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
            return false;

        g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

        CreateRenderTarget();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

        ImFont* font;
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 3;
        icons_config.OversampleV = 3;

        font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());//字体设置为微软雅黑
        io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 20.0f, &icons_config, icons_ranges);
        io.Fonts->AddFontFromMemoryCompressedTTF(fa_brands_400_compressed_data, fa_brands_400_compressed_size, 20.0f, &icons_config, icons_ranges);
        return true;
    }

    void ShutdownRenderer()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupRenderTarget();

        SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);


        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
        g_pSwapChain = nullptr;
    }

    void RenderImGui()
    {
        if (!g_ShowMenu) return;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(664, 405), ImGuiCond_FirstUseEver);
        //ImGui::SetNextWindowBgAlpha(0.0f); // 透明背景（让你自己画背景）
        {
            ImGui::Begin("你好！");
            //ImGui::Begin("Vicky Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
             //   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

            ImGui::Button("测试！");

            ImGui::End();

        }
       
            
        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

}