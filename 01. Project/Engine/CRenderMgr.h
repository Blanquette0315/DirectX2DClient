#pragma once

class CCamera;
class CStructuredBuffer;
#include "CLight2D.h";
#include "CTexture.h";

class CRenderMgr
	: public CSingleton<CRenderMgr>
{
private:
	vector<CCamera*>		m_vecCam;			// 현재 레벨에 있는 모든 카메라
	vector<tLightInfo>		m_vecLight2D;		// 현재 레벨에 있는 모든 2D 광원
	CStructuredBuffer*		m_pLight2DBuffer;	// 2D 광원 정보를 전달할 구조화 버퍼

	vector<tDebugShapeInfo> m_DebugDrawInfo;

	Ptr<CTexture>			m_RTCopyTex;		// 랜더 타겟 복사 텍스처
												// PostProcess를 수행하기 위해서 랜더 타겟 텍스처가 필요한데, RenderTargetView이면서, SRV인것은 말이 안되기 때문에 복사본을 하나 만들어
												// 해당 사본이 SRV의 역할을 하고, RTT는 그대로 RTV로 바인딩을 건다.

public:
	void init();
	void tick();
	void render();

private:
	void UpdateNoiseTexture();
	void UpdateLight2D();

public:
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D->GetLightInfo()); }

	CCamera* GetMainCam()
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}

	// 렌더타겟을 카피텍스처로 복사
	void CopyRenderTarget();

	// DebugDraw는 릴리즈 버전에서는 제외 되어야 하므로 ifdef을 이용해 처리해주었다.
	void DebugDraw(DEBUG_SHAPE _eShape, Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fRadius, float _fDuration)
	{
#ifdef _DEBUG
		m_DebugDrawInfo.push_back(tDebugShapeInfo { _eShape, _vColor, _vPosition, _vScale, _vRotation, _fRadius, _fDuration, 0.f });
#endif
	}

	vector<tDebugShapeInfo>& GetDebutgDrawInfo() { return m_DebugDrawInfo; }

public:
	CRenderMgr();
	~CRenderMgr();
};

