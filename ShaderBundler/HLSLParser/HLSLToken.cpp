#include "HLSLToken.h"
#include "HLSLParserException.h"

using namespace std;

namespace HLSLKeyword
{
	bool IsScalarType(Type keyword)
	{
		return keyword >= Bool && keyword <= Min16uint;
	}

	bool IsTextureType(Type keyword)
	{
		return keyword >= Texture && keyword <= TextureCubeArray;
	}
}

HLSLToken::HLSLToken(HLSLTokenType::Type type, const std::string& value, size_t lineNumber) :
	mType(type),
	mKeywordType(HLSLKeyword::None),
	mValue(value),
	mLineNumber(lineNumber)
{
	if (mType == HLSLTokenType::Identifier)
	{
		mKeywordType = GetKeywordType(value);

		if (mKeywordType != HLSLKeyword::None)
		{
			mType = HLSLTokenType::Keyword;
		}
	}
}

HLSLToken::~HLSLToken(void)
{

}

bool HLSLToken::IsBinaryOperator() const
{
	return 
		(mType >= HLSLTokenType::Add && mType <= HLSLTokenType::MinusEqual) ||
		(mType >= HLSLTokenType::LeftShift && mType <= HLSLTokenType::BooleanOr) ||
		(mType >= HLSLTokenType::Comma && mType <= HLSLTokenType::GreaterThanEqual);
}

bool HLSLToken::IsUnaryOperator() const
{
	return mType == HLSLTokenType::Add || mType == HLSLTokenType::Minus ||
		mType == HLSLTokenType::BitwiseNot || mType == HLSLTokenType::BooleanNot ||
		mType == HLSLTokenType::PlusPlus || mType == HLSLTokenType::MinusMinus;
}

bool HLSLToken::IsPrefixOperator() const
{
	return mType == HLSLTokenType::PlusPlus || mType == HLSLTokenType::MinusMinus;
}

bool HLSLToken::IsPostfixOperator() const
{
	return mType == HLSLTokenType::PlusPlus || mType == HLSLTokenType::MinusMinus ||
		mType == HLSLTokenType::OpenSquare || mType == HLSLTokenType::StructureOperator ||
		mType == HLSLTokenType::OpenParen;
}
	
bool HLSLToken::IsFunctionStorageClass() const
{
	return mType == HLSLTokenType::Identifier && (
		mValue == "inline");
}

bool HLSLToken::IsVariableStorageClass() const
{
	return mType == HLSLTokenType::Identifier && (
		mValue == "extern" ||
		mValue == "nointerpolation" ||
		mValue == "precise" ||
		mValue == "shared" ||
		mValue == "groupshared" ||
		mValue == "static" ||
		mValue == "uniform" ||
		mValue == "volatile");
}

bool HLSLToken::IsVariableTypeModifier() const
{
	return mType == HLSLTokenType::Identifier && (
		mValue == "const" ||
		mValue == "row_major" ||
		mValue == "column_major");
}

bool HLSLToken::IsKeyword() const
{
	return IsFunctionStorageClass() || 
		IsVariableStorageClass() ||
		IsVariableTypeModifier();
}


OperatorPrecedence::Type HLSLToken::GetOperatorPrecedence() const
{
	switch (mType)
	{
	case HLSLTokenType::Comma:
		return OperatorPrecedence::Comma;

	case HLSLTokenType::Assign:
	case HLSLTokenType::AddEqual:
	case HLSLTokenType::MinusEqual:
	case HLSLTokenType::MultiplyEqual:
	case HLSLTokenType::DivideEqual:
	case HLSLTokenType::ModulusEqual:
	case HLSLTokenType::LeftShiftEqual:
	case HLSLTokenType::RightShiftEqual:
	case HLSLTokenType::BitwiseAndEqual:
	case HLSLTokenType::BitwiseXorEqual:
	case HLSLTokenType::BitwiseOrEqual:
		return OperatorPrecedence::Assign;
		
	case HLSLTokenType::TernaryOperator:
		return OperatorPrecedence::Ternary;
		
	case HLSLTokenType::BooleanOr:
		return OperatorPrecedence::BooleanOr;
		
	case HLSLTokenType::BooleanAnd:
		return OperatorPrecedence::BooleanAnd;
		
	case HLSLTokenType::BitwiseOr:
		return OperatorPrecedence::BitwiseOr;
		
	case HLSLTokenType::BitwiseXor:
		return OperatorPrecedence::BitwiseXor;
		
	case HLSLTokenType::BitwiseAnd:
		return OperatorPrecedence::BitwiseAnd;
		
	case HLSLTokenType::Equal:
	case HLSLTokenType::NotEqual:
		return OperatorPrecedence::Equal;
		
	case HLSLTokenType::LessThan:
	case HLSLTokenType::LessThanEqual:
	case HLSLTokenType::GreaterThan:
	case HLSLTokenType::GreaterThanEqual:
		return OperatorPrecedence::Compare;
		
	case HLSLTokenType::LeftShift:
	case HLSLTokenType::RightShift:
		return OperatorPrecedence::Bitshift;
		
	case HLSLTokenType::Add:
	case HLSLTokenType::Minus:
		return OperatorPrecedence::Addition;
		
	case HLSLTokenType::Multiply:
	case HLSLTokenType::Divide:
	case HLSLTokenType::Modulus:
		return OperatorPrecedence::Multiply;

	default:
		return OperatorPrecedence::None;
	}
}

HLSLTokenType::Type HLSLToken::GetType() const
{
	return mType;
}

HLSLKeyword::Type HLSLToken::GetKeywordType() const
{
	return mKeywordType;
}

const std::string& HLSLToken::GetValue() const
{
	return mValue;
}
	
size_t HLSLToken::GetLineNumber() const
{
	return mLineNumber;
}

int HLSLToken::GetIntValue() const
{
	if (mType != HLSLTokenType::Number)
	{
		throw HLSLParserException(*this);
	}
	else if (mValue.find('.') != std::string::npos)
	{
		throw HLSLParserException(*this, "number must be an integer");
	}
	else
	{
		return atoi(mValue.c_str());
	}
}

bool HLSLToken::operator==(const HLSLToken& other) const
{
	return mType == other.mType && mValue == other.mValue;
}


std::map<std::string, HLSLKeyword::Type>* HLSLToken::gKeywordMapping;

HLSLKeyword::Type HLSLToken::GetKeywordType(const std::string& name)
{
	if (gKeywordMapping == NULL)
	{
		gKeywordMapping = new map<string, HLSLKeyword::Type>();
		gKeywordMapping->insert(make_pair(string("AppendStructuredBuffer"), HLSLKeyword::AppendStructuredBuffer));
		gKeywordMapping->insert(make_pair(string("asm"), HLSLKeyword::ASM));
		gKeywordMapping->insert(make_pair(string("asm_fragment"), HLSLKeyword::ASM_fragment));
	
		gKeywordMapping->insert(make_pair(string("BlendState"), HLSLKeyword::BlendState));
		gKeywordMapping->insert(make_pair(string("bool"), HLSLKeyword::Bool));
		gKeywordMapping->insert(make_pair(string("break"), HLSLKeyword::Break));
		gKeywordMapping->insert(make_pair(string("Buffer"), HLSLKeyword::Buffer));
		gKeywordMapping->insert(make_pair(string("ByteAddressBuffer"), HLSLKeyword::ByteAddressBuffer));
	
		gKeywordMapping->insert(make_pair(string("case"), HLSLKeyword::Case));
		gKeywordMapping->insert(make_pair(string("cbuffer"), HLSLKeyword::Cbuffer));
		gKeywordMapping->insert(make_pair(string("centroid"), HLSLKeyword::Centroid));
		gKeywordMapping->insert(make_pair(string("class"), HLSLKeyword::Class));
		gKeywordMapping->insert(make_pair(string("column_major"), HLSLKeyword::Column_major));
		gKeywordMapping->insert(make_pair(string("compile"), HLSLKeyword::Compile));
		gKeywordMapping->insert(make_pair(string("compile_fragment"), HLSLKeyword::Compile_fragment));
		gKeywordMapping->insert(make_pair(string("CompileShader"), HLSLKeyword::CompileShader));
		gKeywordMapping->insert(make_pair(string("const"), HLSLKeyword::Const));
		gKeywordMapping->insert(make_pair(string("continue"), HLSLKeyword::Continue));
		gKeywordMapping->insert(make_pair(string("ComputeShader"), HLSLKeyword::ComputeShader));
		gKeywordMapping->insert(make_pair(string("ConsumeStructedBuffer"), HLSLKeyword::ConsumeStructedBuffer));
	
		gKeywordMapping->insert(make_pair(string("default"), HLSLKeyword::Default));
		gKeywordMapping->insert(make_pair(string("DepthStencilState"), HLSLKeyword::DepthStencilState));
		gKeywordMapping->insert(make_pair(string("DepthStencilView"), HLSLKeyword::DepthStencilView));
		gKeywordMapping->insert(make_pair(string("discard"), HLSLKeyword::Discard));
		gKeywordMapping->insert(make_pair(string("do"), HLSLKeyword::Do));
		gKeywordMapping->insert(make_pair(string("double"), HLSLKeyword::Double));
		gKeywordMapping->insert(make_pair(string("DomainShader"), HLSLKeyword::DomainShader));
		gKeywordMapping->insert(make_pair(string("dword"), HLSLKeyword::Dword));
	
		gKeywordMapping->insert(make_pair(string("else"), HLSLKeyword::Else));
		gKeywordMapping->insert(make_pair(string("export"), HLSLKeyword::Export));
		gKeywordMapping->insert(make_pair(string("extern"), HLSLKeyword::Extern));
	
		gKeywordMapping->insert(make_pair(string("false"), HLSLKeyword::False));
		gKeywordMapping->insert(make_pair(string("float"), HLSLKeyword::Float));
		gKeywordMapping->insert(make_pair(string("for"), HLSLKeyword::For));
		gKeywordMapping->insert(make_pair(string("fxgroup"), HLSLKeyword::Fxgroup));
	
		gKeywordMapping->insert(make_pair(string("GeometryShader"), HLSLKeyword::GeometryShader));
		gKeywordMapping->insert(make_pair(string("groupshared"), HLSLKeyword::Groupshared));
	
		gKeywordMapping->insert(make_pair(string("half"), HLSLKeyword::Half));
		gKeywordMapping->insert(make_pair(string("Hullshader"), HLSLKeyword::Hullshader));
	
		gKeywordMapping->insert(make_pair(string("if"), HLSLKeyword::If));
		gKeywordMapping->insert(make_pair(string("in"), HLSLKeyword::In));
		gKeywordMapping->insert(make_pair(string("inline"), HLSLKeyword::Inline));
		gKeywordMapping->insert(make_pair(string("inout"), HLSLKeyword::Inout));
		gKeywordMapping->insert(make_pair(string("InputPatch"), HLSLKeyword::InputPatch));
		gKeywordMapping->insert(make_pair(string("int"), HLSLKeyword::Int));
		gKeywordMapping->insert(make_pair(string("interface"), HLSLKeyword::Interface));

		gKeywordMapping->insert(make_pair(string("line"), HLSLKeyword::Line));
		gKeywordMapping->insert(make_pair(string("lineadj"), HLSLKeyword::Lineadj));
		gKeywordMapping->insert(make_pair(string("linear"), HLSLKeyword::Linear));
		gKeywordMapping->insert(make_pair(string("LineStream"), HLSLKeyword::LineStream));
	
		gKeywordMapping->insert(make_pair(string("matrix"), HLSLKeyword::Matrix));
		gKeywordMapping->insert(make_pair(string("min16float"), HLSLKeyword::Min16float));
		gKeywordMapping->insert(make_pair(string("min10float"), HLSLKeyword::Min10float));
		gKeywordMapping->insert(make_pair(string("min16int"), HLSLKeyword::Min16int));
		gKeywordMapping->insert(make_pair(string("min12int"), HLSLKeyword::Min12int));
		gKeywordMapping->insert(make_pair(string("min16uint"), HLSLKeyword::Min16uint));

		gKeywordMapping->insert(make_pair(string("namespace"), HLSLKeyword::Namespace));
		gKeywordMapping->insert(make_pair(string("nointerpolation"), HLSLKeyword::Nointerpolation));
		gKeywordMapping->insert(make_pair(string("noperspective"), HLSLKeyword::Noperspective));
		gKeywordMapping->insert(make_pair(string("NULL"), HLSLKeyword::Null));

		gKeywordMapping->insert(make_pair(string("out"), HLSLKeyword::Out));
		gKeywordMapping->insert(make_pair(string("OutputPatch"), HLSLKeyword::OutputPatch));
	
		gKeywordMapping->insert(make_pair(string("packoffset"), HLSLKeyword::Packoffset));
		gKeywordMapping->insert(make_pair(string("pass"), HLSLKeyword::Pass));
		gKeywordMapping->insert(make_pair(string("pixelfragment"), HLSLKeyword::Pixelfragment));
		gKeywordMapping->insert(make_pair(string("PixelShader"), HLSLKeyword::PixelShader));
		gKeywordMapping->insert(make_pair(string("point"), HLSLKeyword::Point));
		gKeywordMapping->insert(make_pair(string("PointStream"), HLSLKeyword::PointStream));
		gKeywordMapping->insert(make_pair(string("precise"), HLSLKeyword::Precise));

		gKeywordMapping->insert(make_pair(string("RasterizerState"), HLSLKeyword::RasterizerState));
		gKeywordMapping->insert(make_pair(string("RenderTargetView"), HLSLKeyword::RenderTargetView));
		gKeywordMapping->insert(make_pair(string("return"), HLSLKeyword::Return));
		gKeywordMapping->insert(make_pair(string("register"), HLSLKeyword::Register));
		gKeywordMapping->insert(make_pair(string("row_major"), HLSLKeyword::Row_major));
		gKeywordMapping->insert(make_pair(string("RWBuffer"), HLSLKeyword::RWBuffer));
		gKeywordMapping->insert(make_pair(string("RWByteAddressBuffer"), HLSLKeyword::RWByteAddressBuffer));
		gKeywordMapping->insert(make_pair(string("RWStructedBuffer"), HLSLKeyword::RWStructedBuffer));
		gKeywordMapping->insert(make_pair(string("RWTexture1D"), HLSLKeyword::RWTexture1D));
		gKeywordMapping->insert(make_pair(string("RWTexture1DArray"), HLSLKeyword::RWTexture1DArray));
		gKeywordMapping->insert(make_pair(string("RWTexture2D"), HLSLKeyword::RWTexture2D));
		gKeywordMapping->insert(make_pair(string("RWTexture2DArray"), HLSLKeyword::RWTexture2DArray));
		gKeywordMapping->insert(make_pair(string("RWTexture3D"), HLSLKeyword::RWTexture3D));

		gKeywordMapping->insert(make_pair(string("sample"), HLSLKeyword::Sample));
		gKeywordMapping->insert(make_pair(string("sampler"), HLSLKeyword::Sampler));
		gKeywordMapping->insert(make_pair(string("SamplerState"), HLSLKeyword::SamplerState));
		gKeywordMapping->insert(make_pair(string("SamplerComparisonState"), HLSLKeyword::SamplerComparisonState));
		gKeywordMapping->insert(make_pair(string("shared"), HLSLKeyword::Shared));
		gKeywordMapping->insert(make_pair(string("snorm"), HLSLKeyword::Snorm));
		gKeywordMapping->insert(make_pair(string("stateblock"), HLSLKeyword::Stateblock));
		gKeywordMapping->insert(make_pair(string("stateblock_state"), HLSLKeyword::Stateblock_state));
		gKeywordMapping->insert(make_pair(string("static"), HLSLKeyword::Static));
		gKeywordMapping->insert(make_pair(string("string"), HLSLKeyword::String));
		gKeywordMapping->insert(make_pair(string("struct"), HLSLKeyword::Struct));
		gKeywordMapping->insert(make_pair(string("switch"), HLSLKeyword::Switch));
		gKeywordMapping->insert(make_pair(string("StructedBuffer"), HLSLKeyword::StructedBuffer));

		gKeywordMapping->insert(make_pair(string("tbuffer"), HLSLKeyword::Tbuffer));
		gKeywordMapping->insert(make_pair(string("technique"), HLSLKeyword::Technique));
		gKeywordMapping->insert(make_pair(string("technique10"), HLSLKeyword::Technique10));
		gKeywordMapping->insert(make_pair(string("technique11"), HLSLKeyword::Technique11));
		gKeywordMapping->insert(make_pair(string("texture"), HLSLKeyword::Texture));
		gKeywordMapping->insert(make_pair(string("Texture1D"), HLSLKeyword::Texture1D));
		gKeywordMapping->insert(make_pair(string("Texture1DArray"), HLSLKeyword::Texture1DArray));
		gKeywordMapping->insert(make_pair(string("Texture2D"), HLSLKeyword::Texture2D));
		gKeywordMapping->insert(make_pair(string("Texture2DArray"), HLSLKeyword::Texture2DArray));
		gKeywordMapping->insert(make_pair(string("Texture2DMS"), HLSLKeyword::Texture2DMS));
		gKeywordMapping->insert(make_pair(string("Texture2DMSArray"), HLSLKeyword::Texture2DMSArray));
		gKeywordMapping->insert(make_pair(string("Texture3D"), HLSLKeyword::Texture3D));
		gKeywordMapping->insert(make_pair(string("TextureCube"), HLSLKeyword::TextureCube));
		gKeywordMapping->insert(make_pair(string("TextureCubeArray"), HLSLKeyword::TextureCubeArray));
		gKeywordMapping->insert(make_pair(string("true"), HLSLKeyword::True));
		gKeywordMapping->insert(make_pair(string("typedef"), HLSLKeyword::Typedef));
		gKeywordMapping->insert(make_pair(string("triangle"), HLSLKeyword::Triangle));
		gKeywordMapping->insert(make_pair(string("triangleadj"), HLSLKeyword::Triangleadj));
		gKeywordMapping->insert(make_pair(string("TriangleStream"), HLSLKeyword::TriangleStream));
	
		gKeywordMapping->insert(make_pair(string("uint"), HLSLKeyword::Uint));
		gKeywordMapping->insert(make_pair(string("uniform"), HLSLKeyword::Uniform));
		gKeywordMapping->insert(make_pair(string("unorm"), HLSLKeyword::Unorm));
		gKeywordMapping->insert(make_pair(string("unsigned"), HLSLKeyword::Unsigned));

		gKeywordMapping->insert(make_pair(string("vector"), HLSLKeyword::Vector));
		gKeywordMapping->insert(make_pair(string("vertexfragment"), HLSLKeyword::Vertexfragment));
		gKeywordMapping->insert(make_pair(string("VertexShader"), HLSLKeyword::VertexShader));
		gKeywordMapping->insert(make_pair(string("void"), HLSLKeyword::Void));
		gKeywordMapping->insert(make_pair(string("volatile"), HLSLKeyword::Volatile));

		gKeywordMapping->insert(make_pair(string("while"), HLSLKeyword::While));
	}

	return (*gKeywordMapping)[name];
}