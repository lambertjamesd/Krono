#pragma once

#include "HLSLExpressionNode.h"

class HLSLBinaryOperatorNode : public HLSLExpressionNode
{
public:
	~HLSLBinaryOperatorNode(void);

	HLSLExpressionNode& GetLeft();
	HLSLExpressionNode& GetRight();

	void SetLeft(std::unique_ptr<HLSLExpressionNode> value);
	void SetRight(std::unique_ptr<HLSLExpressionNode> value);

	// takes ownership away
	std::unique_ptr<HLSLExpressionNode> TakeLeft();
	std::unique_ptr<HLSLExpressionNode> TakeRight();

	static std::unique_ptr<HLSLBinaryOperatorNode> CreateNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> right);

	virtual void Accept(HLSLNodeVisitor& visitor);
protected:
	HLSLBinaryOperatorNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> right);

	std::unique_ptr<HLSLExpressionNode> mLeft;
	std::unique_ptr<HLSLExpressionNode> mRight;
};