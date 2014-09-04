#pragma once

#include "NodeVisitor.h"
#include <stack>
#include "Macro.h"

namespace preproc
{

class EvaluateVisitor : public NodeVisitor
{
public:
	EvaluateVisitor(MacroStorage& macroDB);
	~EvaluateVisitor(void);

	long GetResult() const;
	
	virtual void Visit(DefinedOperatorNode& node);
	virtual void Visit(IdentifierNode& node);
	virtual void Visit(FunctionNode& node);
	virtual void Visit(ConstantNode& node);

	virtual void Visit(NegateOperatorNode& node);
	virtual void Visit(BitwiseNotOperatorNode& node);
	virtual void Visit(BooleanNotOperatorNode& node);
	
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
private:
	std::stack<long> mValueStack;
	MacroStorage& mMacroDB;
};

}