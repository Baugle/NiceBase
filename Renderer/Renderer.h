#pragma once
#include <dxgi.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windowsx.h>
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")


#include <font_awesome.hpp>//Í¼±ê×ÖÌå


namespace Base::Renderer
{
	
	bool InitRenderer(IDXGISwapChain* pSwapChain);
	void ShutdownRenderer();
	void RenderImGui();



}
