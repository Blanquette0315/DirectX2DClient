#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;

#include <Engine/ptr.h>
#include <Engine/CRes.h>
class ResUI;

class InspectorUI :
    public UI
{
private:
    CGameObject*            m_TargetObj;
    ComponentUI*            m_arrComUI[(UINT)COMPONENT_TYPE::END];

    Ptr<CRes>               m_TargetRes;
    ResUI*                  m_arrResUI[(UINT)RES_TYPE::END];

    vector<CGameObject*>    m_arrObject;
    vector<string>          m_ObjName;
    int                     m_iCurItem;
    bool                    m_bMouseCheck;

public:
    void SetTargetObject(CGameObject* _Target);
    void SetTargetResource(CRes* _Resource);
    void SetTargetbyKey(DWORD_PTR _strKey);

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

