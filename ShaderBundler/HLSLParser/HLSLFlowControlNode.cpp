#include "HLSLFlowControlNode.h"

#include "HLSLNodeVisitor.h"

HLSLFlowControlNode::HLSLFlowControlNode(const HLSLToken& token) :
	HLSLStatementNode(token)
{

}


HLSLFlowControlNode::~HLSLFlowControlNode(void)
{

}

HLSLDoNode::HLSLDoNode(const HLSLToken& token, std::unique_ptr<HLSLNode> body, std::unique_ptr<HLSLExpressionNode> condition) :
	HLSLFlowControlNode(token),
	mBody(move(body)),
	mCondition(move(condition))
{

}

HLSLNode& HLSLDoNode::GetBody()
{
	return *mBody;
}

HLSLExpressionNode& HLSLDoNode::GetCondition()
{
	return *mCondition;
}

void HLSLDoNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLIfNode::HLSLIfNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> condtion, std::unique_ptr<HLSLNode> body) :
	HLSLFlowControlNode(token),
	mCondition(move(condtion)),
	mBody(move(body))
{

}

void HLSLIfNode::SetElseBody(std::unique_ptr<HLSLNode> body)
{
	mElseBody = move(body);
}

HLSLExpressionNode& HLSLIfNode::GetCondition()
{
	return *mCondition;
}

HLSLNode& HLSLIfNode::GetBody()
{
	return *mBody;
}

HLSLNode* HLSLIfNode::GetElseBody()
{
	return mElseBody.get();
}

void HLSLIfNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLForNode::HLSLForNode(const HLSLToken& token, 
		std::unique_ptr<HLSLStatementNode> initializer, 
		std::unique_ptr<HLSLExpressionNode> condition, 
		std::unique_ptr<HLSLExpressionNode> iterator, 
		std::unique_ptr<HLSLNode> body) :
	HLSLFlowControlNode(token),
	mInitializer(move(initializer)),
	mCondition(move(condition)),
	mModifier(move(iterator)),
	mBody(move(body))
{

}

HLSLStatementNode& HLSLForNode::GetInitializer()
{
	return *mInitializer;
}

HLSLExpressionNode& HLSLForNode::GetCondition()
{
	return *mCondition;
}

HLSLExpressionNode& HLSLForNode::GetModifier()
{
	return *mModifier;
}

HLSLNode& HLSLForNode::GetBody()
{
	return *mBody;
}
	
void HLSLForNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLSwitchNode::HLSLSwitchNode(const HLSLToken& token,
		std::unique_ptr<HLSLExpressionNode> selectValue,
		std::unique_ptr<HLSLNode> body) :
	HLSLFlowControlNode(token),
	mSelectValue(move(selectValue)),
	mBody(move(body))
{

}

HLSLExpressionNode& HLSLSwitchNode::GetSelectValue()
{
	return *mSelectValue;
}

HLSLNode& HLSLSwitchNode::GetBody()
{
	return *mBody;
}

void HLSLSwitchNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLSwitchLabelNode::HLSLSwitchLabelNode(const HLSLToken& token, int value) :
	HLSLFlowControlNode(token),
	mValue(value)
{

}

int HLSLSwitchLabelNode::GetValue() const
{
	return mValue;
}

void HLSLSwitchLabelNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLDefaultNode::HLSLDefaultNode(const HLSLToken& token) :
	HLSLFlowControlNode(token)
{

}

void HLSLDefaultNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLWhileNode::HLSLWhileNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> condition, std::unique_ptr<HLSLNode> body) :
	HLSLFlowControlNode(token),
	mCondition(move(condition)),
	mBody(move(body))
{

}

HLSLExpressionNode& HLSLWhileNode::GetCondition()
{
	return *mCondition;
}

HLSLNode& HLSLWhileNode::GetBody()
{
	return *mBody;
}

void HLSLWhileNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLBreakNode::HLSLBreakNode(const HLSLToken& token) :
	HLSLFlowControlNode(token)
{

}

void HLSLBreakNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLContinueNode::HLSLContinueNode(const HLSLToken& token) :
	HLSLFlowControlNode(token)
{

}

void HLSLContinueNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLDiscardNode::HLSLDiscardNode(const HLSLToken& token) :
	HLSLFlowControlNode(token)
{

}

void HLSLDiscardNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}