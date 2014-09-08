#pragma once

#include "HLSLExpressionNode.h"

class HLSLPostfixNode : public HLSLExpressionNode
{
public:
	~HLSLPostfixNode(void);

	HLSLExpressionNode& GetLeft();
protected:
	HLSLPostfixNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left);

	std::unique_ptr<HLSLExpressionNode> mLeft;
};

class HLSLIndexNode : public HLSLPostfixNode
{
public:
	HLSLIndexNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, std::unique_ptr<HLSLExpressionNode> index);

	HLSLExpressionNode& GetIndex();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mIndex;
};

class HLSLPostfixIncr : public HLSLPostfixNode
{
public:
	HLSLPostfixIncr(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left);

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLPostfixDecr : public HLSLPostfixNode
{
public:
	HLSLPostfixDecr(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
};

class HLSLStructureNode : public HLSLPostfixNode
{
public:
	HLSLStructureNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left, const std::string& right);
	
	const std::string& GetRight();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::string mRight;
};

class HLSLFunctionCallNode : public HLSLPostfixNode
{
public:
	HLSLFunctionCallNode(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> left);

	void AddParameter(std::unique_ptr<HLSLExpressionNode> parameter);

	size_t GetParameterCount() const;
	HLSLExpressionNode& GetParameter(size_t index);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::vector<std::unique_ptr<HLSLExpressionNode> > mExpressionList;
};