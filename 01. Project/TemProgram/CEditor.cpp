#include "pch.h"
#include "CEditor.h"

#include "imgui.h"
#include "CGameObjectEX.h"
#include <Engine\CTransform.h>
#include <Engine\CMeshRender.h>
#include <Engine\CGrid2DScript.h>
#include <Engine\CCamera.h>

#include <Engine\CRenderMgr.h>

CEditor::CEditor()
{
}

CEditor::~CEditor()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugDrawObject);
}

void CEditor::init()
{
	CreateDebugDrawObject();

	// Editor 용도 Grid Object 추가
	CGameObjectEX* pGridObj = new CGameObjectEX;
	pGridObj->SetName(L"Grid Object");

	pGridObj->AddComponent(new CTransform);
	pGridObj->AddComponent(new CMeshRender);
	pGridObj->AddComponent(new CGrid2DScript);

	pGridObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pGridObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GridMtrl"));

	pGridObj->GetScript<CGrid2DScript>()->SetGridColor(Vec4(0.2f, 0.9f, 0.2f, 1.f));
	pGridObj->GetScript<CGrid2DScript>()->SetGridInterval(100.f);
	pGridObj->GetScript<CGrid2DScript>()->SetThickness(2.f);

	m_vecEditorObj.push_back(pGridObj);
}

void CEditor::progress()
{
	tick();

	render();
}

void CEditor::tick()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); i++)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditor::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}


	// DebugDrawRender
	// 순서를 list에있는 물체를 그리고 새로들어온 것을 그려주도록 잡은 이유는
	// 반대로 할경우 새로들어온 물체를 그리고, 유지시간이 있다면, 새로 추가해준 뒤, 다시 한번 더 그리기 때문이다.
	// 일정 시간동안 렌더링 되는 Shape
	list<tDebugShapeInfo>::iterator iter = m_DebugDrawList.begin();
	for (; iter != m_DebugDrawList.end();)
	{
		iter->fCurtime += DT;
		if (iter->fDuration < iter->fCurtime)
		{
			iter = m_DebugDrawList.erase(iter);
		}
		else
		{
			DebugDraw(*iter);
			++iter;
		}
	}
		
	// 새로 추가된 DebugShape 확인
	vector<tDebugShapeInfo>& vecinfo = CRenderMgr::GetInst()->GetDebutgDrawInfo();
	
	for (size_t i = 0; i < vecinfo.size(); ++i)
	{
		DebugDraw(vecinfo[i]);
	
		if (0.f < vecinfo[i].fDuration)
		{
			m_DebugDrawList.push_back(vecinfo[i]);
		}
	}
	vecinfo.clear();
}

void CEditor::CreateDebugDrawObject()
{
	CGameObjectEX* pDebugObj = nullptr;

	// DEBUG_SHAPE::RECT
	pDebugObj = new CGameObjectEX;

	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"));

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::RECT] = pDebugObj;

	// DEBUG_SHAPE::CIRCLE
	pDebugObj = new CGameObjectEX;
	
	pDebugObj->AddComponent(new CTransform);
	pDebugObj->AddComponent(new CMeshRender);

	pDebugObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	pDebugObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugDrawMtrl"));

	m_DebugDrawObject[(UINT)DEBUG_SHAPE::CIRCLE] = pDebugObj;
}

void CEditor::DebugDraw(tDebugShapeInfo& _info)
{
	CGameObjectEX* pDebugObj = m_DebugDrawObject[(UINT)_info.eShape];

	pDebugObj->Transform()->SetRelativePos(_info.vPosition);
	pDebugObj->Transform()->SetRelativeRotation(_info.vRot);

	if (DEBUG_SHAPE::CIRCLE == _info.eShape)
	{
		pDebugObj->Transform()->SetRelativeScale(Vec3(_info.fRadius, _info.fRadius, _info.fRadius));
	}
	if (DEBUG_SHAPE::RECT == _info.eShape)
	{
		pDebugObj->Transform()->SetRelativeScale(_info.vScale);
	}

	pDebugObj->MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, _info.vColor);

	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();

	pDebugObj->Transform()->finaltick();

	g_transform.matWorld = pDebugObj->Transform()->GetWorldMat();
	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();

	pDebugObj->render();
}


// Simple Meth에 해당 함수들이 선언만 되어있고 정의가 되어있지 않아서 정의를 해주었다.
void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

// 컨버젼 함수 구현을 해주었다.
// 컨버젼 함수 문법은 조금 다르기 때문에 봐둘 것. (SimpleMeth 내에서 구현하는 것도 다르다.)
Vec2::operator ImVec2() const { return ImVec2(x, y); }