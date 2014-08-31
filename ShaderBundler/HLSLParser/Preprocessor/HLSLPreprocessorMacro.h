#pragma once

#include <map>
#include <string>
#include <memory>

class HLSLPreprocessorMacro
{
public:
	HLSLPreprocessorMacro(void);
	virtual ~HLSLPreprocessorMacro(void);

	virtual bool HasParameters() = 0;
private:
};

class HLSLConstantPreprocessorMacro : public HLSLPreprocessorMacro
{
public:

private:
};

class HLSLPreprocessorMacroStorage
{
public:

private:
	std::map<std::string, std::unique_ptr<HLSLPreprocessorMacro> > mMacros;
};