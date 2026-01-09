#include <NightOwlPch.h>

#ifdef OPEN_GL
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>
#include <imgui.cpp>
#include <imgui_draw.cpp>
#include <imgui_tables.cpp>
#include <imgui_widgets.cpp>
#include <backends/imgui_impl_vulkan.cpp>
#endif
