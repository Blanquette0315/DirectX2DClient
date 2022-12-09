#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UI.h"
#include "ParamUI.h"


CImGuiMgr::CImGuiMgr()
{
}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);

    clear();
}

void CImGuiMgr::init(HWND _hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // UI 생성
    CreateUI();
}

void CImGuiMgr::progress()
{
    // ImGui가 그려지기 위해 필요한 3가지 함수들
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::ParamCount = 0;

    // 이부분에 우리가 만들 창들을 입력하면 된다.
    // 이제 구조가 바뀌어서 Create할 때 만들어두고 update, render쪽에서 해준다.
    {
        // 이것은 데모 윈도우를 띄우기 위한 코드다.
        bool bTrue = true;
        ImGui::ShowDemoWindow(&bTrue);

        // 모든 UI의 업데이트를 돌려주고, render를 돌려준다.
        map<string, UI*>::iterator iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->update();
        }
        
        iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->render();
        }
    }


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"

#include "ListUI.h"
#include "TextureUI.h"
#include "TileMapTableUI.h"
#include "CurAnimEditor.h"

void CImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;

    pUI = new InspectorUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new OutlinerUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ContentUI;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new CurAnimEditor;
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    ListUI* pList = new ListUI;
    pList->SetModal(true);
    m_mapUI.insert(make_pair(pList->GetName(), pList));

    TextureUI* pTextureUI = new TextureUI;
    m_mapUI.insert(make_pair(pTextureUI->GetName(), pTextureUI));

    TileMapTableUI* pTileMapTableUI = new TileMapTableUI("TempTileTable");
    m_mapUI.insert(make_pair(pTileMapTableUI->GetName(), pTileMapTableUI));
}


void CImGuiMgr::RegisterUI(UI* _UI)
{
    UI* tempUI = FindUI(_UI->GetName());
    assert(!tempUI);

    m_mapUI.insert(make_pair(_UI->GetName(), _UI));
}

UI* CImGuiMgr::FindUI(const string& _name)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_name);

    if (iter == m_mapUI.end())
    {
        return nullptr;
    }

    return iter->second;
}