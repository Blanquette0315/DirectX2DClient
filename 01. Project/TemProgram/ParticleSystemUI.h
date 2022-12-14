#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
private:
    int m_iMaxCount;
    int m_iAliveCount;

    Vec4 m_vStartScale;
    Vec4 m_vEndScale;

    Vec4 m_vStartColor;
    Vec4 m_vEndColor;

    Vec2 m_vMinMaxSpeed;
    Vec2 m_vMinMaxLifeTime;

    float m_fSpawnRange;

    float m_Frequency;
    bool  m_WorldSpawn;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void SetTexture(DWORD_PTR _strTextureKey);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

