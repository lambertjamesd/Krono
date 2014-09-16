#pragma once

#include "HLSLParser/HLSLTypeVisitor.h"
#include <vector>
#include <string>

class HLSLBuiltInFunctions
{
public:
	static void PopulateVisitor(HLSLTypeVisitor& typeVisitor);
private:
};

