#include "pch.h"
#include "MaterialUI.h"

#include <Engine/CMaterial.h>
#include <Engine/CResMgr.h>
#include "ParamUI.h"

MaterialUI::MaterialUI()
	: ResUI("Material##UI", RES_TYPE::MATERIAL)
	, m_eSelectTexParam(TEX_PARAM::TEX_END)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::update()
{
	ResUI::update();
}

void MaterialUI::render_update()
{
	ResUI::render_update();

	CMaterial* pMtrl = ((CMaterial*)GetTarget().Get());

	string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	string strRelativePath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

	ImGui::Text("Key           ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Path          ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlPath", (char*)strRelativePath.data(), strRelativePath.length(), ImGuiInputTextFlags_ReadOnly);

	string strShaderKey;
	if (nullptr != pMtrl->GetShader())
	{
		strShaderKey = string(pMtrl->GetShader()->GetKey().begin(), pMtrl->GetShader()->GetKey().end());
	}

	ImGui::Text("GraphicsShader");
	ImGui::SameLine();
	ImGui::InputText("##ShaderName", (char*)strShaderKey.data(), strShaderKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::NewLine();
	ImGui::Text("Shader Parameter");
	
	const vector<tScalarParam> vecScalar = pMtrl->GetShader()->GetScalarParam();
	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int iInput, iOutput;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &iInput);
			pMtrl->GetScalarParam(vecScalar[i].eParam, &iOutput);
			//ImGui::Text("Test");
			//ImGui::SameLine();
			//ImGui::InputInt("##TestInt", &iOutput);
			ParamUI::Param_Int(vecScalar[i].strName, &iOutput);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &iOutput);

			//if (iInput != iOutput)
			//{
			//	pMtrl->Save(L"material\\mtrl.mtrl");
			//}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float fData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
			ParamUI::Param_Float(vecScalar[i].strName, &fData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec2(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec4(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{

		}
		break;
		}
	}
}