#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _eResType)
	: m_eResType(_eResType)
	, m_iRefCount(0)
{
}

CRes::CRes(const CRes& _other)
	: CEntity(_other)
	, m_eResType(_other.m_eResType)
	, m_iRefCount(0)
{
}

CRes::~CRes()
{
}
