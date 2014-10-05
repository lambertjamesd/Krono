#pragma once

#include "HLSLStatementNode.h"
#include "HLSLType.h"

class HLSLExpressionNode : public HLSLStatementNode
{
public:
	~HLSLExpressionNode(void);

	const HLSLType& GetType() const;
	void ResolveType(const HLSLType& type);
protected:
	HLSLExpressionNode(const HLSLToken& token);
private:
	HLSLType mType;
};

class HLSLIdentifierNode : public HLSLExpressionNode
{
public:
	HLSLIdentifierNode(const HLSLToken& token);

	const std::string& GetName() const;

	bool IsTypeName() const;

	void ResolveNamedType(const std::shared_ptr<HLSLTypeNode>& value);
	const std::shared_ptr<HLSLTypeNode>& GetNamedType() const;

	void ResolveVariableDefinition(HLSLVariableDefinition* variableDefinition);
	HLSLVariableDefinition* GetVariableDefinition();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::shared_ptr<HLSLTypeNode> mNamedType;
	HLSLVariableDefinition* mVariableDefintion;
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