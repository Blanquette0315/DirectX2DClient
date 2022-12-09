#include "pch.h"
#include "CMonsterScript.h"


CMonsterScript::CMonsterScript()
	: m_pTargetObject(nullptr)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::tick()
{
	if (!IsValid(m_pTargetObject))
		return;
}