#include "Node.h"
#include "NodeVisitor.h"

namespace preproc
{

Node::Node(void) :
	mToken(Token::None, "", 0)
{
}

Node::Node(const Token& token) :
	mToken(token)
{
}

Node::~Node(void)
{
}

const Token& Node::GetToken() const
{
	return mToken;
}

NodeList::NodeList(void)
{

}
	
void NodeList::Accept(NodeVisitor& visitor)
{
	for (auto it = mNodeList.begin(); it != mNodeList.end(); ++it)
	{
		(*it)->Accept(visitor);
	}
}

void NodeList::AddNode(std::unique_ptr<Node> node)
{
	mNodeList.push_back(move(node));
}

OtherNode::OtherNode(const Token& token) :
	Node(token),
	mValue(token.GetValue())
{

}

OtherNode::OtherNode(const Token& token, const std::string& value) :
	Node(token),
	mValue(value)
{

}

void OtherNode::Accept(NodeVisitor& visitor)
{
	visitor.Visit(*this);
}

const std::string& OtherNode::GetValue() const
{
	return mValue;
}

}