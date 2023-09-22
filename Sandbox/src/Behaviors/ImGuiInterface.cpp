#include "ImGuiInterface.h"

#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"


ImGuiInterface::~ImGuiInterface()
{
	TerminateImGui();
}

void ImGuiInterface::Awake()
{
	InitImGui();
}

void ImGuiInterface::Start()
{
	characterAnimator = gameObject->GetComponent<NightOwl::Animator>();
}

void ImGuiInterface::Update()
{
	BeginFrame();

	if (!ImGui::Begin("Controls", &showWindow, ImGuiWindowFlags_MenuBar))
	{
		ENGINE_LOG_ERROR("ImGui Failed to Initialize");

		ImGui::End();
		EndFrame();
		return;
	}

	if (ImGui::CollapsingHeader("Animator Controls"))
	{
		const char* items[] = { "Shoved Reaction With Spin", "Start Walking", "Drunk Walking Turn", "Running Slide" };
		static int item_current = 0;
		static int previsouItem = item_current;
		ImGui::Combo("Animations", &item_current, items, IM_ARRAYSIZE(items));

		if (previsouItem != item_current)
		{
			characterAnimator->SetCurrentAnimation(items[item_current]);
			previsouItem = item_current;
		}

		if (ImGui::Button(characterAnimator->IsPlaying() ? "Pause" : "Play"))
		{
			if (characterAnimator->IsPlaying())
			{
				characterAnimator->Pause();
			}
			else
			{
				characterAnimator->Play();
			}
		}

		if (ImGui::Button("Reset"))
		{
			characterAnimator->Reset();
		}
	}

	ImGui::End();
	EndFrame();
}

void ImGuiInterface::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

	// Removing ".ini" file extension due to the CRT Guidelines
	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	auto* window = static_cast<GLFWwindow*>(NightOwl::WindowApi::GetWindow()->GetWindowHandle());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiInterface::TerminateImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiInterface::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiInterface::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
