#pragma once

#include "HLSLPreprocessorNode.h"
#include "HLSLPreprocessorExpressionNode.h"

class HLSLPreprocessorDirectiveNode : public HLSLPreprocessorNode
{
protected:
	HLSLPreprocessorDirectiveNode(void) : HLSLPreprocessorNode() {}
};

class HLSLPreprocessorDefineNode : public HLSLPreprocessorDirectiveNode
{
public:
	HLSLPreprocessorDefineNode(const std::string& name, std::unique_ptr<HLSLPreprocessorNode> value);
	HLSLPreprocessorDefineNode(const std::string& name, const std::vector<std::string>& parameters, std::unique_ptr<HLSLPreprocessorNode> value);

	virtual void Accept(HLSLPreprocessorNodeVisitor& visitor);
protected:
	std::string mName;
	std::vector<std::string> mParameters;
	std::unique_ptr<HLSLPreprocessorNode> mValue;
};

class HLSLPreprocessorElifNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorElseNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorEndIfNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorErrorNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorIfNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorIfDefNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorIfNDefNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorIncludeNode : public HLSLPreprocessorDirectiveNode
{
public:
	enum IncludeType
	{
		LocalInclude,
		SystemInclude
	};

	HLSLPreprocessorIncludeNode(const std::string& filename, IncludeType includeType);
protected:
	std::string mFilename;
	IncludeType mIncludeType;
};

class HLSLPreprocessorLineNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorPragmaNode : public HLSLPreprocessorDirectiveNode
{
public:
};

class HLSLPreprocessorUnDefNode : public HLSLPreprocessorDirectiveNode
{
public:
};