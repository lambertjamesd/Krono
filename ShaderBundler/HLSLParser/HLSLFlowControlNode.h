#pragma once

#include "HLSLNode.h"
#include "HLSLStatementNode.h"
#include "HLSLExpressionNode.h"

class HLSLFlowControlNode : public HLSLStatementNode
{
public:
	~HLSLFlowControlNode(void);
protected:
	HLSLFlowControlNode(const HLSLToken& token);
};

class HLSLDoNode : public HLSLFlowControlNode
{
public:
	HLSLDoNode(const HLSLToken& token, std::unique_ptr<HLSLNode> body, std::unique_ptr<HLSLExpressionNode> condition);

	HLSLNode& GetBody();
	HLSLExpressionNode& GetCondition();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLNode> mBody;
	std::unique_ptr<HLSLExpressionNode> mCondition;
};

class HLSLIfNode : public HLSLFlowControlNode
{
public:
	HLSLIfNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> condtion, std::unique_ptr<HLSLNode> body);

	void SetElseBody(std::unique_ptr<HLSLNode> body);

	HLSLExpressionNode& GetCondition();
	HLSLNode& GetBody();
	HLSLNode* GetElseBody();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mCondition;
	std::unique_ptr<HLSLNode> mBody;
	std::unique_ptr<HLSLNode> mElseBody;
};

class HLSLForNode : public HLSLFlowControlNode
{
public:
	HLSLForNode(const HLSLToken& token, 
		std::unique_ptr<HLSLStatementNode> initializer, 
		std::unique_ptr<HLSLExpressionNode> condition, 
		std::unique_ptr<HLSLExpressionNode> iterator, 
		std::unique_ptr<HLSLNode> body);

	HLSLStatementNode& GetInitializer();
	HLSLExpressionNode& GetCondition();
	HLSLExpressionNode& GetModifier();
	HLSLNode& GetBody();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLStatementNode> mInitializer;
	std::unique_ptr<HLSLExpressionNode> mCondition;
	std::unique_ptr<HLSLExpressionNode> mModifier;
	std::unique_ptr<HLSLNode> mBody;
};

class HLSLSwitchNode : public HLSLFlowControlNode
{
public:
	HLSLSwitchNode(const HLSLToken& token,
		std::unique_ptr<HLSLExpressionNode> selectValue,
		std::unique_ptr<HLSLNode> body);

	HLSLExpressionNode& GetSelectValue();
	HLSLNode& GetBody();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mSelectValue;
	std::unique_ptr<HLSLNode> mBody;
};

class HLSLSwitchLabelNode : public HLSLFlowControlNode
{
public:
	HLSLSwitchLabelNode(const HLSLToken& token, int value);

	int GetValue() const;
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	int mValue;
};

class HLSLDefaultNode : public HLSLFlowControlNode
{
public:
	HLSLDefaultNode(const HLSLToken& token);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLWhileNode : public HLSLFlowControlNode
{
public:
	HLSLWhileNode(const HLSLToken& token,
		std::unique_ptr<HLSLExpressionNode> condition,
		std::unique_ptr<HLSLNode> body);

	HLSLExpressionNode& GetCondition();
	HLSLNode& GetBody();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mCondition;
	std::unique_ptr<HLSLNode> mBody;
};

class HLSLBreakNode : public HLSLFlowControlNode
{
public:
	HLSLBreakNode(const HLSLToken& token);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLContinueNode : public HLSLFlowControlNode
{
public:
	HLSLContinueNode(const HLSLToken& token);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLDiscardNode : public HLSLFlowControlNode
{
public:
	HLSLDiscardNode(const HLSLToken& token);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
};