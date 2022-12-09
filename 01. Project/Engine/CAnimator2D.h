#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;

    bool                        m_bRepeat;

public:
    // �ִϸ��̼� UI�� ����� ���� �߰��� �Լ�
    const map<wstring, CAnimation2D*>& GetAllAnimation() { return m_mapAnim; }
    CAnimation2D* GetCurAnimation() { return m_pCurAnim; }

    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm);

public:
    virtual void finaltick() override;

public:
    // ���� : �ִϸ��̼� Ű��, ��Ʋ�� �ؽ�ó �̹���, �»�� ��ǥ, �� ĭ�� ����, �� �̹��� �� ����, �� ������, ��� �ʴ� �ð�
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    // �ִϸ��̼��� ã�ƿ��� �Լ�
    CAnimation2D* FindAnimation(const wstring& _strKey);

    void Play(const wstring& _strKey, bool _bRepeat);

    void UpdateData();
    void Clear();

public:
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    ~CAnimator2D();
};

