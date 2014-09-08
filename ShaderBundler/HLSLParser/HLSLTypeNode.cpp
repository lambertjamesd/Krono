#include "HLSLTypeNode.h"
#include "HLSLNodeVisitor.h"
#include "HLSLStatementNode.h"

HLSLTypeNode::HLSLTypeNode(const HLSLToken& token) :
	HLSLNode(token)
{
}


HLSLTypeNode::~HLSLTypeNode(void)
{

}

HLSLVoidNode::HLSLVoidNode(const HLSLToken& token) :
	HLSLTypeNode(token)
{

}

void HLSLVoidNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLNamedTypeNode::HLSLNamedTypeNode(const HLSLToken& token) :
	HLSLTypeNode(token)
{

}

const std::string& HLSLNamedTypeNode::GetName() const
{
	return mToken.GetValue();
}

void HLSLNamedTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLScalarTypeNode::HLSLScalarTypeNode(const HLSLToken& token) :
	HLSLTypeNode(token)
{

}

void HLSLScalarTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLBufferTypeNode::HLSLBufferTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType) :
	HLSLTypeNode(token),
	mInternalType(move(internalType))
{

}

HLSLTypeNode& HLSLBufferTypeNode::GetInternalType()
{
	return *mInternalType;
}

void HLSLBufferTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLArrayTypeNode::HLSLArrayTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t size) :
	HLSLTypeNode(token),
	mInternalType(move(internalType)),
	mSize(size)
{

}

HLSLTypeNode& HLSLArrayTypeNode::GetInternalType()
{
	return *mInternalType;
}

size_t HLSLArrayTypeNode::GetSize() const
{
	return mSize;
}

void HLSLArrayTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLVectorTypeNode::HLSLVectorTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t size) :
	HLSLTypeNode(token),
	mInternalType(move(internalType)),
	mSize(size)
{

}

HLSLTypeNode& HLSLVectorTypeNode::GetInternalType()
{
	return *mInternalType;
}

size_t HLSLVectorTypeNode::GetSize() const
{
	return mSize;
}

void HLSLVectorTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLMatrixTypeNode::HLSLMatrixTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t rows, size_t columns) :
	HLSLTypeNode(token),
	mInternalType(move(internalType)),
	mRows(rows),
	mColumns(columns)
{

}

HLSLTypeNode& HLSLMatrixTypeNode::GetInternalType()
{
	return *mInternalType;
}

size_t HLSLMatrixTypeNode::GetRows() const
{
	return mRows;
}

size_t HLSLMatrixTypeNode::GetColumns() const
{
	return mColumns;
}

void HLSLMatrixTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLTextureTypeNode::HLSLTextureTypeNode(const HLSLToken& token) :
	HLSLTypeNode(token)
{

}

void HLSLTextureTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLSamplerTypeNode::HLSLSamplerTypeNode(const HLSLToken& token) :
	HLSLTypeNode(token)
{

}

void HLSLSamplerTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLStructTypeNode::HLSLStructTypeNode(HLSLStructDefinition& definition) :
	HLSLTypeNode(definition.GetToken()),
	mDefinition(definition)
{

}

const std::string& HLSLStructTypeNode::GetName() const
{
	return mDefinition.GetName();
}

HLSLStructDefinition& HLSLStructTypeNode::GetDefinition()
{
	return mDefinition;
}

void HLSLStructTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}

HLSLFunctionTypeNode::HLSLFunctionTypeNode(HLSLFunctionDefinition& definition) :
	HLSLTypeNode(definition.GetToken()),
	mDefinition(definition)
{

}

const std::string& HLSLFunctionTypeNode::GetName() const
{
	return mDefinition.GetName();
}

HLSLFunctionDefinition& HLSLFunctionTypeNode::GetDefinition()
{
	return mDefinition;
}

void HLSLFunctionTypeNode::Accept(HLSLNodeVisitor& visitor)
{
	visitor.Visit(*this);
}