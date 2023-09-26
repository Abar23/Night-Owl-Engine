#include "ImGuiInterface.h"

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

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
	characterRenderer = gameObject->GetComponent<NightOwl::MeshRenderer>();
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

	ImGui::SetWindowSize(ImVec2(300, 400));

	if (ImGui::CollapsingHeader("Animator Controls"))
	{
		const char* animations[] = { "Shoved Reaction With Spin", "Start Walking", "Drunk Walking Turn", "Running Slide" };
		static int currentAnimations = 0;
		static int previousItem = currentAnimations;
		ImGui::Combo("Animations", &currentAnimations, animations, IM_ARRAYSIZE(animations));

		if (previousItem != currentAnimations)
		{
			characterAnimator->SetCurrentAnimation(animations[currentAnimations]);
			previousItem = currentAnimations;
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

		static bool shouldRenderMesh = characterRenderer->IsVisible();
		ImGui::Checkbox("Render Mesh", &shouldRenderMesh);
		characterRenderer->SetVisible(shouldRenderMesh);
	}

	ImGui::End();
	EndFrame();
}

void ImGuiInterface::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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
