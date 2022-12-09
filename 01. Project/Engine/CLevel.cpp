#include "pch.h"
#include "CLevel.h"

#include "CLayer.h"
#include "CGameObject.h"

CLevel::CLevel()
{
	m_arrLayer[0].SetName(L"Default");

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_iLayerIdx = i;
	}
}

CLevel::~CLevel()
{
}

void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].begin();
	}
}

void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].clear();
	}

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].finaltick();
	}
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i].GetName())
		{
			return &m_arrLayer[i];
		}

		return nullptr;
	}
}

void CLevel::AddGameObject(CGameObject* _pObjet, UINT _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);

	m_arrLayer[_iLayerIdx].AddGameObject(_pObjet);
}

void CLevel::AddGameObejct(CGameObject* _pObject, const wstring& _strLayerName)
{
	CLayer* pLayer = GetLayer(_strLayerName);

	assert(pLayer);

	pLayer->AddGameObject(_pObject);
}

CGameObject* CLevel::FindObjectByName(const wstring& _name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_name == vecObjects[j]->GetName())
			{
				return vecObjects[j];
			}
		}
	}

	return nullptr;
}

void CLevel::FindObjectByName(const wstring& _name, vector<CGameObject*>& _out)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObjects = m_arrLayer[i].GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (_name == vecObjects[j]->GetName())
			{
				_out.push_back(vecObjects[j]);
			}
		}
	}
}

const vector<CGameObject*> CLevel::GetGameObjects()
{
	vector<CGameObject*> temVec = {};
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		for (size_t j = 0; j < m_arrLayer[i].GetObjects().size(); ++j)
		{
			if (IsValid(m_arrLayer[i].GetObjects()[j]))
			{
				temVec.push_back(m_arrLayer[i].GetObjects()[j]);
			}
		}
	}

	return temVec;
}

