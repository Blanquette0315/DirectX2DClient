#pragma once

#include "imgui.h"

class UI;

class CImGuiMgr
	: public CSingleton<CImGuiMgr>
{
private:
	map<string, UI*>	m_mapUI;

public:
	void RegisterUI(UI* _UI);
	UI* FindUI(const string& _name);

public:
	void init(HWND _hWnd);
	void progress();

private:
	void clear();
	void CreateUI();

public:
	CImGuiMgr();
	~CImGuiMgr();
};

