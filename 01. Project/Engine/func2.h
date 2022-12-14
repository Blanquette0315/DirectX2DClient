#pragma once

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	typename vector<T*>::iterator iter = _vec.begin();
	for (; iter != _vec.end(); ++iter)
	{
		if (nullptr != (*iter))
			delete (*iter);
	}
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}
	_map.clear();
}

template<typename T, int SIZE>
void Safe_Del_Array(T* (&_arr)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (nullptr != _arr[i])
			delete _arr[i];
	}
}

int GetSizeofFormat(DXGI_FORMAT _eFormat);

// 게임 오브젝트 유효성 검사
class CGameObject;
bool IsValid(CGameObject*& _pObj);

// 커포넌트용 유효성 체크
class CEntity;
inline bool IsValid(CEntity* _pCom)
{
	if (nullptr == _pCom)
		return false;
	return true;
}


// Component, Resource Type enum class 값에 따른 char와 wchar로의 변환 함수
const char* ToString(RES_TYPE);
const wchar_t* ToWString(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWString(COMPONENT_TYPE);

// ========================
// =   Event 관련 함수    =
// ========================
class CGameObject;
void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx = 0);

// ==================
// =   Debug Draw   =
// ==================
#ifdef _DEBUG
// 지속 시간을 0으로 넣어주는 이유는 Collider2D에서 매 Finaltick 마다 호출해주기 때문에 0이 들어가지 않으면 중첩이 발생한다.
void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration = 0.f);
void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration = 0.f);

//void DebugDrawCube();
//void DebugDrawSphere();
#endif