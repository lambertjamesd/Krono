#include "HLSLPreprocessorNode.h"


HLSLPreprocessorNode::HLSLPreprocessorNode(void)
{
}


HLSLPreprocessorNode::~HLSLPreprocessorNode(void)
{
}

HLSLPreprooessorNodeList::HLSLPreprooessorNodeList(void)
{

}
	
void HLSLPreprooessorNodeList::Accept(HLSLPreprocessorNodeVisitor& visitor)
{
	for (auto it = mNodeList.begin(); it != mNodeList.end(); ++it)
	{
		(*it)->Accept(visitor);
	}
}

void HLSLPreprooessorNodeList::AddNode(std::unique_ptr<HLSLPreprocessorNode> node)
{
	mNodeList.push_back(move(node));
}