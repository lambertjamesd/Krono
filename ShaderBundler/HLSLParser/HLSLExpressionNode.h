#pragma once

#include "HLSLStatementNode.h"

class HLSLExpressionNode : public HLSLStatementNode
{
public:
	~HLSLExpressionNode(void);
protected:
	HLSLExpressionNode(const HLSLToken& token);
};

class HLSLIdentifierNode : public HLSLExpressionNode
{
public:
	HLSLIdentifierNode(const HLSLToken& token);

	const std::string& GetName() const;

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLNumberConstantNode : public HLSLExpressionNode
{
public:
	HLSLNumberConstantNode(const HLSLToken& token);

	const std::string& GetStringValue() const;
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLBooleanConstantNode : public HLSLExpressionNode
{
public:
	HLSLBooleanConstantNode(const HLSLToken& token);

	bool GetBoolValue() const;
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLTypeExpressionNode : public HLSLExpressionNode
{
public:
	HLSLTypeExpressionNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type);

	HLSLTypeNode& GetType();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mType;
};

class HLSLParenthesisNode : public HLSLExpressionNode
{
public:
	HLSLParenthesisNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> value);

	HLSLExpressionNode& GetValue();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mValue;
};