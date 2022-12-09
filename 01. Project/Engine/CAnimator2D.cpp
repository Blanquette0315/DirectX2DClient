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
	// �ִϸ��̼��� �ִٸ� �����ϰ� ���ٸ� �������� �ʴ´�.
	if (!IsValid(m_pCurAnim))
	{
		return;
	}

	// �ִϸ��̼��� Finish �����̰�, �ݺ� ����� ���
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{
		// �ٽ� 0���������� ���½��� �����ϰ� �Ѵ�.
		m_pCurAnim->Reset();
	}

	// �ִϸ��̼� ������Ʈ
	m_pCurAnim->finaltick();
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	// ��Ʋ�� �ؽ�ó�� �ִ��� Ȯ��
	assert(_AtlasTex.Get());

	// ���ڷ� ���� Ű������ �̹� �ִϸ��̼��� �����ϴ��� Ȯ��
	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);

	// ���ο� �ִϸ��̼� �߰�
	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}


void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm)
{
	// ��Ʋ�� �ؽ�ó�� �ִ��� Ȯ��
	assert(_AtlasTex.Get());

	// ���ڷ� ���� Ű������ �̹� �ִϸ��̼��� �����ϴ��� Ȯ��
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
	// ������ �ִϸ��̼��� ã��
	CAnimation2D* pCurAnim = FindAnimation(_strKey);

	if (IsValid(pCurAnim))
	{
		m_pCurAnim = pCurAnim;
	}

	// �ݺ� ���� ����
	m_bRepeat = _bRepeat;

	// ������ �ִϸ��̼� ����
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
