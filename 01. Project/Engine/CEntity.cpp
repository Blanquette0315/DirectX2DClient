#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_ID = 0;

CEntity::CEntity()
	: m_ID(g_ID++)
{

}

CEntity::CEntity(const CEntity& _other)
	: m_ID(g_ID++)
	, m_strName(_other.m_strName)
{

}

CEntity::~CEntity()
{

}