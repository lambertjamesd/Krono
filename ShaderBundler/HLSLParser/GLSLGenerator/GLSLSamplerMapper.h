#pragma once

#include "HLSLParser/HLSLNodeVisitor.h"

#include <map>
#include <vector>
#include <string>

class GLSLVariableBinding
{
public:
	GLSLVariableBinding();
	GLSLVariableBinding(size_t variableLocation);

	void Append(const GLSLVariableBinding& other);
private:
	size_t mBindingMapping;
};

class GLSLSamplerTexturePair
{
public:
	GLSLSamplerTexturePair(size_t samplerLocation, size_t textureLocation);

	size_t GetSamplerLocation() const;
	size_t GetTextureLocation() const;

	bool operator<(const GLSLSamplerTexturePair& other) const;
private:
	size_t mSamplerLocation;
	size_t mTextureLocation;
};

class GLSLVariableAddress
{
public:
	GLSLVariableAddress(HLSLVariableDefinition& variableDefinition);

	GLSLVariableAddress MemberAddress(const std::string& memeberName) const;
	GLSLVariableAddress IndexAddress(size_t index) const;

	bool operator<(const GLSLVariableAddress& other) const;
private:
	HLSLVariableDefinition* mVariableDefinition;
	std::string mStructureName;
	std::size_t mIndex;
};

class GLSLAddressRetriever : public HLSLNodeVisitor
{
public:
	GLSLAddressRetriever(HLSLNodeVisitor& functionMapper);
	virtual ~GLSLAddressRetriever();
	
	virtual void Visit(HLSLIdentifierNode& node);
	virtual void Visit(HLSLParenthesisNode& node);
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);
private:
	GLSLVariableAddress mResult;
};

class GLSLFunctionMapper : public HLSLNodeVisitor
{
public:
	GLSLFunctionMapper(HLSLFunctionDefinition& functionDefinition, const std::vector<GLSLVariableBinding>& parameterBindings);
	virtual ~GLSLFunctionMapper(void);
	
	virtual void Visit(HLSLNode& node);
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLReturnStatement& node);
	
	virtual void Visit(HLSLDoNode& node);
	virtual void Visit(HLSLIfNode& node);
	virtual void Visit(HLSLForNode& node);
	virtual void Visit(HLSLSwitchNode& node);
	virtual void Visit(HLSLWhileNode& node);
	virtual void Visit(HLSLIdentifierNode& node);
	
	virtual void Visit(HLSLParenthesisNode& node);
	
	virtual void Visit(HLSLBinaryOperatorNode& node);
	
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);
private:
	void BeginExpression();
	void AppendExpression(GLSLVariableBinding binding);
	GLSLVariableBinding EndExpression();
	std::vector<GLSLVariableBinding> mExpressionBindingStack;

	GLSLVariableBinding GetVariableBinding(HLSLVariableDefinition& variableDefinition) const;
	void UpdateVariableBinding(HLSLVariableDefinition& variableDefinition, GLSLVariableBinding binding);
	std::map<HLSLVariableDefinition*, GLSLVariableBinding> mVariableBindingMapping;

	GLSLVariableBinding mReturnValueBinding;
	std::map<GLSLSamplerTexturePair, size_t> mPairUseCount;
};

