#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI* m_Tree;

private:
    void SetResourceToInspector(DWORD_PTR _res);

public:
    void ResetContent();

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    ContentUI();
    ~ContentUI();
};

