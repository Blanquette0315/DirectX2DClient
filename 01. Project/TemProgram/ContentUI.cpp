#include "pch.h"
#include "ContentUI.h"

#include <Engine/CResMgr.h>

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "TreeUI.h"

ContentUI::ContentUI()
	: UI("Content")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	// Tree 설정
	m_Tree->SetDummyRoot(true);

	// Tree에 Delegate 등록
	m_Tree->AddDynamic_Selected(this, (FUNC_1)& ContentUI::SetResourceToInspector);

	// 현재 리소스 상태를 갱신
	ResetContent();
}

ContentUI::~ContentUI()
{
}

void ContentUI::update()
{
	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}
	
	UI::update();
}

void ContentUI::render_update()
{

}

void ContentUI::ResetContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "Content", 0);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, ToString((RES_TYPE)i), 0, true);

		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			wstring strName = iter->first;
			m_Tree->AddItem(pResNode, string(strName.begin(), strName.end()), (DWORD_PTR)iter->second.Get());
		}
	}
}

void ContentUI::SetResourceToInspector(DWORD_PTR _res)
{
	// _res : 클릭된 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CRes* pRes = (CRes*)pSelectedNode->GetData();

	// InspectorUI에 클릭된 Resource를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetResource(pRes);
}