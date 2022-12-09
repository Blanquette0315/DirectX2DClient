#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CComponent.h"
#include "CMeshRender.h"
#include "CRenderComponent.h"

#include "CScript.h"

CGameObject::CGameObject()
	: m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _origin.m_arrCom[i])
			continue;

		AddComponent(_origin.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScripts);
	Safe_Del_Vec(m_vecChild);
}


void CGameObject::begin()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	// Script
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->begin();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	// Script
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->tick();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}

	// Register Layer
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->GetLayer(m_iLayerIdx)->RegisterObject(this);
}

void CGameObject::render()
{
	if (nullptr == m_pRenderComponent)
		return;

	m_pRenderComponent->render();
}

void CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eComType = _pComponent->GetType();

	// ��ũ��Ʈ�� �ƴ� ���
	if (COMPONENT_TYPE::SCRIPT != eComType)
	{
		assert(!m_arrCom[(UINT)eComType]);

		// �Էµ� Component�� RenderComponent���
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_pComponent);
		if (nullptr != pRenderCom)
		{
			assert(!m_pRenderComponent); // render ��� Component�� �Ѱ��� ���� �� �ִ�.
			m_pRenderComponent = pRenderCom;
		}

		// GameObj�� Component�� ���θ� ����Ŵ.
		_pComponent->m_pOwnerObject = this;
		m_arrCom[(UINT)eComType] = _pComponent;
	}

	// ��ũ��Ʈ�� ���
	else
	{
		_pComponent->m_pOwnerObject = this;
		m_vecScripts.push_back((CScript*)_pComponent);
	}
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	// �ڽ����� ������ ������Ʈ�� �ֻ��� �θ� ������Ʈ��.
	if (nullptr == _pChild->GetParent())
	{
		// ���Ҽ� ���̾��� ��� ���̾� ������ ���־�� �Ѵ�.
		if (-1 == _pChild->m_iLayerIdx)
		{
			_pChild->m_iLayerIdx = m_iLayerIdx;
		}
		// ���̾ �־��ٸ� ���̾ �����ϴ� �θ� vector���� ���־�� �Ѵ�.
		else
		{
			CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(_pChild->m_iLayerIdx);
			pLayer->DeregisterObject(_pChild);
		}
	}

	// �ڽ����� ������ ������Ʈ�� �ٸ� ������Ʈ�� �ڽ� ������Ʈ���ٸ�
	else
	{
		// �ڽ��� ���� �θ���踦 �����־�� �Ѵ�.
		_pChild->DisconnectFromParent();
	}

	_pChild->m_pParent = this;
	m_vecChild.push_back(_pChild);
}


void CGameObject::DisconnectFromParent()
{
	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();

	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;

			return;
		}
	}

	// ���� �θ��� ChildVector�� ������ ���ٴ� ���� ���� �̻��ϴٴ� �̾߱��̹Ƿ� assert�� �ɾ��ش�.
	assert(nullptr);
}


#include "CEventMgr.h"
void CGameObject::Destroy()
{
	tEvent eve = {};
	eve.eType = EVENT_TYPE::DELETE_OBJECT;
	eve.wParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(eve);
}