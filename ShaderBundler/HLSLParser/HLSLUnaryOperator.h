#pragma once

#include "HLSLExpressionNode.h"

class HLSLUnaryOperator : public HLSLExpressionNode
{
public:
	~HLSLUnaryOperator(void);

	HLSLExpressionNode& GetInnerExpression();

	static std::unique_ptr<HLSLUnaryOperator> CreateNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> inner);

	virtual void Accept(HLSLNodeVisitor& visitor);
protected:
	HLSLUnaryOperator(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> value);
	std::unique_ptr<HLSLExpressionNode> mInnerExpression;
};

class HLSLCastNode : public HLSLExpressionNode
{
public:
	HLSLCastNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type, std::unique_ptr<HLSLExpressionNode> right);

	HLSLTypeNode& GetType();
	HLSLExpressionNode& GetRight();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mType;
	std::unique_ptr<HLSLExpressionNode> mRight;
};