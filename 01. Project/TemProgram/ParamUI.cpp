#include "pch.h"
#include "ParamUI.h"

#include <Engine/CResMgr.h>

#include "imgui.h"
#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

UINT ParamUI::ParamCount = 0;


void ParamUI::Param_Int(const string& _ParamName, int* _pInOut)
{
	char szName[50] = "";
	sprintf_s(szName, 50, "##Int%d", ParamCount++);

	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputInt(szName, _pInOut);
}

void ParamUI::Param_Float(const string& _ParamName, float* _pInOut)
{
	char szName[50] = "";
	sprintf_s(szName, 50, "##Float%d", ParamCount++);

	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat(szName, _pInOut);
}

void ParamUI::Param_Vec2(const string& _ParamName, Vec2* _pInOut)
{
	char szName[50] = "";
	sprintf_s(szName, 50, "##Vec2%d", ParamCount++);

	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();

	float arrData[2] = { _pInOut->x, _pInOut->y };
	ImGui::InputFloat2(szName, arrData);
	_pInOut->x = arrData[0];
	_pInOut->y = arrData[1];

}

void ParamUI::Param_Vec4(const string& _ParamName, Vec4* _pInOut)
{
	char szName[50] = "";
	sprintf_s(szName, 50, "##Vec4%d", ParamCount++);

	ImGui::Text(_ParamName.c_str());
	ImGui::SameLine();
	ImGui::InputFloat4(szName, *_pInOut);
}

void ParamUI::Param_Mat(const string& _ParamName, Matrix* _pInOut)
{
}

bool ParamUI::Param_Tex(const string& _ParamName, Ptr<CTexture>& _Tex, UI* _Inst, FUNC_1 _Func)
{
	ImGui::Text(_ParamName.c_str());

	if (nullptr != _Tex)
	{
		ImGui::Image(_Tex->GetSRV().Get(), ImVec2(100.f, 100.f));
	}
	else
	{
		ImGui::Image(nullptr, ImVec2(100.f, 100.f));
	}


	// 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##ContentTree"))
		{
			TreeNode* pNode = (TreeNode*)payload->Data;
			CRes* pRes = (CRes*)pNode->GetData();

			if (RES_TYPE::TEXTURE == pRes->GetResType())
			{
				_Tex = (CTexture*)pRes;
			}
		}
		ImGui::EndDragDropTarget();
	}

	char szName[50] = "";
	sprintf_s(szName, 50, "##TexBtn%d", ParamCount++);

	ImGui::SameLine();
	if (ImGui::Button(szName, Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(_Inst, _Func);

		pListUI->Open();

		return true;
	}

	return false;
}