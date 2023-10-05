#include <NightOwlPch.h>

#ifdef OPEN_GL
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>
#include <ImGui/backends/imgui_impl_glfw.cpp>
#include <ImGui/backends/imgui_impl_opengl3.cpp>
#endif
