#pragma once

#include "Node.h"
#include "ExpressionNode.h"

namespace preproc
{

class DirectiveNode : public Node
{
protected:
	DirectiveNode(const Token& token) : Node(token) {}
};

class DefineNode : public DirectiveNode
{
public:
	DefineNode(const Token& token, const std::string& name, const std::vector<std::string>& parameters, bool hasParameters, std::unique_ptr<Node> value);

	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetName() const;
	const std::shared_ptr<Node>& GetValue() const;

	bool HasParameters() const;

	const std::vector<std::string>& GetParameters() const;
protected:
	std::string mName;
	std::vector<std::string> mParameters;
	bool mHasParameters;
	std::shared_ptr<Node> mValue;
};

class IfNode : public DirectiveNode
{
public:
	IfNode(const Token& token, std::unique_ptr<Node> expression, std::unique_ptr<Node> body, std::unique_ptr<Node> elseBody);

	virtual void Accept(NodeVisitor& visitor);

	Node& GetExpression();
	Node& GetBody();
	Node* GetElse();
private:
	std::unique_ptr<Node> mExpression;
	std::unique_ptr<Node> mBody;
	std::unique_ptr<Node> mElseBody;
};

class IncludeNode : public DirectiveNode
{
public:
	enum IncludeType
	{
		LocalInclude,
		SystemInclude
	};

	IncludeNode(const Token& token, const std::string& filename, IncludeType includeType);
	
	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetFilename() const;
	IncludeType GetIncludeType() const;
protected:
	std::string mFilename;
	IncludeType mIncludeType;
};

class LineNode : public DirectiveNode
{
public:
	LineNode(const Token& token, size_t line, const std::string& file);
	virtual void Accept(NodeVisitor& visitor);
};

class PragmaNode : public DirectiveNode
{
public:
	PragmaNode(const Token& token);
	virtual void Accept(NodeVisitor& visitor);
};

class UnDefNode : public DirectiveNode
{
public:
	UnDefNode(const Token& token, const std::string& name);
	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetName() const;
private:
	std::string mName;
};

}
