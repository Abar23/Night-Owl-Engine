#include "ImGuiInterface.h"

#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

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
	planarCloth = gameObject->GetComponent<NightOwl::PlanarCloth>();
	planeRenderer = gameObject->GetComponent<NightOwl::MeshRenderer>();
	targetController = gameObject->GetScene()->FindWithName("Target")->GetComponent<IkTargetController>();

	GetClothPropertyValues();
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

	if (ImGui::CollapsingHeader("Cloth Sim Controls"))
	{
		ImGui::Text("Controls");
		if (ImGui::Button("Reset"))
		{
			Reset();
			targetController->Reset();
		}

		ImGui::Checkbox("Debug Render", &debugRender);
		if (debugRender)
		{
			planeRenderer->SetVisible(false);
			planarCloth->DebugRender();
		}
		else
		{
			planeRenderer->SetVisible(true);
		}

		ImGui::Separator();
		ImGui::Text("Fix Corners");
		ImGui::Checkbox("Corner 1", &fixCornerData[0]);
		ImGui::Checkbox("Corner 2", &fixCornerData[1]);
		ImGui::Checkbox("Corner 3", &fixCornerData[2]);
		ImGui::Checkbox("Corner 4", &fixCornerData[3]);

		if (ImGui::Button("Unpin All"))
		{
			UnpinAllCorners();
		}

		if (ImGui::Button("Pin All"))
		{
			PinAllCorners();
		}

		ImGui::Separator();
		ImGui::Text("Physical Properties");
		ImGui::SliderFloat("Gravity", &gravity, -40.1f, 0.0f, "%.3f");
		ImGui::SliderFloat("Cloth Mass", &clothMass, 0.1f, 100.0f, "%.3f");
		ImGui::SliderFloat("Stretch Tolerance", &springStretchTolerance, 0.1f, 4.0f, "%.3f");

		ImGui::Separator();
		ImGui::Text("Spring Constants");
		ImGui::SliderFloat("Structural  ", &structuralSpringConstant, 0.0f, 10000.0f, "%.3f");
		ImGui::SliderFloat("Shear  ", &shearSpringConstant, 0.0f, 10000.0f, "%.3f");
		ImGui::SliderFloat("Flexion  ", &flexionSpringConstant, 0.0f, 10000.0f, "%.3f");

		ImGui::Separator();
		ImGui::Text("Damp Constants");
		ImGui::SliderFloat("Structural ", &structuralDampConstant, 0.0f, 1000.0f, "%.3f");
		ImGui::SliderFloat("Shear ", &shearDampConstant, 0.0f, 1000.0f, "%.3f");
		ImGui::SliderFloat("Flexion ", &flexionDampConstant, 0.0f, 1000.0f, "%.3f");
	}

	ImGui::End();
	EndFrame();

	planarCloth->SetSpringStretchTolerance(springStretchTolerance);

	planarCloth->SetStructuralSpringConstant(structuralSpringConstant);
	planarCloth->SetShearSpringConstant(shearSpringConstant);
	planarCloth->SetFlexionSpringConstant(flexionSpringConstant);

	planarCloth->SetStructuralDampConstant(structuralDampConstant);
	planarCloth->SetShearDampConstant(shearDampConstant);
	planarCloth->SetFlexionDampConstant(flexionDampConstant);

	planarCloth->SetMass(clothMass);

	planarCloth->SetGravity(gravity);

	for (int cornerIndex = 0; cornerIndex < fixCornerData.size(); ++cornerIndex)
	{
		planarCloth->EnableFixedCorner(cornerIndex, fixCornerData[cornerIndex]);
	}
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

void ImGuiInterface::Reset()
{
	planarCloth->Reset();

	GetClothPropertyValues();
}

void ImGuiInterface::GetClothPropertyValues()
{
	structuralSpringConstant = planarCloth->GetStructuralSpringConstant();
	shearSpringConstant = planarCloth->GetShearSpringConstant();
	flexionSpringConstant = planarCloth->GetFlexionSpringConstant();

	springStretchTolerance = planarCloth->GetSpringStretchTolerance();

	structuralDampConstant = planarCloth->GetStructuralDampConstant();
	shearDampConstant = planarCloth->GetShearDampConstant();
	flexionDampConstant = planarCloth->GetFlexionDampConstant();

	clothMass = planarCloth->GetMass();

	gravity = planarCloth->GetGravity();

	PinAllCorners();
}

void ImGuiInterface::UnpinAllCorners()
{
	std::ranges::fill(fixCornerData, false);
}

void ImGuiInterface::PinAllCorners()
{
	std::ranges::fill(fixCornerData, true);
}
