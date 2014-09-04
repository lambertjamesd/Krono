#pragma once

#include "Node.h"
#include "DirectiveNode.h"
#include "ExpressionNode.h"
#include "UnaryOperatorNode.h"
#include "BinaryOperatorNode.h"

namespace preproc
{

class NodeVisitor
{
public:
	virtual ~NodeVisitor(void);

	virtual void Visit(Node& node);

	virtual void Visit(OtherNode& node);

	virtual void Visit(DirectiveNode& node);
	virtual void Visit(DefineNode& node);
	virtual void Visit(IfNode& node);
	virtual void Visit(IncludeNode& node);
	virtual void Visit(LineNode& node);
	virtual void Visit(PragmaNode& node);
	virtual void Visit(UnDefNode& node);
	
	virtual void Visit(ExpressionNode& node);
	virtual void Visit(DefinedOperatorNode& node);
	virtual void Visit(IdentifierNode& node);
	virtual void Visit(FunctionNode& node);
	virtual void Visit(ConstantNode& node);

	virtual void Visit(UnaryOperatorNode& node);
	virtual void Visit(NegateOperatorNode& node);
	virtual void Visit(BitwiseNotOperatorNode& node);
	virtual void Visit(BooleanNotOperatorNode& node);
	
	virtual void Visit(BinaryOperatorNode& node);
	
	virtual void Visit(AddNode& node);
	virtual void Visit(SubtractNode& node);
	virtual void Visit(MultiplyNode& node);
	virtual void Visit(DivideNode& node);
	virtual void Visit(ModulusNode& node);
	virtual void Visit(EqualNode& node);
	virtual void Visit(NotEqualNode& node);
	virtual void Visit(GreaterThanNode& node);
	virtual void Visit(LessThanNode& node);
	virtual void Visit(GreaterThanEqualNode& node);
	virtual void Visit(LessThanEqualNode& node);
	virtual void Visit(BooleanAndNode& node);
	virtual void Visit(BooleanOrNode& node);
	virtual void Visit(BitwiseAndNode& node);
	virtual void Visit(BitwiseOrNode& node);
	virtual void Visit(BitwiseXorNode& node);
	virtual void Visit(BitshiftLeftNode& node);
	virtual void Visit(BitshiftRightNode& node);

protected:
	NodeVisitor(void);
};

}