#include "EvaluateVisitor.h"
#include "Exception.h"

using namespace std;

namespace preproc
{

EvaluateVisitor::EvaluateVisitor(MacroStorage& macroDB) :
	mMacroDB(macroDB)
{
}


EvaluateVisitor::~EvaluateVisitor(void)
{
}

long EvaluateVisitor::GetResult() const
{
	if (mValueStack.size())
	{
		return mValueStack.top();
	}
	else
	{
		return 0;
	}
}

void EvaluateVisitor::Visit(DefinedOperatorNode& node)
{
	if (mMacroDB.GetMacro(node.GetName()) != NULL)
	{
		mValueStack.push(1);
	}
	else
	{
		mValueStack.push(0);
	}
}

void EvaluateVisitor::Visit(IdentifierNode& node)
{
	mValueStack.push(0);
}

void EvaluateVisitor::Visit(FunctionNode& node)
{
	throw Exception("Undeclared macro");
}

void EvaluateVisitor::Visit(ConstantNode& node)
{
	mValueStack.push(node.GetNumberValue());
}

void EvaluateVisitor::Visit(NegateOperatorNode& node)
{
	node.GetInternalValue().Accept(*this);
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.push(-a);
}

void EvaluateVisitor::Visit(BitwiseNotOperatorNode& node)
{
	node.GetInternalValue().Accept(*this);
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.push(~a);
}

void EvaluateVisitor::Visit(BooleanNotOperatorNode& node)
{
	node.GetInternalValue().Accept(*this);
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.push(!a);
}
	
void EvaluateVisitor::Visit(AddNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a + b);
}

void EvaluateVisitor::Visit(SubtractNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a - b);
}

void EvaluateVisitor::Visit(MultiplyNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a * b);
}

void EvaluateVisitor::Visit(DivideNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a / b);
}

void EvaluateVisitor::Visit(ModulusNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a % b);
}

void EvaluateVisitor::Visit(EqualNode& node)
{	
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a == b);
}
void EvaluateVisitor::Visit(NotEqualNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a != b);
}

void EvaluateVisitor::Visit(GreaterThanNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a > b);
}
void EvaluateVisitor::Visit(LessThanNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a < b);
}

void EvaluateVisitor::Visit(GreaterThanEqualNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a >= b);
}

void EvaluateVisitor::Visit(LessThanEqualNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a <= b);
}

void EvaluateVisitor::Visit(BooleanAndNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a && b);
}

void EvaluateVisitor::Visit(BooleanOrNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a || b);
}

void EvaluateVisitor::Visit(BitwiseAndNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a & b);
}

void EvaluateVisitor::Visit(BitwiseOrNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a | b);
}

void EvaluateVisitor::Visit(BitwiseXorNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a ^ b);
}

void EvaluateVisitor::Visit(BitshiftLeftNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a << b);
}

void EvaluateVisitor::Visit(BitshiftRightNode& node)
{
	node.GetOperandA().Accept(*this);
	node.GetOperandB().Accept(*this);
	long b = mValueStack.top();
	long a = mValueStack.top();
	mValueStack.pop();
	mValueStack.pop();
	mValueStack.push(a >> b);
}

}
