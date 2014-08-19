#pragma once

#include <vector>
#include "BundleDefinition.h"

class GLSLBundler
{
public:
	GLSLBundler(void);
	~GLSLBundler(void);

	std::vector<char> Process(const BundleDefinition& bundleDef) const;
private:
};

