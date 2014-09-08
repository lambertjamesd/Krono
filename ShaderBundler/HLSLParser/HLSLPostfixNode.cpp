#include "HLSLPostfixNode.h"
#include "HLSLNodeVisitor.h"

HLSLPostfixNode::HLSLPostfixNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left) :
	HLSLExpressionNode(token),
	mLeft(move(left))
{
}


HLSLPostfixNode::~HLSLPostfixNode(void)
{
}

HLSLExpressionNode& HLSLPostfixNode::GetLeft()
{
	return *mLeft;
}

HLSLIndexNode::HLSLIndexNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> index) :
	HLSLPostfixNode(token, move(left)),
	mIndex(move(index))
{

}

HLSLExpressionNode& HLSLIndexNode::GetIndex()
{
	return *mIndex;
}

void HLSLIndexNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLPostfixIncr::HLSLPostfixIncr(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left) :
	HLSLPostfixNode(token, move(left))
{

}

void HLSLPostfixIncr::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLPostfixDecr::HLSLPostfixDecr(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left) :
	HLSLPostfixNode(token, move(left))
{

}

void HLSLPostfixDecr::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLStructureNode::HLSLStructureNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, const std::string& right) :
	HLSLPostfixNode(token, move(left)),
	mRight(right)
{

}

const std::string& HLSLStructureNode::GetRight()
{
	return mRight;
}

void HLSLStructureNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLFunctionCallNode::HLSLFunctionCallNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left) :
	HLSLPostfixNode(token, move(left))
{

}

void HLSLFunctionCallNode::AddParameter(std::unique_ptr<HLSLExpressionNode> parameter)
{
	mExpressionList.push_back(move(parameter));
}

size_t HLSLFunctionCallNode::GetParameterCount() const
{
	return mExpressionList.size();
}

HLSLExpressionNode& HLSLFunctionCallNode::GetParameter(size_t index)
{
	return *mExpressionList[index];
}

void HLSLFunctionCallNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}