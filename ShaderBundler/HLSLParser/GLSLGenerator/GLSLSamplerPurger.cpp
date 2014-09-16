#include "GLSLSamplerPurger.h"
#include <cassert>

using namespace std;

GLSLSamplerPurger::GLSLSamplerPurger(size_t pass) :
	mPass(pass),
	mShouldRemoveNode(false)
{
}


GLSLSamplerPurger::~GLSLSamplerPurger(void)
{
}

void GLSLSamplerPurger::Purge(HLSLNode& node)
{
	GLSLSamplerPurger firstStage(0);
	node.Accept(firstStage);
	GLSLSamplerPurger secondStage(1);
	node.Accept(secondStage);
}

void GLSLSamplerPurger::Visit(HLSLNode& node)
{
	// do nothing
}

void GLSLSamplerPurger::Visit(HLSLNodeList& node)
{
	for (size_t i = 0; i < node.GetSize();)
	{
		node.GetElement(i).Accept(*this);

		if (ShouldRemoveChild())
		{
			node.RemoveElement(i);
		}
		else
		{
			if (ShouldReplaceChild())
			{
				node.ReplaceElement(i, move(GetReplacement()));
			}

			++i;
		}
	}
}

void GLSLSamplerPurger::Visit(HLSLVariableDefinition& node)
{
	if (ShouldRemove(node.GetType().GetType()))
	{
		RemoveCurrentNode();
	}

	if (node.GetInitialValue() != NULL)
	{
		node.GetInitialValue()->Accept(*this);
	}
}

void GLSLSamplerPurger::Visit(HLSLFunctionParameter& node)
{
	if (node.IsOptional())
	{
		node.GetInitializer()->Accept(*this);
	}

	if (ShouldRemove(node.GetType().GetType()))
	{
		RemoveCurrentNode();
	}
}

void GLSLSamplerPurger::Visit(HLSLFunctionDefinition& node)
{
	// remove defintion only if on second pass
	if (mPass == 1 && ShouldRemove(node.GetReturnType().GetType()))
	{
		RemoveCurrentNode();
	}
	else
	{
		for (size_t i = 0; i < node.GetParameterCount();)
		{
			node.GetParameter(i).Accept(*this);
			
			if (ShouldRemoveChild())
			{
				node.RemoveParameter(i);
			}
			else if (ShouldReplaceChild())
			{
				node.ReplaceParameter(i, move(GetReplacement<HLSLFunctionParameter>()));
				++i;
			}
			else
			{
				++i;
			}
		}

		if (node.GetBody() != NULL)
		{
			node.GetBody()->Accept(*this);
		}
	}
}

void GLSLSamplerPurger::Visit(HLSLReturnStatement& node)
{
	node.GetReturnValue().Accept(*this);
}
	
void GLSLSamplerPurger::Visit(HLSLStructureMember& node)
{
	if (ShouldRemove(node.GetType().GetType()))
	{
		RemoveCurrentNode();
	}
}

void GLSLSamplerPurger::Visit(HLSLStructDefinition& node)
{
	for (size_t i = 0; i < node.GetMemberCount();)
	{
		node.GetMember(i).Accept(*this);

		if (ShouldRemoveChild())
		{
			node.RemoveMember(i);
		}
		else if (ShouldReplaceChild())
		{
			node.ReplaceMember(i, move(GetReplacement<HLSLStructureMember>()));
			++i;
		}
		else
		{
			++i;
		}
	}

	if (mPass == 1 && node.GetMemberCount() == 0)
	{
		RemoveCurrentNode();
	}
}

void GLSLSamplerPurger::Visit(HLSLDoNode& node)
{
	node.GetCondition().Accept(*this);
	node.GetBody().Accept(*this);
}

void GLSLSamplerPurger::Visit(HLSLIfNode& node)
{
	node.GetCondition().Accept(*this);
	node.GetBody().Accept(*this);

	if (node.GetElseBody() != NULL)
	{
		node.GetElseBody()->Accept(*this);
	}
}

void GLSLSamplerPurger::Visit(HLSLForNode& node)
{
	node.GetInitializer().Accept(*this);
	node.GetCondition().Accept(*this);
	node.GetModifier().Accept(*this);
	node.GetBody().Accept(*this);
}

void GLSLSamplerPurger::Visit(HLSLSwitchNode& node)
{
	node.GetSelectValue().Accept(*this);
	node.GetBody().Accept(*this);
}

void GLSLSamplerPurger::Visit(HLSLWhileNode& node)
{
	node.GetCondition().Accept(*this);
	node.GetBody().Accept(*this);
}
	
void GLSLSamplerPurger::Visit(HLSLExpressionNode& node)
{
	if (ShouldRemove(node.GetType()))
	{
		RemoveCurrentNode();
	}
}

void GLSLSamplerPurger::Visit(HLSLParenthesisNode& node)
{
	if (ShouldRemove(node.GetType()))
	{
		RemoveCurrentNode();
	}
	else
	{
		node.GetValue().Accept(*this);
	}
}
	
void GLSLSamplerPurger::Visit(HLSLBinaryOperatorNode& node)
{
	bool removeLeft = false;
	bool removeRight = false;

	node.GetLeft().Accept(*this);

	if (ShouldRemoveChild())
	{
		removeLeft = true;
	}
	else if (ShouldReplaceChild())
	{
		node.SetLeft(move(GetReplacement<HLSLExpressionNode>()));
	}

	node.GetRight().Accept(*this);

	if (ShouldRemoveChild())
	{
		removeRight = true;
	}
	else if (ShouldReplaceChild())
	{
		node.SetRight(move(GetReplacement<HLSLExpressionNode>()));
	}

	if (removeLeft && removeRight)
	{
		RemoveCurrentNode();
	}
	else if (removeLeft)
	{
		ReplaceCurrentNode(move(node.TakeRight()));
	}
	else if (removeRight)
	{
		ReplaceCurrentNode(move(node.TakeLeft()));
	}
}
	
void GLSLSamplerPurger::Visit(HLSLUnaryOperator& node)
{
	node.GetInnerExpression().Accept(*this);
	
	// no unary operates on sampler type so there is no need
	// to replace the inner contents
	assert(!ShouldRemoveChild() && !ShouldReplaceChild());
}

void GLSLSamplerPurger::Visit(HLSLCastNode& node)
{
	node.GetRight().Accept(*this);

	if (ShouldRemoveChild() || ShouldRemove(node.GetType().GetType()))
	{
		RemoveCurrentNode();
	}
	else if (ShouldReplaceChild())
	{
		ReplaceCurrentNode(move(GetReplacement()));
	}
}
	
void GLSLSamplerPurger::Visit(HLSLPrefixNode& node)
{
	node.GetRight().Accept(*this);

	// no prefix operator applies to samplers
	assert(!ShouldRemoveChild() && !ShouldReplaceChild());
}
	
void GLSLSamplerPurger::Visit(HLSLPostfixNode& node)
{
	node.GetLeft().Accept(*this);

	// no postfix operator applies to samplers
	assert(!ShouldRemoveChild() && !ShouldReplaceChild());
}

void GLSLSamplerPurger::Visit(HLSLIndexNode& node)
{
	bool remove = false;

	node.GetLeft().Accept(*this);

	if (ShouldRemoveChild())
	{
		RemoveCurrentNode();
		return;
	}
	else if (ShouldReplaceChild())
	{
		node.SetLeft(move(GetReplacement<HLSLExpressionNode>()));
	}

	node.GetIndex().Accept(*this);
	// the index is an integer, not a sampler
	assert(!ShouldRemoveChild() && !ShouldReplaceChild());
}

void GLSLSamplerPurger::Visit(HLSLStructureNode& node)
{
	node.GetLeft().Accept(*this);

	// samplers shouldn't have function members
	assert(!ShouldRemoveChild());

	if (ShouldReplaceChild())
	{
		node.SetLeft(GetReplacement<HLSLExpressionNode>());
	}

	if (ShouldRemove(node.GetType()))
	{
		RemoveCurrentNode();
	}
}

void GLSLSamplerPurger::Visit(HLSLFunctionCallNode& node)
{
	if (ShouldRemove(node.GetType()))
	{
		RemoveCurrentNode();
		return;
	}

	node.GetLeft().Accept(*this);
	
	// samplers shouldn't be called as a function
	assert(!ShouldRemoveChild());

	if (ShouldReplaceChild())
	{
		node.SetLeft(GetReplacement<HLSLExpressionNode>());
	}

	for (size_t i = 0; i < node.GetParameterCount();)
	{
		node.GetParameter(i).Accept(*this);

		if (ShouldRemoveChild())
		{
			node.RemoveParameter(i);
		}
		else if (ShouldReplaceChild())
		{
			node.ReplaceParameter(i, move(GetReplacement<HLSLExpressionNode>()));
			++i;
		}
		else
		{
			++i;
		}
	}
}

bool GLSLSamplerPurger::ShouldRemove(const HLSLType& type)
{
	return type.GetType() == HLSLType::Sampler ||
		(type.GetType() == HLSLType::Struct && type.GetStructure().GetMemberCount() == 0);
}

void GLSLSamplerPurger::RemoveCurrentNode()
{
	assert(!mShouldRemoveNode);
	mShouldRemoveNode = true;
}

void GLSLSamplerPurger::ReplaceCurrentNode(std::unique_ptr<HLSLNode> node)
{
	assert(!mShouldRemoveNode && mReplacementNode == NULL);
	mReplacementNode = move(node);
}

bool GLSLSamplerPurger::ShouldRemoveChild()
{
	bool result = mShouldRemoveNode;
	mShouldRemoveNode = false;
	return result;
}

bool GLSLSamplerPurger::ShouldReplaceChild()
{
	return mReplacementNode != NULL;
}

std::unique_ptr<HLSLNode>& GLSLSamplerPurger::GetReplacement()
{
	return mReplacementNode;
}