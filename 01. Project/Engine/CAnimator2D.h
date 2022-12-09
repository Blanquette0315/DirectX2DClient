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
    // 애니메이션 UI를 만들기 위해 추가한 함수
    const map<wstring, CAnimation2D*>& GetAllAnimation() { return m_mapAnim; }
    CAnimation2D* GetCurAnimation() { return m_pCurAnim; }

    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm);

public:
    virtual void finaltick() override;

public:
    // 인자 : 애니메이션 키값, 아틀라스 텍스처 이미지, 좌상단 좌표, 한 칸의 간격, 한 이미지 당 간격, 몇 장인지, 출력 초당 시간
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    // 애니메이션을 찾아오는 함수
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

