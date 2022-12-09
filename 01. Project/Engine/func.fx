#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "register.fx"

// 첫번째 인자의 inout은 가져와서 수정이 가능한 레퍼런스와 유사한 느낌이다.
// out은 가져오기만 할 뿐 쓸수는 없는 const 레퍼런스와 유사한 느낌
// https://learn.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-function-parameters 참고
void CalcLight2D(inout tLightColor _LightColor, float3 _vWorldPos, int iLightIdx)
{
	// Directinal Light
	if (0 == g_Light2DBuffer[iLightIdx].iLightType)
	{
		_LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
	}

	// Point Light
	else if (1 == g_Light2DBuffer[iLightIdx].iLightType)
	{
		// distance()는 두 점 사이의 거리(스칼라 값)을 구해준다.
		// 우리는 그리는 순서를 잡기 위해 z값을 사용한다. 하지만 광원의 z축 영역까지 계산을 하려면 매우 복잡하기 때문에
		// 2D 환경에서는 우선 x,y로만 계산해 광원을 2D로 적용했다.
		float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);

		if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
		{
			// 광원과의 거리를 구해 점점 밝아지는 효과를 적용한다.
			float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;
			_LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;
		}
	}

	// Spot Light
	// 방향과 각도 제한을 걸어주면 된다.
	else if (2 == g_Light2DBuffer[iLightIdx].iLightType)
    {
		// 해당 픽셀의 지점과 광원과의 거리 구하기
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);
		// 거리 체크
        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
            float x = _vWorldPos.x - g_Light2DBuffer[iLightIdx].vWorldPos.x;
            float y = _vWorldPos.y - g_Light2DBuffer[iLightIdx].vWorldPos.y;
			
            float radian = atan2(y, x);
            float deg = degrees(radian);
			
            float rangeDirdeg = degrees(atan2(g_Light2DBuffer[iLightIdx].vWorldDir.y, g_Light2DBuffer[iLightIdx].vWorldDir.x));
			
            //float rangeDirdeg = degrees(acos(g_Light2DBuffer[iLightIdx].vWorldDir.x));
            //float rangeDirdeg = degrees(acos(1));
            float rangedeg1 = rangeDirdeg + g_Light2DBuffer[iLightIdx].fAngle / 2;
            float rangedeg2 = rangeDirdeg - g_Light2DBuffer[iLightIdx].fAngle / 2;
			// 각도 체크
            if ((deg < rangedeg1 && deg > rangeDirdeg)
				|| (deg < rangeDirdeg && deg > rangedeg2))
            {
				// 원점에 가까워 질 수록 점점 밝아진다.
                float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;
                _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;
            }
        }
    }
}


// ComputeShader에서 사용할 Sampler 구현
// CS에서는 PS에서 사용하던 Texture.Sample(Sampler, UV)를 사용할 수 없다.
// SampleLevel()은 사용이 가능하지만 우리가 원하는 가우시안 필터를 사용하는 샘플러를 지정할 수 없기 때문에 직접 구현한다.

// 가우시안 필터 구조체
// HLSL에서 static 키워드를 붙이면, 전역에서 사용이 가능해진다. (즉, CS쪽에서도 사용이 가능해진다.)
// 가우시안 필터는 중점의 비중이 제일크고, 멀어질 수록 비중이 작아지는 필터이다. (해당 필터 행렬의 총합은 1이다.)
// 해당 필터를 사용하는 샘플러를 만들기 위해 샘플 함수를 직접 구현하는 것이다.
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

// 가우시안 샘플링 함수
// 직접 만든 샘플러는 WRAP이 없어서 직접 구현을 해주어야 한다.
float4 GaussianSample(float2 _UV)
{
    float4 vOutColor = (float4) 0.f;
	
    if (1.f < _UV.x)
        _UV.x = frac(_UV.x);
    else if (_UV.x < 0.f)
        _UV.x = 1 + frac(_UV.x);

    if (1.f < _UV.y)
        _UV.y = frac(_UV.y);
    else if (_UV.y < 0.f)
        _UV.y = 1 + frac(_UV.y);
    
    // 해당 UV값에 맞는 텍스처 픽셀 좌표 계산
    int2 iUV = _UV * g_vNoiseResolution;
    // 우리가 얻은 UV는 중심점이기 때문에 가우시안 필터의 0번째 인덱스와 동일하게 바꿔준다.
    iUV -= int2(2, 2);
    // 이후 가우시안 필터의 행렬에 맞게 반복문을 수행해 샘플링을 수행한다.
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            int2 idx = int2(iUV.y + i, iUV.x + j);
            // 노이즈 텍스처에 접근해서 알맞은 가우시안 필터의 값을 곱해 누적해준다.
            // HLSL에서 텍스처의 픽셀에 직접 접근하려면 [int][int]가 아니라 [int2]로 접근을 해야한다.
            vOutColor += g_Noise[idx] * GaussianFilter[j][i];
        }
    }
    
    // 샘플링이 끝난 결과를 반환해준다.
    return vOutColor;
}

// 0 ~ 1 사이의 값을 반환
float Random(int key)
{
    float fValue = 0.f;
	
    return fValue;
}
#endif