#pragma once
#include "CEntity.h"
#include "CLayer.h"

class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer m_arrLayer[MAX_LAYER];

public:
    // 시점 함수
    void begin();
    void tick();
    void finaltick();

public:
    CLayer* GetLayer(UINT _iIdx) { return &m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

    void AddGameObject(CGameObject* _pObjet, UINT _iLayerIdx);
    void AddGameObejct(CGameObject* _pObject, const wstring& _strLayerName);

    CGameObject* FindObjectByName(const wstring& _name);
    void FindObjectByName(const wstring& _name, vector<CGameObject*>& _out);

    const vector<CGameObject*> GetGameObjects();

    CLONE_DUMMY(CLevel);
 
public:
    CLevel();
    virtual ~CLevel();
};

