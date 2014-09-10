#pragma once

#include "HLSLParser/HLSLNodeVisitor.h"

class GLSLSamplerPurger : public HLSLNodeVisitor
{
public:
	~GLSLSamplerPurger(void);

	// does a 2 pass purge
	// the first pass removes samplers from
	// expressions and structurs
	// the second pass removes function definitions
	// that return samplers
	// two passes are necessary so functions definitions are
	// not removed before a function all that references it
	static void Purge(HLSLNode& node);
	
	virtual void Visit(HLSLNode& node);
	virtual void Visit(HLSLNodeList& node);
	
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLFunctionParameter& node);
	virtual void Visit(HLSLFunctionDefinition& node);
	
	virtual void Visit(HLSLStructureMember& node);
	virtual void Visit(HLSLStructDefinition& node);

	virtual void Visit(HLSLDoNode& node);
	virtual void Visit(HLSLIfNode& node);
	virtual void Visit(HLSLForNode& node);
	virtual void Visit(HLSLSwitchNode& node);
	virtual void Visit(HLSLWhileNode& node);
	
	virtual void Visit(HLSLExpressionNode& node);
	virtual void Visit(HLSLParenthesisNode& node);
	
	virtual void Visit(HLSLBinaryOperatorNode& node);
	
	virtual void Visit(HLSLUnaryOperator& node);
	virtual void Visit(HLSLCastNode& node);
	
	virtual void Visit(HLSLPrefixNode& node);
	
	virtual void Visit(HLSLPostfixNode& node);
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);
private:
	GLSLSamplerPurger(size_t pass);
	size_t mPass;

	static bool ShouldRemove(const HLSLType& type);

	void RemoveCurrentNode();
	void ReplaceCurrentNode(std::unique_ptr<HLSLNode> node);

	// returns the current value of mShouldRemoveNode
	// and resets mShouldRemoveNode to false
	bool ShouldRemoveChild();
	// if this returns true the caller must
	// move the pointer returned by GetReplacement()
	// to another smart pointer
	bool ShouldReplaceChild();

	template <typename T>
	std::unique_ptr<T> GetReplacement()
	{
		return std::unique_ptr<T>(dynamic_cast<T*>(GetReplacement().release()));
	}

	std::unique_ptr<HLSLNode>& GetReplacement();

	bool mShouldRemoveNode;
	std::unique_ptr<HLSLNode> mReplacementNode;
};

