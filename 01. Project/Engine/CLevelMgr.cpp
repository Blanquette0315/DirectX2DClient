#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "GlobalComponent.h"

#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CGrid2DScript.h"

#include "CPaintShader.h"
#include "CParticleSystem.h"
#include "CTileMap.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;

	// Layer 이름 설정
	m_pCurLevel->GetLayer(1)->SetName(L"Player");
	m_pCurLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"MonsterProjectile");

	// ComputeShader 테스트
	Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	pComputeShader->Execute();

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraScript);

	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHICS);

	m_pCurLevel->AddGameObject(pCamObj, 0);

	// Directional Light 추가
	// 보통 방향성 광원은 낮일때는 0.7, 밤에는 0.2정도를 두고 사용한다.
	// 광원은 그려질 필요가 없기 때문에 CRenderMesh는 추가하지 않는다.
	// Directional Light는 방향 정보가 필요 없기 때문에 넣지 않았다.
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(0.7f, 0.7f, 0.7f));
	//pDirLight->Light2D()->SetLightEmbient(Vec3(0.2f, 0.2f, 0.2f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	m_pCurLevel->AddGameObject(pDirLight, 0);


	// Point Light 추가
	//CGameObject* pPointLight = new CGameObject;
	//pPointLight->SetName(L"PointLight");
	//
	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);
	//
	//pPointLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));
	////DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(-200.f, 0.f, 0.f), 500.f, 100.f);
	//
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetRadius(500.f);
	//
	//m_pCurLevel->AddGameObject(pPointLight, 0);

	// Spot Light 추가
	CGameObject* pSpotLight = new CGameObject;
	pSpotLight->SetName(L"SpotLight");

	pSpotLight->AddComponent(new CTransform);
	pSpotLight->AddComponent(new CLight2D);

	pSpotLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));
	pSpotLight->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, -45.f* (XM_PI / 180)));
	DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(-200.f, 0.f, 0.f), 900.f, 100.f);

	pSpotLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pSpotLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pSpotLight->Light2D()->SetRadius(500.f);
	pSpotLight->Light2D()->SetAngle(45);

	m_pCurLevel->AddGameObject(pSpotLight, 0);


	// GameObject 초기화
	CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Test", CResMgr::GetInst()->FindRes<CTexture>(L"link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->Play(L"LeftWalk", true);

	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Player"));


	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetIgnoreParentScale(true);
	pChild->Transform()->SetRelativePos(Vec3(512.f, 0.f, 100.f));
	pChild->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));

	//pObject->AddChild(pChild);
	m_pCurLevel->AddGameObject(pChild, 1);
	m_pCurLevel->AddGameObject(pObject, 0);

	// TileMap Object
	CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(800.f, 800.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"TILEAtlas"));
	pTileMapObj->TileMap()->SetTileCount(10, 10);

	m_pCurLevel->AddGameObject(pTileMapObj, 0);


	// Particle Object
	/*CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle");
	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem(100));

	pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pParticle->ParticleSystem()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	pParticle->ParticleSystem()->SetCS((CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get());
	pParticle->ParticleSystem()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Bubbles50px"));

	pParticle->ParticleSystem()->SetMinMaxLifeTime(Vec2(0.2f, 1.f));
	pParticle->ParticleSystem()->SetMinMaxSpeed(Vec2(100.f, 300.f));
	pParticle->ParticleSystem()->Set_SE_Color(Vec4(0.24f, 0.28f, 0.8f, 1.f), Vec4(0.78f, 0.74f, 0.9f, 1.f));
	pParticle->ParticleSystem()->Set_SE_Scale(Vec4(50.f, 50.f, 1.f, 0.f), Vec4(10.f, 10.f, 1.f, 0.f));
	pParticle->ParticleSystem()->SetWorldSpawn(true);

	pParticle->ParticleSystem()->SetFrequency(5.f);
	pParticle->ParticleSystem()->SetSpawnRange(100.f);

	m_pCurLevel->AddGameObject(pParticle, 0);*/

	// PostProcess Object
	/*CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcess");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	m_pCurLevel->AddGameObject(pPostProcess, 0);*/


	// 충돌 레이어 지정
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);

	// 레벨 활성화
	m_pCurLevel->begin();
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
}

void CLevelMgr::finaltick()
{
	m_pCurLevel->finaltick();
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _name)
{
	return m_pCurLevel->FindObjectByName(_name);
}

void CLevelMgr::FindObjectByName(const wstring& _name, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_name, _out);
}