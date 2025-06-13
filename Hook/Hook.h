#pragma once
#include "Renderer.h"
#include <Windows.h>
#include <MinHook.h>
#include <d3d11.h>
namespace Base::Hook {
    void SetupHooks();
    void CleanupHooks();
}
