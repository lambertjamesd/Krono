#pragma once

#include "HLSLExpressionNode.h"
#include "HLSLTypeNode.h"

class HLSLPrefixNode : public HLSLExpressionNode
{
public:
	~HLSLPrefixNode(void);

	HLSLExpressionNode& GetRight();
protected:
	HLSLPrefixNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right);
	std::unique_ptr<HLSLExpressionNode> mRight;
};

class HLSLPrefixIncrNode : public HLSLPrefixNode
{
public:
	HLSLPrefixIncrNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right);

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLPrefixDecrNode : public HLSLPrefixNode
{
public:
	HLSLPrefixDecrNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> right);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};