#include "ImGuiInterface.h"

#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include "NightOwl/Component/Concrete/ChainIK.h"
#include "NightOwl/Core/Application/Scene.h"

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
	splineAnimator = gameObject->GetComponent<SplineAnimator>();
	chainIk = gameObject->GetComponent<NightOwl::ChainIK>();
	ikTargetController = gameObject->GetScene()->FindWithName("Target")->GetComponent<IkTargetController>();
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
		if (ImGui::Button("Reset"))
		{
			splineAnimator->Reset();
			ikTargetController->Reset();
		}

		static bool shouldRenderMesh = characterRenderer->IsVisible();
		ImGui::Checkbox("Render Mesh", &shouldRenderMesh);
		characterRenderer->SetVisible(shouldRenderMesh);

		static bool areConstraintsEnabled = chainIk->AreConstraintsEnabled();
		ImGui::Checkbox("Enable Constraints", &areConstraintsEnabled);
		chainIk->EnableConstraints(areConstraintsEnabled);
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
