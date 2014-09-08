#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Tokenizer.h"

namespace preproc
{

class NodeVisitor;

class Node
{
public:
	virtual ~Node(void);

	virtual void Accept(NodeVisitor& visitor) = 0;

	const Token& GetToken() const;
protected:
	Node(void);
	Node(const Token& token);

	Token mToken;
};

class NodeList : public Node
{
public:
	NodeList(void);
	
	virtual void Accept(NodeVisitor& visitor);

	void AddNode(std::unique_ptr<Node> node);

	const std::vector<std::unique_ptr<Node> >& GetNodeList() const;
protected:
	std::vector<std::unique_ptr<Node> > mNodeList;
};

class OtherNode : public Node
{
public:
	OtherNode(const Token& token);
	OtherNode(const Token& token, const std::string& value);
	virtual void Accept(NodeVisitor& visitor);

	const std::string& GetValue() const;
private:
	std::string mValue;
};

}