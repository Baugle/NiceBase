#pragma once
#pragma warning(disable:4996)//针对于VS2022的安全开发生命周期（SDL）检查，屏蔽掉SDL规则错误

#include <format>

#define NAME "NiceBase"
#define PATH std::format("{}\\NiceBase\\", std::getenv("appdata"))


#include <imgui.h>