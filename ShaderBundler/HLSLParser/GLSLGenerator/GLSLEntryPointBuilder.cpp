#include "GLSLEntryPointBuilder.h"
#include "HLSLParser/HLSLParserException.h"
#include <algorithm>

using namespace std;

GLSLEntryPointBuilder::GLSLEntryPointBuilder(GLSLGenerator& generator, ShaderType::Type type, HLSLFunctionDefinition* entryPoint) :
	mGenerator(generator),
	mOutput(generator.GetOutput()),
	mShaderType(type),
	mEntryPoint(entryPoint)
{

}

void GLSLEntryPointBuilder::CreateAttributeNames(HLSLStructDefinition& structure, std::map<std::string, HLSLTypeNode*>& names)
{
	for (size_t i = 0; i < structure.GetMemberCount(); ++i)
	{
		HLSLStructureMember& member = structure.GetMember(i);
		CreateAttributeNames(member.GetType(), member.GetSemantic().GetValue(), names);
	}
}

void GLSLEntryPointBuilder::CreateAttributeNames(HLSLFunctionDefinition& function, std::map<std::string, HLSLTypeNode*>& names)
{
	for (size_t i = 0; i < function.GetParameterCount(); ++i)
	{
		HLSLFunctionParameter& parameter = function.GetParameter(i);
		CreateAttributeNames(parameter.GetType(), parameter.GetSemantic().GetValue(), names);
	}
}

void GLSLEntryPointBuilder::CreateAttributeNames(HLSLTypeNode& type, const std::string& semantic, std::map<std::string, HLSLTypeNode*>& names)
{
	HLSLType parameterType = type.GetType();

	if (parameterType.IsPureNumerical())
	{
		if (parameterType.GetType() == HLSLType::Struct)
		{
			CreateAttributeNames(parameterType.GetStructure(), names);
		}
		else
		{
			if (semantic.length() == 0)
			{
				throw HLSLParserException(type.GetToken(), "no semantic specified");
			}
			else
			{
				names[semantic] = &type;
			}
		}
	}
	else
	{
		throw HLSLParserException(type.GetToken(), "only numerical values allowed");
	}
}

void GLSLEntryPointBuilder::GenerateStructureCopy(HLSLStructDefinition& structure, const std::string& structureName, const std::string& prefix, bool assignIntoStruct)
{
	for (size_t i = 0; i < structure.GetMemberCount(); ++i)
	{
		HLSLStructureMember& member = structure.GetMember(i);
		HLSLType type = member.GetType().GetType();

		std::string memberName = structureName + "." + member.GetName();

		if (type.GetType() == HLSLType::Struct)
		{
			GenerateStructureCopy(type.GetStructure(), memberName, prefix, assignIntoStruct);
		}
		else
		{
			const char* glAttributeName = GetGLSemanticName(member.GetSemantic().GetValue());
			std::string attributeName;

			if (glAttributeName)
			{
				attributeName = glAttributeName;
			}
			else
			{
				attributeName = prefix + member.GetSemantic().GetValue();
			}

			if (assignIntoStruct)
			{
				mOutput << '\t' << memberName << " = " << attributeName << ";" << endl;
			}
			else
			{
				mOutput << '\t' << attributeName << " = " << memberName << ";" << endl;
			}
		}
	}
}

void GLSLEntryPointBuilder::OutputParameterInput(const std::string& inputPrefix, HLSLFunctionDefinition& type)
{
	bool isFirst = true;

	for (size_t i = 0; i < type.GetParameterCount(); ++i)
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			mOutput << ", ";
		}

		HLSLFunctionParameter& functionParameter = type.GetParameter(i);

		if (functionParameter.GetType().GetType().GetType() == HLSLType::Struct)
		{
			mOutput << functionParameter.GetName();
		}
		else
		{
			mOutput << inputPrefix << functionParameter.GetSemantic().GetValue();
		}
	}
}

void GLSLEntryPointBuilder::GenerateInOut(bool isInput, const std::string& prefix, const std::map<std::string, HLSLTypeNode*>& names, bool forceLayout)
{
	for (auto it = names.begin(); it != names.end(); ++it)
	{
		if (GetGLSemanticName(it->first) == NULL)
		{
			const std::string& semanticName = it->first;

			if (forceLayout)
			{
				std::string::const_iterator digitPosition = std::find_if(semanticName.begin(), semanticName.end(), ::isdigit);
				size_t index = 0;

				if (digitPosition != semanticName.end())
				{
					index = atoi(semanticName.c_str() + (digitPosition - semanticName.begin()));
				}

				mOutput << "layout(location = " << index << ") ";
			}

			mOutput << (isInput ? "in" : "out") << ' ' ;
			it->second->Accept(mGenerator);
			mOutput << ' ' << prefix << semanticName << ';' << endl;
		}
	}
}

void GLSLEntryPointBuilder::GenerateEntryPoint()
{
	if (mEntryPoint == NULL)
	{
		throw HLSLParserException(HLSLToken(HLSLTokenType::None, "", 0), "entry point not found");
	}

	map<string, HLSLTypeNode*> inputNames;
	CreateAttributeNames(*mEntryPoint, inputNames);

	map<string, HLSLTypeNode*> outputNames;
	CreateAttributeNames(mEntryPoint->GetReturnType(), mEntryPoint->GetSemantic().GetValue(), outputNames);

	std::string inputPrefix;
	std::string outputPrefix;

	if (mShaderType == ShaderType::VertexShader)
	{
		inputPrefix = "attr";
		outputPrefix = "varying";

		GenerateInOut(true, inputPrefix, inputNames);
		mOutput << endl;
		GenerateInOut(false, outputPrefix, outputNames);
	}
	else if (mShaderType == ShaderType::PixelShader)
	{
		inputPrefix = "varying";
		outputPrefix = "target";

		GenerateInOut(true, inputPrefix, inputNames);
		mOutput << endl;
		GenerateInOut(false, outputPrefix, outputNames, true);
	}
	else
	{
		throw HLSLParserException(mEntryPoint->GetToken(), "shader type currently not supported");
	}

	mOutput << "void main()" << endl << '{' << endl;

	for (size_t i = 0; i < mEntryPoint->GetParameterCount(); ++i)
	{
		HLSLFunctionParameter& parameter = mEntryPoint->GetParameter(i);

		if (parameter.GetType().GetType().GetType() == HLSLType::Struct)
		{
			mOutput << '\t';
			parameter.GetType().Accept(mGenerator);
			mOutput << ' ' << parameter.GetName() << ';' << endl;

			GenerateStructureCopy(parameter.GetType().GetType().GetStructure(), parameter.GetName(), inputPrefix, true);
		}
	}

	mOutput << '\t';

	if (mEntryPoint->GetReturnType().GetType().GetType() == HLSLType::Struct)
	{
		mEntryPoint->GetReturnType().Accept(mGenerator);
		mOutput << " result = ";
	}
	else
	{
		const char* glName = GetGLSemanticName(mEntryPoint->GetSemantic().GetValue());

		if (glName == NULL)
		{
			mOutput << outputPrefix << mEntryPoint->GetSemantic().GetValue() << " = ";
		}
		else
		{
			mOutput << glName << " = ";
		}
	}
	
	mOutput << mEntryPoint->GetName() <<'(';

	OutputParameterInput(inputPrefix, *mEntryPoint);

	mOutput << ");" << endl;

	if (mEntryPoint->GetReturnType().GetType().GetType() == HLSLType::Struct)
	{
		GenerateStructureCopy(mEntryPoint->GetReturnType().GetType().GetStructure(), "result", outputPrefix, false);
	}

	mOutput << '}';
}

const char* GLSLEntryPointBuilder::GetGLSemanticName(const std::string& semanticName)
{
	if (semanticName.compare(0, 3, "SV_") == 0)
	{
		std::string lowerName = semanticName;
		std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

		if (lowerName == "sv_position")
		{
			switch (mShaderType)
			{
			case ShaderType::VertexShader:
				return "gl_Position";
			case ShaderType::PixelShader:
				return "gl_FragCoord";
			}
		}
		else if (lowerName == "sv_depth")
		{
			return "gl_Depth";
		}
	}

	return NULL;
}
