#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"

CPrefab::CPrefab()
	: CRes(RES_TYPE::PREFAB)
	, m_pProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _pProto)
	: CRes(RES_TYPE::PREFAB)
	, m_pProtoObj(_pProto)
{
	if (-1 != _pProto->GetLayerIdx())
	{
		m_pProtoObj = _pProto->Clone();
	}
}

CPrefab::~CPrefab()
{
	if (nullptr != m_pProtoObj)
		delete m_pProtoObj;
}

CGameObject* CPrefab::Instantiate()
{
	if(nullptr == m_pProtoObj)
		return nullptr;

	return m_pProtoObj->Clone();
}