#include <Windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <vector>
#include <iostream>
#include <thread>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "mem.h"
#include "globals.h"

void PlayerPosCoordConverter() {
	while (true) {
		if (globals::gui::cheats::bTracer) {
			std::vector<Vector3> playerScreenPositions;
			for (size_t i = 1; i < globals::cheatInfo::playerWorldPositions.size(); i++) {
				Vector3 screenPos;
				if (WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::playerWorldPositions[i], screenPos)) {
					playerScreenPositions.push_back(screenPos);
				}
			}
			globals::cheatInfo::playerScreenPositions = playerScreenPositions;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
}

void BonePosCoordConvert() {
	while (true) {
		if (globals::gui::cheats::bBoneEsp) {
			std::vector<BonesStruct> bonesScreenPositions;
			for (size_t i = 0; i < globals::cheatInfo::bonesWorldPositions.size(); i++) {
				BonesStruct bones;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].head, bones.head))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].neck, bones.neck))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_upperarm, bones.l_upperarm))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_upperarm, bones.r_upperarm))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_forearm, bones.l_forearm))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_forearm, bones.r_forearm))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_hand, bones.l_hand))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_hand, bones.r_hand))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].spine4, bones.spine4))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].pelvis, bones.pelvis))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_hip, bones.l_hip))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_hip, bones.r_hip))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_knee, bones.l_knee))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_knee, bones.r_knee))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].l_foot, bones.l_foot))
					continue;
				if (!WorldToScreen(globals::cheatInfo::viewMatrix, globals::cheatInfo::bonesWorldPositions[i].r_foot, bones.r_foot))
					continue;
				bonesScreenPositions.push_back(bones);
			}
			globals::cheatInfo::bonesScreenPositions = bonesScreenPositions;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
}

void RefreshEntList() {
	while (true) {
		std::vector<uintptr_t> entList = globals::offsets::GameAssembly::BaseNetworkable::GetEntList();
		std::vector<std::string> entNames;

		for (size_t i = 0; i < entList.size(); i++) {
			uintptr_t currEntTypeNamePtr = MemUtils::RPMChain<uintptr_t>(entList[i], { globals::offsets::GameAssembly::entTypePtr, globals::offsets::GameAssembly::entType1Ptr });
			std::string name = MemUtils::RPMString(currEntTypeNamePtr);
			entNames.push_back(name);
		}
		globals::cheatInfo::entNames = entNames;
		globals::cheatInfo::entList = entList;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void RefreshPlayerPos() {
	while (true) {
		std::vector<Vector3> playerPositions;
		for (size_t i = 0; i < globals::cheatInfo::entList.size(); i++) {
			if (!strcmp(globals::cheatInfo::entNames[i].c_str(), "BasePlayer")) {
				playerPositions.push_back(globals::offsets::GameAssembly::BasePlayer::PlayerModel::GetPosition(globals::cheatInfo::entList[i]));
			}
		}
		globals::cheatInfo::playerWorldPositions = playerPositions;
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

void RefreshBonePos() {
	while (true) {
		if (globals::gui::cheats::bBoneEsp) {
			std::vector<BonesStruct> bonesWorldPositions;

			for (size_t i = 1; i < globals::cheatInfo::entList.size(); i++) {
				BonesStruct bones;
				if (!strcmp(globals::cheatInfo::entNames[i].c_str(), "BasePlayer")) {
					bones.head = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::head);
					bones.neck = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::neck);
					bones.l_upperarm = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_upperarm);
					bones.r_upperarm = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_upperarm);
					bones.l_forearm = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_forearm);
					bones.r_forearm = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_forearm);
					bones.l_hand = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_hand);
					bones.r_hand = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_hand);
					bones.spine4 = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::spine4);
					bones.pelvis = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::pelvis);
					bones.l_hip = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_hip);
					bones.r_hip = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_hip);
					bones.l_knee = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_knee);
					bones.r_knee = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_knee);
					bones.l_foot = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::l_foot);
					bones.r_foot = GetBoneLocation(globals::cheatInfo::entList[i], BoneList::r_foot);
					bonesWorldPositions.push_back(bones);
				}
			}
			globals::cheatInfo::bonesWorldPositions = bonesWorldPositions;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
}

void RefreshViewMatrix() {
	while (true) {
		globals::cheatInfo::viewMatrix = globals::offsets::UnityPlayer::CameraManager::GetViewMatrix();
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0;
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	//SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);

	ProcUtils::procId = ProcUtils::GetProcessIdByName(L"RustClient.exe");
	ProcUtils::hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcUtils::procId);
	globals::offsets::GameAssembly::baseAddress = ProcUtils::GetModuleBaseAddress(ProcUtils::procId, L"GameAssembly.dll");
	globals::offsets::UnityPlayer::baseAddress = ProcUtils::GetModuleBaseAddress(ProcUtils::procId, L"UnityPlayer.dll");

	std::thread _RefreshViewMatrix(RefreshViewMatrix);
	std::thread _RefreshEntList(RefreshEntList);
	std::thread _RefreshPlayerPos(RefreshPlayerPos);
	std::thread _RefreshBonePos(RefreshBonePos);
	std::thread _PlayerPosCoordConverter(PlayerPosCoordConverter);
	std::thread _BoneCoordConverter(BonePosCoordConvert);

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";
	
	RegisterClassEx(&wc);

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	
	HWND hWnd = CreateWindowEx(
		WS_EX_TOPMOST |
		WS_EX_TRANSPARENT |
		WS_EX_LAYERED,
		wc.lpszClassName,
		L"Window",
		WS_POPUP,
		0,
		0,
		desktop.right,
		desktop.bottom,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT client_area{};
		GetClientRect(hWnd, &client_area);
		
		RECT window_area{};
		GetWindowRect(hWnd, &window_area);

		RECT diff{};
		ClientToScreen(hWnd, (LPPOINT)&diff);

		const MARGINS margins{
			window_area.left + (diff.left - window_area.left),
			window_area.top + (diff.top - window_area.top),
			client_area.right,
			client_area.bottom
		};

		DwmExtendFrameIntoClientArea(hWnd, &margins);
	}

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	ID3D11Device* device{nullptr};
	ID3D11DeviceContext* context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		2,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&context
	);

	ID3D11Texture2D* back_buffer{nullptr};
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	
	if (back_buffer) {
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else {
		return 1;
	}

	//hides from alt+tab and taskbar
	SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	{
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device, context);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14.0f);

		io.IniFilename = NULL;

		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TabActive] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	}

	bool running = true;
	
	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}
		}

		if (!running) {
			break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		
		ImGui::NewFrame();
		{
			if (GetAsyncKeyState(VK_INSERT) & 1)
			{
				globals::gui::bShowMenu = !globals::gui::bShowMenu;
				if (globals::gui::bShowMenu) {
					SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
				}
				else {
					SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TRANSPARENT);
				}
			}

			if (GetAsyncKeyState(VK_END) & 1)
			{
				break;
			}

			if (globals::gui::bShowMenu)
			{
				ImGui::SetNextWindowSize({ 300, 300 });
				if (ImGui::Begin("Rust External by synq666", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
				{
					ImGui::Checkbox("Tracer", &globals::gui::cheats::bTracer);
					ImGui::Checkbox("Bone ESP", &globals::gui::cheats::bBoneEsp);

					ImGui::End();
				}
			}

			ImGui::SetNextWindowPos({ FLT_MAX, FLT_MAX });
			if (ImGui::Begin("##Draw", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiInputTextFlags_ReadOnly | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar))
			{
				auto draw = ImGui::GetBackgroundDrawList();
				if (globals::gui::cheats::bTracer) {
					for (size_t i = 0; i < globals::cheatInfo::playerScreenPositions.size(); i++) {
						Vector3 screenPos = globals::cheatInfo::playerScreenPositions[i];
						draw->AddLine({ 960, 1080 }, { screenPos.x, screenPos.y }, IM_COL32(255, 255, 255, 255), 1.0f);
					}
				}
				if (globals::gui::cheats::bBoneEsp) {
					for (size_t i = 0; i < globals::cheatInfo::bonesScreenPositions.size(); i++) {
						BonesStruct bones = globals::cheatInfo::bonesScreenPositions[i];
						draw->AddLine({ bones.head.x, bones.head.y }, { bones.neck.x, bones.neck.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.neck.x, bones.neck.y }, { bones.spine4.x, bones.spine4.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.spine4.x, bones.spine4.y }, { bones.pelvis.x, bones.pelvis.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.neck.x, bones.neck.y }, { bones.l_upperarm.x, bones.l_upperarm.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.l_upperarm.x, bones.l_upperarm.y }, { bones.l_forearm.x, bones.l_forearm.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.l_forearm.x, bones.l_forearm.y }, { bones.l_hand.x, bones.l_hand.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.neck.x, bones.neck.y }, { bones.r_upperarm.x, bones.r_upperarm.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.r_upperarm.x, bones.r_upperarm.y }, { bones.r_forearm.x, bones.r_forearm.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.r_forearm.x, bones.r_forearm.y }, { bones.r_hand.x, bones.r_hand.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.pelvis.x, bones.pelvis.y }, { bones.l_hip.x, bones.l_hip.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.l_hip.x, bones.l_hip.y }, { bones.l_knee.x, bones.l_knee.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.l_knee.x, bones.l_knee.y }, { bones.l_foot.x, bones.l_foot.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.pelvis.x, bones.pelvis.y }, { bones.r_hip.x, bones.r_hip.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.r_hip.x, bones.r_hip.y }, { bones.r_knee.x, bones.r_knee.y }, IM_COL32(255, 255, 255, 255), 1.0f);
						draw->AddLine({ bones.r_knee.x, bones.r_knee.y }, { bones.r_foot.x, bones.r_foot.y }, IM_COL32(255, 255, 255, 255), 1.0f);
					}
				}

				ImGui::End();
			}
		}
		ImGui::Render();

		constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
		context->OMSetRenderTargets(1, &render_target_view, nullptr);
		context->ClearRenderTargetView(render_target_view, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swap_chain->Present(0, 0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	
	ImGui::DestroyContext();

	if (swap_chain) {
		swap_chain->Release();
	}

	if (context) {
		context->Release();
	}

	if (device) {
		device->Release();
	}

	if (render_target_view) {
		render_target_view->Release();
	}

	DestroyWindow(hWnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}