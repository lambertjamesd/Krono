#pragma once

#include "HLSLNode.h"
#include "HLSLTypeNode.h"

class HLSLExpressionNode;

class HLSLStatementNode : public HLSLNode
{
public:
	~HLSLStatementNode(void);
protected:
	HLSLStatementNode(const HLSLToken& token);
};

class HLSLStatementBlock : public HLSLNodeList
{
public:
	HLSLStatementBlock();

	virtual void Accept(HLSLNodeVisitor& visitor);
};

class HLSLTypedefDefinition : public HLSLStatementNode
{
public:
	HLSLTypedefDefinition(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> type, const std::string& name);

	const std::shared_ptr<HLSLTypeNode>& GetType() const;
	const std::string& GetName() const;

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::shared_ptr<HLSLTypeNode> mType;
	std::string mName;
};

class HLSLRegisterLocation
{
public:
	HLSLRegisterLocation();
	HLSLRegisterLocation(const std::string& name, size_t registerNumber, size_t index);

	bool HasValue() const;

	const std::string& GetName() const;
	size_t GetRegisterNumber() const;
	size_t GetIndex() const;
private:
	std::string mName;
	size_t mRegisterNumber;
	size_t mIndex;
};

namespace VariableStorageClass
{
	enum Type
	{
		None,
		Extern = (1 << 0),
		NoInterpolation = (1 << 1),
		Precise = (1 << 2),
		Shared = (1 << 3),
		GroupShared = (1 << 4),
		Static = (1 << 5),
		Uniform = (1 << 6),
		Volatile = (1 << 7)
	};
}

namespace VariableTypeModifer
{
	enum Type
	{
		None,
		Const = (1 << 0),
		RowMajor = (1 << 1),
		ColumnMajor = (1 << 2),
	};
}

class HLSLVariableDefinition : public HLSLStatementNode
{
public:
	HLSLVariableDefinition(const HLSLToken& token, 
		VariableStorageClass::Type storageClass, 
		VariableTypeModifer::Type typeModifier, 
		std::unique_ptr<HLSLTypeNode> type,
		const std::string& name);

	void SetSemantic(const std::string& semantic);
	void SetRegisterLocation(const HLSLRegisterLocation& value);
	void SetInitialValue(std::unique_ptr<HLSLExpressionNode> value);

	VariableStorageClass::Type GetStorageClass() const;
	VariableTypeModifer::Type GetTypeModifier() const;

	HLSLTypeNode& GetType();
	const std::shared_ptr<HLSLTypeNode>& GetTypePtr() const;
	const std::string& GetName() const;
	const std::string& GetSemantic() const;
	const HLSLRegisterLocation& GetRegisterLocation() const;
	HLSLExpressionNode* GetInitialValue();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	VariableStorageClass::Type mStorageClass; 
	VariableTypeModifer::Type mTypeModifier; 
	std::shared_ptr<HLSLTypeNode> mType;
	std::string mName;

	std::string mSemantic;
	HLSLRegisterLocation mRegisterLocation;
	std::unique_ptr<HLSLExpressionNode> mInitialValue;
};

class HLSLFunctionParameter : public HLSLNode
{
public:
	enum InputModifier
	{
		None,
		In,
		Out,
		InOut,
		Uniform
	};

	HLSLFunctionParameter(const HLSLToken& token, InputModifier inputModifer, std::unique_ptr<HLSLTypeNode> type, const std::string& name);

	void SetSemantic(const std::string& value);
	void SetInterpolationMode(InterpolationMode::Type value);
	void SetInitialzier(std::unique_ptr<HLSLExpressionNode> value);

	InputModifier GetInputModifier() const;
	HLSLTypeNode& GetType();
	const std::string& GetName() const;
	const std::string& GetSemantic() const;
	InterpolationMode::Type GetInterpolationMode() const;
	HLSLExpressionNode* GetInitializer();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	InputModifier mInputModifier;
	std::unique_ptr<HLSLTypeNode> mType;
	std::string mName;
	std::string mSemantic;
	InterpolationMode::Type mInterpolationMode;
	std::unique_ptr<HLSLExpressionNode> mInitializer;
};

class HLSLFunctionDefinition : public HLSLStatementNode
{
public:
	HLSLFunctionDefinition(const HLSLToken& token, std::unique_ptr<HLSLTypeNode> returnType, const std::string& name);

	void AddParameter(std::unique_ptr<HLSLFunctionParameter> value);

	void SetSemantic(const std::string& value);
	void SetBody(std::unique_ptr<HLSLStatementBlock> value);
	
	HLSLTypeNode& GetReturnType() const;
	const std::string& GetName() const;
	size_t GetParameterCount() const;
	HLSLFunctionParameter& GetParameter(size_t index);
	HLSLStatementBlock* GetBody();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLTypeNode> mReturnType;
	std::string mName;

	std::vector<std::unique_ptr<HLSLFunctionParameter> > mParameters;
	std::string mSemantic;

	std::unique_ptr<HLSLStatementBlock> mBody;
};

class HLSLReturnStatement : public HLSLStatementNode
{
public:
	HLSLReturnStatement(const HLSLToken& token, std::unique_ptr<HLSLExpressionNode> returnValue);

	HLSLExpressionNode& GetReturnValue();
	
	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::unique_ptr<HLSLExpressionNode> mReturnValue;
};

class HLSLStructureMember : public HLSLNode
{
public:
	HLSLStructureMember(const HLSLToken& token, InterpolationMode::Type interpolationMode, std::unique_ptr<HLSLTypeNode> type, const std::string& name);

	void SetSemantic(const std::string& value);
	
	InterpolationMode::Type GetInterpolationMode() const;
	HLSLTypeNode& GetType();
	const std::string& GetName();
	const std::string& GetSemantic();

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	InterpolationMode::Type mInterpolationMode;
	std::unique_ptr<HLSLTypeNode> mType;
	std::string mName;

	std::string mSemantic;
};

class HLSLStructDefinition : public HLSLStatementNode
{
public:
	HLSLStructDefinition(const HLSLToken& token, const std::string& name);

	void AddMember(std::unique_ptr<HLSLStructureMember> member);

	const std::string& GetName() const;
	size_t GetMemberCount() const;
	HLSLStructureMember& GetMember(size_t index);

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::string mName;
	std::vector<std::unique_ptr<HLSLStructureMember> > mMemberList;
};

class HLSLCBufferDefinition : public HLSLStatementNode
{
public:
	HLSLCBufferDefinition(const HLSLToken& token, const std::string& name);

	void SetRegister(const HLSLRegisterLocation& registerLocation);

	void AddMember(std::unique_ptr<HLSLVariableDefinition> member);

	const std::string& GetName() const;
	const HLSLRegisterLocation& GetRegisterLocation() const;
	
	size_t GetMemberCount() const;
	HLSLVariableDefinition& GetMember(size_t index);

	virtual void Accept(HLSLNodeVisitor& visitor);
private:
	std::string mName;
	HLSLRegisterLocation mRegisterLocation;
	std::vector<std::unique_ptr<HLSLVariableDefinition> > mMemberList;
};