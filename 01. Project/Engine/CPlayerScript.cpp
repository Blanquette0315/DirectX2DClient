#include "pch.h"
#include "CPlayerScript.h"

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: m_fAccTime(0.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	int a = 0;
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT*100;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT*100;
	}

	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += 100.f * DT;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= 100.f * DT;
	}

	if ((KEY_PRESSED(KEY::Y)))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);
	}

	if(vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);


	int a = 0;
	if (KEY_TAP(KEY::NUM_1))
	{
		a = 0;
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetCurMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}
	}

	if (KEY_TAP(KEY::NUM_2))
	{
		a = 1;
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetCurMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}
	}

	static bool bStart = false;
	if (KEY_TAP(KEY::SPACE))
	{
		// 플레이어 위치에 미사일 생성
		Vec3 vMissilePos = Transform()->GetRelativePos();
		vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

		Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
		Instantiate(pMissilePrefab->Instantiate(), vMissilePos);

		//DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), vMissilePos, 5.f, 3.f);

		bStart = true;
	}

	if (bStart)
	{
		m_fAccTime += DT;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, &m_fAccTime);
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	_pOther->Destroy();
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}

