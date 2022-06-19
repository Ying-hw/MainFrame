#include "stdafx.h"
#include "TreeStruct.h"
#include "MainFrame.h"
#include "MacroDefine.h"
#include "AbstractObject.h"

TreeStruct* Tree_ApiFun::TreeFunction::createTree()
{
	TreeStruct* treeHead = new TreeStruct{ std::string(), std::list<TreeStruct*>() };
	return treeHead;
}

void Tree_ApiFun::TreeFunction::appendChild(std::string& strNameId, TreeStruct* pRoot, bool isParent)
{
	if (!isParent)
	{
		if (!findTreeChild(strNameId, pRoot))
		{
			TreeStruct* pNewChild = new TreeStruct{ strNameId, std::list<TreeStruct*>() };
			pRoot->childTree.emplace_back(pNewChild);
		}
	}
	else
	{
		TreeStruct* treeHead = new TreeStruct{ strNameId, std::list<TreeStruct*>() };
		pRoot->childTree.push_back(treeHead);
	}
}

TreeStruct* Tree_ApiFun::TreeFunction::findTreeChild(std::string& strNameId, TreeStruct* pRoot)
{
	auto itera = std::find_if(pRoot->childTree.begin(), pRoot->childTree.end(), [&strNameId](const TreeStruct* pTree) {
		return strNameId == pTree->strNameId;
	});
	if (itera != pRoot->childTree.end())
	{
		return *itera;
	}
	return nullptr;
}
