#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	// 애니메이션이 있다면 동작하고 없다면 동작하지 않는다.
	if (!IsValid(m_pCurAnim))
	{
		return;
	}

	// 애니메이션이 Finish 상태이고, 반복 재생일 경우
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{
		// 다시 0프레임으로 리셋시켜 동작하게 한다.
		m_pCurAnim->Reset();
	}

	// 애니메이션 업데이트
	m_pCurAnim->finaltick();
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	// 아틀라스 텍스처가 있는지 확인
	assert(_AtlasTex.Get());

	// 인자로 받은 키값으로 이미 애니메이션이 존재하는지 확인
	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);

	// 새로운 애니메이션 추가
	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}


void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm)
{
	// 아틀라스 텍스처가 있는지 확인
	assert(_AtlasTex.Get());

	// 인자로 받은 키값으로 이미 애니메이션이 존재하는지 확인
	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);
	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vecFrm);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
	// 실행할 애니메이션을 찾기
	CAnimation2D* pCurAnim = FindAnimation(_strKey);

	if (IsValid(pCurAnim))
	{
		m_pCurAnim = pCurAnim;
	}

	// 반복 여부 저장
	m_bRepeat = _bRepeat;

	// 실행할 애니메이션 리셋
	m_pCurAnim->Reset();
}

void CAnimator2D::UpdateData()
{
	if (!IsValid(m_pCurAnim))
		return;

	m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	if (!IsValid(m_pCurAnim))
	{
		return;
	}

	m_pCurAnim->Clear();
}
