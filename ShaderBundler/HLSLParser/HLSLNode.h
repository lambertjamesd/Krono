#pragma once

#include "HLSLToken.h"
#include <memory>
#include <vector>

class HLSLNodeVisitor;

class HLSLNode
{
public:
	virtual ~HLSLNode(void);

	const HLSLToken& GetToken() const;

	virtual void Accept(HLSLNodeVisitor& visitor) = 0;
protected:
	HLSLNode(const HLSLToken& token);

	HLSLToken mToken;
};

class HLSLNodeList : public HLSLNode
{
public:
	HLSLNodeList();

	void AddNode(std::unique_ptr<HLSLNode> element);

	size_t GetSize() const;
	HLSLNode& GetElement(size_t index);
	void RemoveElement(size_t index);
	void ReplaceElement(size_t index, std::unique_ptr<HLSLNode> node);
	
	virtual void Accept(HLSLNodeVisitor& visitor);
protected:
	std::vector<std::unique_ptr<HLSLNode> > mElementList;
};