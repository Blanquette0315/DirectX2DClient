#pragma once

class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel* m_pCurLevel;

public:
	CLevel* GetCurLevel() { return m_pCurLevel; }

public:
	CGameObject* FindObjectByName(const wstring& _name);
	void FindObjectByName(const wstring& _name, vector<CGameObject*>& _out);

public:
	void init();
	void tick();
	void finaltick();

public:
	CLevelMgr();
	~CLevelMgr();
};

