#include "Node.h"
#include "NodeVisitor.h"

namespace preproc
{

Node::Node(void)
{
}


Node::~Node(void)
{
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

OtherNode::OtherNode(const std::string& value) :
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