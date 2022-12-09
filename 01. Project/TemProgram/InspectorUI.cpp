#include "pch.h"
#include "InspectorUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CLevel.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>

#include "ListUI.h"
#include "CImGuiMgr.h"

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "CameraUI.h"
#include "ParticleSystemUI.h"
#include "TileMapUI.h"
#include "Animator2DUI.h"

#include "MeshUI.h"
#include "RTextureUI.h"
#include "MaterialUI.h"

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
	, m_iCurItem(0)
	, m_bMouseCheck(false)
{
	// ComponentUI
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 120.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 80.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 280.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetSize(ImVec2(0.f, 330.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 500.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	// ResourceUI
	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MESH]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new RTextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);
}

InspectorUI::~InspectorUI()
{
}

void InspectorUI::update()
{
	if (!IsValid(m_TargetObj))
	{
		SetTargetObject(nullptr);
	}


	// LevelMgr로 부터 지금 레벨에 있는 모든 오브젝트 가져오기
	m_arrObject = CLevelMgr::GetInst()->GetCurLevel()->GetGameObjects();
	m_ObjName.clear();
	for (size_t i = 0; i < m_arrObject.size(); ++i)
	{
		m_ObjName.push_back(string(m_arrObject[i]->GetName().begin(), m_arrObject[i]->GetName().end()));
	}

	if (m_bMouseCheck)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Vec2 TemMousePos = CKeyMgr::GetInst()->GetGWMousePos();
			
			for (size_t i = 0; i < m_arrObject.size(); ++i)
			{
				float Left = m_arrObject[i]->Transform()->GetWorldPos().x - m_arrObject[i]->Transform()->GetWorldScale().x * 0.5f;
				float Right = m_arrObject[i]->Transform()->GetWorldPos().x + m_arrObject[i]->Transform()->GetWorldScale().x * 0.5f;
				float Top = m_arrObject[i]->Transform()->GetWorldPos().y + m_arrObject[i]->Transform()->GetWorldScale().y * 0.5f;
				float Bot = m_arrObject[i]->Transform()->GetWorldPos().y - m_arrObject[i]->Transform()->GetWorldScale().y * 0.5f;

				if (TemMousePos.x >= Left
					&& TemMousePos.x <= Right)
				{
					if (TemMousePos.y <= Top
						&& TemMousePos.y >= Bot)
					{
						SetTargetObject(m_arrObject[i]);
						break;
					}
				}
			}
		}
	}

	UI::update();
}

void InspectorUI::render_update()
{
	if (nullptr != m_TargetObj)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 1.f));
		ImGui::Button("Select Object");
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	
		ImGui::Text("Select Object List"); ImGui::SameLine();
		if (ImGui::Button("##SelectObjBtn", Vec2(18.f, 18.f)))
		{
			ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
			assert(pListUI);
	
			pListUI->SetItemList(m_ObjName);
			pListUI->AddDynamicDBClicked(this, (FUNC_1)&InspectorUI::SetTargetbyKey);
	
			pListUI->Open();
		}
		ImGui::SameLine(); ImGui::Text("Select Object Mouse"); ImGui::SameLine();
		ImGui::Checkbox("##CheckSelecObjMouse", &m_bMouseCheck);
	
		ImGui::Text("Now Object : "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), string(m_TargetObj->GetName().begin(), m_TargetObj->GetName().end()).c_str());
	}
}

void InspectorUI::SetTargetbyKey(DWORD_PTR _strKey)
{
	string strKey = (char*)_strKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	SetTargetObject(CLevelMgr::GetInst()->FindObjectByName((wstrKey)));
}

void InspectorUI::SetTargetObject(CGameObject* _Target)
{
	// Resource가 타겟인 상태였다면
	if (nullptr != _Target)
	{
		SetTargetResource(nullptr);
	}

	m_TargetObj = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			if (m_TargetObj != nullptr && nullptr == m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->SetTarget(nullptr);
				m_arrComUI[i]->Close();
			}
			else
			{
				m_arrComUI[i]->SetTarget(_Target);

				if (nullptr != _Target)
					m_arrComUI[i]->Open();
				else
					m_arrComUI[i]->Close();
			}
		}
	}
}

void InspectorUI::SetTargetResource(CRes* _Resource)
{
	// Object가 타겟인 상태였다면
	if (nullptr != m_TargetObj)
	{
		SetTargetObject(nullptr);
	}

	if (nullptr != _Resource)
	{
		// 기존에 가리키던 리소스가 있으면, 해당 UI를 끄고
		if (nullptr != m_TargetRes && nullptr != m_arrResUI[(UINT)m_TargetRes->GetResType()])
		{
			m_arrResUI[(UINT)m_TargetRes->GetResType()]->Close();
		}

		// 새로 지정된 리소스를 담당하는 UI를 활성화
		m_TargetRes = _Resource;
		RES_TYPE eType = m_TargetRes->GetResType();

		if (nullptr != m_arrResUI[(UINT)eType])
		{
			m_arrResUI[(UINT)eType]->SetTarget(m_TargetRes);
			m_arrResUI[(UINT)eType]->Open();
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			if (nullptr != m_arrResUI[i])
			{
				m_arrResUI[i]->SetTarget(nullptr);
				m_arrResUI[i]->Close();
			}
		}
	}
}