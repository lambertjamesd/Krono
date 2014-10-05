#include "GLSLSamplerMapper.h"
#include <cassert>

GLSLVariableBinding::GLSLVariableBinding() :
	mBindingMapping(0)
{

}

GLSLVariableBinding::GLSLVariableBinding(size_t variableLocation) :
	mBindingMapping(1 << variableLocation)
{

}

void GLSLVariableBinding::Append(const GLSLVariableBinding& other)
{
	mBindingMapping |= other.mBindingMapping;
}

GLSLSamplerTexturePair::GLSLSamplerTexturePair(size_t samplerLocation, size_t textureLocation) :
	mSamplerLocation(samplerLocation),
	mTextureLocation(textureLocation)
{

}

size_t GLSLSamplerTexturePair::GetSamplerLocation() const
{
	return mSamplerLocation;
}

size_t GLSLSamplerTexturePair::GetTextureLocation() const
{
	return mTextureLocation;
}

bool GLSLSamplerTexturePair::operator<(const GLSLSamplerTexturePair& other) const
{
	if (mSamplerLocation == other.mSamplerLocation)
	{
		return mTextureLocation < other.mTextureLocation;
	}
	else
	{
		return mSamplerLocation < other.mSamplerLocation;
	}
}

///////////////////////////////////////////////////

///////////////////////////////////////////////////

GLSLFunctionMapper::GLSLFunctionMapper(HLSLFunctionDefinition& functionDefinition, const std::vector<GLSLVariableBinding>& parameterBindings)
{
	for (size_t i = 0; i < functionDefinition.GetParameterCount(); ++i)
	{
		UpdateVariableBinding(functionDefinition.GetParameter(i).GetVariableDefinition(), parameterBindings[i]);
	}
}

GLSLFunctionMapper::~GLSLFunctionMapper(void)
{

}

void GLSLFunctionMapper::Visit(HLSLNode& node)
{
	// Do nothing
}

void GLSLFunctionMapper::Visit(HLSLVariableDefinition& node)
{
	if (node.HasInitialValue())
	{
		BeginExpression();
		node.GetInitialValue()->Accept(*this);
		UpdateVariableBinding(node, EndExpression());
	}
}

void GLSLFunctionMapper::Visit(HLSLReturnStatement& node)
{
	if (node.HasReturnValue())
	{
		BeginExpression();
		node.GetReturnValue().Accept(*this);
		mReturnValueBinding.Append(EndExpression());
	}
}

void GLSLFunctionMapper::Visit(HLSLDoNode& node)
{
	node.GetBody().Accept(*this);

	BeginExpression();
	node.GetCondition().Accept(*this);
	EndExpression();
}

void GLSLFunctionMapper::Visit(HLSLIfNode& node)
{
	BeginExpression();
	node.GetCondition().Accept(*this);
	EndExpression();

	node.GetBody().Accept(*this);

	if (node.GetElseBody() != NULL)
	{
		node.GetElseBody()->Accept(*this);
	}
}

void GLSLFunctionMapper::Visit(HLSLForNode& node)
{
	node.GetInitializer().Accept(*this);
	BeginExpression();
	node.GetCondition().Accept(*this);
	EndExpression();
	BeginExpression();
	node.GetModifier().Accept(*this);
	EndExpression();

	node.GetBody().Accept(*this);
}

void GLSLFunctionMapper::Visit(HLSLSwitchNode& node)
{
	BeginExpression();
	node.GetSelectValue().Accept(*this);
	EndExpression();

	node.GetBody().Accept(*this);
}

void GLSLFunctionMapper::Visit(HLSLWhileNode& node)
{
	BeginExpression();
	node.GetCondition().Accept(*this);
	EndExpression();

	node.GetBody().Accept(*this);
}

void GLSLFunctionMapper::Visit(HLSLIdentifierNode& node)
{
	if (node.GetVariableDefinition() != NULL)
	{
		AppendExpression(GetVariableBinding(*node.GetVariableDefinition()));
	}
}

void GLSLFunctionMapper::Visit(HLSLParenthesisNode& node)
{

}

void GLSLFunctionMapper::Visit(HLSLBinaryOperatorNode& node)
{

}

void GLSLFunctionMapper::Visit(HLSLIndexNode& node)
{

}

void GLSLFunctionMapper::Visit(HLSLStructureNode& node)
{

}

void GLSLFunctionMapper::Visit(HLSLFunctionCallNode& node)
{

}

void GLSLFunctionMapper::BeginExpression()
{
	mExpressionBindingStack.push_back(GLSLVariableBinding());
}

void GLSLFunctionMapper::AppendExpression(GLSLVariableBinding binding)
{
	assert(mExpressionBindingStack.size() > 0);
	mExpressionBindingStack.back().Append(binding);
}

GLSLVariableBinding GLSLFunctionMapper::EndExpression()
{
	assert(mExpressionBindingStack.size() > 0);
	GLSLVariableBinding result = mExpressionBindingStack.back();
	mExpressionBindingStack.pop_back();
	return result;
}

GLSLVariableBinding GLSLFunctionMapper::GetVariableBinding(HLSLVariableDefinition& variableDefinition) const
{
	auto findResult = mVariableBindingMapping.find(&variableDefinition);

	if (findResult == mVariableBindingMapping.end())
	{
		if (variableDefinition.GetRegisterLocation().HasValue() &&
			(variableDefinition.GetType().GetType().GetType() == HLSLType::Sampler ||
			variableDefinition.GetType().GetType().GetType() == HLSLType::Texture))
		{
			return GLSLVariableBinding(variableDefinition.GetRegisterLocation().GetRegisterNumber());
		}
		else
		{
			return GLSLVariableBinding();
		}
	}
	else
	{
		return findResult->second;
	}
}

void GLSLFunctionMapper::UpdateVariableBinding(HLSLVariableDefinition& variableDefinition, GLSLVariableBinding binding)
{
	auto findResult = mVariableBindingMapping.find(&variableDefinition);

	if (findResult != mVariableBindingMapping.end())
	{
		findResult->second.Append(binding);
	}
	else
	{
		mVariableBindingMapping.insert( std::make_pair(&variableDefinition, binding) );
	}
}