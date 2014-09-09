#pragma once

#include "HLSLNode.h"
#include "HLSLType.h"

class HLSLTypeNode : public HLSLNode
{
public:
	~HLSLTypeNode(void);

	const HLSLType& GetType() const;
	void ResolveType(const HLSLType& value);
protected:
	HLSLTypeNode(const HLSLToken& token);

	HLSLType mType;
};

class HLSLVoidNode : public HLSLTypeNode
{
public:
	HLSLVoidNode(const HLSLToken& token);

	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLNamedTypeNode : public HLSLTypeNode
{
public:
	HLSLNamedTypeNode(const HLSLToken& token);

	const std::string& GetName() const;
	
	void ResolveType(const HLSLType& value);
	void ResolveType(std::shared_ptr<HLSLTypeNode> type);
	HLSLTypeNode* GetResolvedType();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::shared_ptr<HLSLTypeNode> mResolvedType;
};

class HLSLScalarTypeNode : public HLSLTypeNode
{
public:
	HLSLScalarTypeNode(const HLSLToken& token);

	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLBufferTypeNode : public HLSLTypeNode
{
public:
	HLSLBufferTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType);

	HLSLTypeNode& GetInternalType();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mInternalType;
};

class HLSLArrayTypeNode : public HLSLTypeNode
{
public:
	HLSLArrayTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t size);

	HLSLTypeNode& GetInternalType();
	size_t GetSize() const;

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mInternalType;
	size_t mSize;
};

class HLSLVectorTypeNode : public HLSLTypeNode
{
public:
	HLSLVectorTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t size);

	HLSLTypeNode& GetInternalType();
	size_t GetSize() const;

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mInternalType;
	size_t mSize;
};

class HLSLMatrixTypeNode : public HLSLTypeNode
{
public:
	HLSLMatrixTypeNode(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> internalType, size_t rows, size_t columns);

	HLSLTypeNode& GetInternalType();
	size_t GetRows() const;
	size_t GetColumns() const;

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mInternalType;
	size_t mRows;
	size_t mColumns;
};

class HLSLTextureTypeNode : public HLSLTypeNode
{
public:
	HLSLTextureTypeNode(const HLSLToken& token);

	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLSamplerTypeNode : public HLSLTypeNode
{
public:
	HLSLSamplerTypeNode(const HLSLToken& token);

	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLStructDefinition;

class HLSLStructTypeNode : public HLSLTypeNode
{
public:
	HLSLStructTypeNode(HLSLStructDefinition& definition);

	const std::string& GetName() const;
	HLSLStructDefinition& GetDefinition();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	HLSLStructDefinition& mDefinition;
};

class HLSLFunctionTypeNode : public HLSLTypeNode
{
public:
	HLSLFunctionTypeNode(HLSLFunctionDefinition& definition);

	const std::string& GetName() const;
	HLSLFunctionDefinition& GetDefinition();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	HLSLFunctionDefinition& mDefinition;
};