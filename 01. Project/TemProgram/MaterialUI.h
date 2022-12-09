#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{
private:
    TEX_PARAM   m_eSelectTexParam;

private:
    virtual void update() override;
    virtual void render_update() override;

private:
    //void SetTexture(DWORD_PTR _strTexKey);

public:
    MaterialUI();
    ~MaterialUI();
};

