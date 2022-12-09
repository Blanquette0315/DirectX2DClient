#include "pch.h"
#include "TreeUI.h"

// ============
// = TreeNode =
// ============

TreeNode::TreeNode()
	: m_ParentNode(nullptr)
	, m_data(0)
	, m_TreeUI(nullptr)
	, m_bFrame(false)
	, m_bSelected(false)
	, m_iIdx(0)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChildNode);
}

void TreeNode::render_update()
{
	int iFlag = 0;
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;
	if (m_vecChildNode.empty())
		iFlag |= ImGuiTreeNodeFlags_Leaf;

	string strName = m_strName;
	if (m_bFrame && m_vecChildNode.empty())
		strName = "\t" + strName;

	// 뒤에 식별번호를 붙여서 이름(키) 중복을 방지해준다.
	// UINT니까 4억개 정도는 방지가 될 것.
	char szTag[50] = "";
	sprintf_s(szTag, 50, "##%d", m_iIdx);
	strName += szTag;

	if (ImGui::TreeNodeEx(strName.c_str(),iFlag))
	{
		// 드래그 체크
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			m_TreeUI->SetBeginDragNode(this);

			// 첫 인자인 키값은 나중에 드랍을 받는쪽에서 체크해 같으면 데이터를 전달해준다.
			// 두번째 인자인 데이터는 해당 노드 자체를 보낼 것이기 때문에 데이터 사이즈는 TreeNode만큼이다.
			ImGui::SetDragDropPayload(m_TreeUI->GetName().c_str(), (void*)this, sizeof(TreeNode));
			ImGui::Text(strName.c_str());

			ImGui::EndDragDropSource();
		}

		// 드랍체크
		if (ImGui::BeginDragDropTarget())
		{
			m_TreeUI->SetDropTargetNode(this);

			ImGui::EndDragDropTarget();
		}

		// 클릭 체크 : Clicked로 하면, 드래그와 문제가 생겨서 Released로 변경하였다.
		if (!m_bFrame && ImGui::IsItemHovered(0) && ImGui::IsMouseReleased(0))
		{
			m_TreeUI->SetSelectedNode(this);
		}

		// 자식 노드들까지 재귀적 호출을 해준다.
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}




// ============
// =   Tree   =
// ============
// TreeUI의 이름은 생성자에서 넣어주어서 이름 중복을 방지하도록 구현했다. (ImGui에서 이름 중복이 발생하면 안됨)

UINT TreeUI::m_iNextNodeIdx = 0;

TreeUI::TreeUI(const string& _strName)
	: UI(_strName)
	, m_RootNode(nullptr)
	, m_bDummyRootUse(false)
	, m_SelectedNode(nullptr)
	, m_BeginDragNode(nullptr)
	, m_DropTargetNode(nullptr)
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}

void TreeUI::update()
{
}

void TreeUI::render_update()
{
	if (nullptr == m_RootNode)
		return;

	if (!m_bDummyRootUse)
	{
		m_RootNode->render_update();
	}
	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChild();
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_update();
		}
	}

	// 마우스 왼쪽 릴리즈 체크
	if (ImGui::IsMouseReleased(0))
	{
		m_BeginDragNode = nullptr;
		m_DropTargetNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetData(_data);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;
	pNode->m_iIdx = m_iNextNodeIdx++;

	// RootNode로 지정되는 노드
	if (nullptr == _parent)
	{
		// 해당 노드를 Root노드로 지정하려하는데, 이미 rootnode가 존재한다면 중단.
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

	return pNode;
}

void TreeUI::Clear()
{
	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	// 이전에 선택된 노드가 있었다면, 해당 노드의 선택을 풀어준다.
	if (nullptr != m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}

	// 들어온 노드를 선택 노드로 지정하고, 해당 노드에게 선택됨을 알려준다.
	m_SelectedNode = _SelectedNode;
	m_SelectedNode->m_bSelected = true;

	if (m_SelectInst && m_SelectedNode)
	{
		(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
	}
}

void TreeUI::SetDropTargetNode(TreeNode* _DropTargetNode)
{
	m_DropTargetNode = _DropTargetNode;

	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str()))
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_BeginDragNode, (DWORD_PTR)m_DropTargetNode);
		}
	}

	// 외부 드랍 관련 PayLoad 처리하기
}