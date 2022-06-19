#ifndef  __TREESTRUCT__
#define __TREESTRUCT__
#include <string>
#include <List>

class AbstractObject;

struct TreeStruct 
{
	std::string strNameId;
	std::list<TreeStruct*> childTree;
};


namespace Tree_ApiFun 
{
	class TreeFunction
	{ 
	public:
		TreeFunction() {}
		TreeStruct* createTree();
		void appendChild(std::string& strNameId, TreeStruct* pRoot, bool isParent);
		TreeStruct* findTreeChild(std::string& strNameId, TreeStruct* pRoot);
	};
	
}
#endif __TREESTRUCT__

