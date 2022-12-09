#pragma once
#include "CComponent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "GlobalComponent.h"

class CScript :
    public CComponent
{
private:
    int m_iScriptType;

public:
    virtual void tick() = 0;
    virtual void finaltick() final {};

public:
    virtual void BeginOverlap(CCollider2D* _pOther) {}
    virtual void Overlap(CCollider2D* _pOther) {}
    virtual void EndOverlap(CCollider2D* _pOther) {}

public:
    virtual CScript* Clone() = 0;

public:
    CScript();
    ~CScript();
};

