#pragma once


// 우리가 사용할 정점 정보를 담을 구조체
struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

// 광원 구조체 (해당 구조체는 2D와 3D 모두에서 사용이 가능하다.)
struct tLightInfo
{
	Vec4			vDiff;		// 난반사 광		: 광원 자체적인 색상
	Vec4			vSpec;		// 반사광 계수	: 스페큘러는 3D 광원에서 사용한다.
	Vec4			vEmb;		// 환경광		: 주변 빛에 따른 색상

	Vec4			vWorldPos;	// 광원 월드 위치
	Vec4			vWorldDir;	// 광원의 방향
	LIGHT_TYPE		iLightType;	// 광원의 타입
	float			fRadius;	// 광원 반경 (POINT, SPOT) : 빛이 도달 할 수 있는 최대 거리
	float			fAngle;		// 광원 각도 (SPOT)
	int				pad;		// 16바이트 단위로 맞추기 위한 패딩값
};

// ======================
// =   Animation 구조체  =
// ======================
// Pixel 기준의 좌표가 아닌 UV기준 좌표로 계산해 저장한 뒤 바인딩 걸어준다.
struct tAnim2DFrm
{
	Vec2  vLeftTop;
	Vec2  vSlice;
	Vec2  vOffset;
	Vec2  vFullsize;
	float fDuration;
};

// ======================
// =   Particle 구조체   =
// ======================
// 파티클 입자 하나를 설명하는 구조체
struct tParticle
{
	Vec4 vRelativePos;	// 파티클의 상대 위치
	Vec4 vDir;			// 파티클의 방향

	float fMaxTime;		// 최대 파티클 생존시간
	float fCurTime;		// 현재 파티클 생존시간
	float fSpeed;		// 파티클의 속도
	UINT  iActive;		// 파티클 활성화 여부 bool이 아닌 UINT로 한 이유는 16바이트 단위로 맞추기 위함이다.
};

struct tParticleShare
{
	UINT	iAliveCount;	// 이번 프레임에 활성화 가능한 파티클 갯수
	UINT	iPadding[3];
};

// ======================
// =   TileMap 구조체   =
// ======================

struct tTile
{
	Vec2 vLeftTop; // UV
	Vec2 vSlice;
};

// ======================
// =  Debug Shape Info  =
// ======================
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration; // 1프레임만 유지하고 사라지면 안되기 때문에 시간을 넣어주었다.
	float		fCurtime;
};

// ======================
// =  Shader Paramater  =
// ======================
struct tScalarParam
{
	SCALAR_PARAM	eParam;
	string			strName;
};

struct tTextureParam
{
	TEX_PARAM		eParam;
	string			strName;
};

// ======================
// = 상수 버퍼용 구조체 =
// ======================

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};

struct tGlobalData
{
	Vec2	vRenderResolution;	// 출력 해상도
	Vec2	vNoiseResolution;	// 노이즈 텍스처 해상도
	int		iLight2DCount;		// 광원 갯수
	int		iLight3DCount;

	float	fAccTime;			// 누적 시간
	float	fDT;				// Delta Time
};

extern tGlobalData g_global;

// Animation2D용 상수 버퍼 구조체
// 상수 버퍼는 16바이트 단위로 맞춰주어야 한다.
struct tAnim2DInfo
{
	Vec2 vLeftTop;		// 샘플링할 위치 : 우리는 아틀라스 이미지를 바인딩하기 때문에 어떤 위치를 잘라야하는지 알아야 한다.
	Vec2 vSlice;		// 받은 좌상단을 기점으로 얼마나 자를지 Slice 정보
	Vec2 vOffset;		// 중심이 안맞을 경우 offset 해줄 값
	Vec2 vFullSize;		// 전체 크기 : 
	int	 iAnim2DUse;	// 지금 에니메이션2D가 사용중인지 확인하기 위한 값. 0일 경우 사용하지 않는 중이다.
						// 0일 경우 t0에 바인딩된 텍스처를 사용하고, 1일 경우 t12에 바인딩된 아틀라스 텍스처를 사용한다.
	int  ipadding[3];	// 16바이트 단위로 맞추기 위한 공간할당 (패딩)
};

struct tBGRA
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
};