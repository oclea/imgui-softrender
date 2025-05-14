#pragma once

#include <imgui/imgui.h>
#include "imgui/softrender/texture.h"

IMGUI_IMPL_API bool     ImGui_ImplSoftrender_Init(texture_base_t *screen);
IMGUI_IMPL_API void     ImGui_ImplSoftrender_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSoftrender_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplSoftrender_RenderDrawData(ImDrawData* draw_data);
