#pragma once

#include <string>
#include <vector>
#include <memory>

class HLSLPreprocessorNodeVisitor;

class HLSLPreprocessorNode
{
public:
	virtual ~HLSLPreprocessorNode(void);

	virtual void Accept(HLSLPreprocessorNodeVisitor& visitor) = 0;
protected:
	HLSLPreprocessorNode(void);
};

class HLSLPreprooessorNodeList : public HLSLPreprocessorNode
{
public:
	HLSLPreprooessorNodeList(void);
	
	virtual void Accept(HLSLPreprocessorNodeVisitor& visitor);

	void AddNode(std::unique_ptr<HLSLPreprocessorNode> node);

	const std::vector<std::unique_ptr<HLSLPreprocessorNode> >& GetNodeList() const;
protected:
	std::vector<std::unique_ptr<HLSLPreprocessorNode> > mNodeList;
};

class HLSLPreprocessorIdentifierNode : public HLSLPreprocessorNode
{
public:
	HLSLPreprocessorIdentifierNode(const std::string& value);
	virtual void Accept(HLSLPreprocessorNodeVisitor& visitor);

	const std::string& GetValue() const;
private:
	std::string mValue;
};

class HLSLPreprocessorOtherNode : public HLSLPreprocessorNode
{
public:
	HLSLPreprocessorOtherNode(const std::string& value);
	virtual void Accept(HLSLPreprocessorNodeVisitor& visitor);

	const std::string& GetValue() const;
private:
	std::string mValue;
};