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

void HLSLNodeList::RemoveElement(size_t index)
{
	for (size_t i = index + 1; i < mElementList.size(); ++i)
	{
		mElementList[i - 1] = move(mElementList[i]);
	}

	mElementList.pop_back();
}

void HLSLNodeList::ReplaceElement(size_t index, std::unique_ptr<HLSLNode> node)
{
	mElementList[index] = move(node);
}

void HLSLNodeList::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}