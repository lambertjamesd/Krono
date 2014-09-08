#include "HLSLNode.h"
#include "HLSLNodeVisitor.h"

HLSLNode::HLSLNode(const HLSLToken& token) :
	mToken(token)
{
}


HLSLNode::~HLSLNode(void)
{
}

const HLSLToken& HLSLNode::GetToken() const
{
	return mToken;
}

HLSLNodeList::HLSLNodeList() :
	HLSLNode(HLSLToken(HLSLTokenType::None, "", 0))
{

}

void HLSLNodeList::AddNode(std::unique_ptr<HLSLNode> element)
{
	mElementList.push_back(move(element));
}

size_t HLSLNodeList::GetSize() const
{
	return mElementList.size();
}

HLSLNode& HLSLNodeList::GetElement(size_t index)
{
	return *mElementList[index];
}

void HLSLNodeList::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}