#pragma once

#include <string>
#include <cstdlib>
#include <map>

namespace HLSLTokenType
{
	enum Type
	{
		None,
		Error,
		EndOfFile,
		Whitespace,
		Identifier,
		Keyword,
		Number,
		String,
		Comment,
		
		Add,
		Minus,
		Multiply,
		Divide,
		Modulus,

		Assign,
		AddEqual,
		MinusEqual,
		MultiplyEqual,
		DivideEqual,
		ModulusEqual,

		BitwiseNot,
		LeftShift,
		RightShift,
		BitwiseAnd,
		BitwiseOr,
		BitwiseXor,
		LeftShiftEqual,
		RightShiftEqual,
		BitwiseAndEqual,
		BitwiseOrEqual,
		BitwiseXorEqual,

		BooleanAnd,
		BooleanOr,

		TernaryOperator,
		Colon,
		SemiColon,

		Comma,

		LessThan,
		GreaterThan,
		Equal,
		NotEqual,
		LessThanEqual,
		GreaterThanEqual,

		PlusPlus,
		MinusMinus,

		StructureOperator,

		BooleanNot,

		OpenSquare,
		CloseSquare,

		OpenParen,
		CloseParen,

		OpenCurly,
		CloseCurly
	};
}

namespace HLSLKeyword
{
	enum Type
	{
		None,
		
		// scalar types
		Bool,
		Int,
		Uint,
		Dword,
		Half,
		Float,
		Double,
		Min16float,
		Min10float,
		Min16int,
		Min12int,
		Min16uint,

		// texture types

		Texture,
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		Texture2DMS,
		Texture2DMSArray,
		Texture3D,
		TextureCube,
		TextureCubeArray,

		// control flow
		Break,
		Case,
		Continue,
		Discard,
		Default,
		Do,
		If,
		For,
		Switch,
		While,
		
		Else,

		AppendStructuredBuffer,
		ASM,
		ASM_fragment,
	
		BlendState,
		Buffer,
		ByteAddressBuffer,
	
		Cbuffer,
		Centroid,
		Class,
		Column_major,
		Compile,
		Compile_fragment,
		CompileShader,
		Const,
		ComputeShader,
		ConsumeStructedBuffer,
	
		DepthStencilState,
		DepthStencilView,
		DomainShader,
	
		Export,
		Extern,
	
		False,
		Fxgroup,
	
		GeometryShader,
		Groupshared,
	
		Hullshader,
	
		In,
		Inline,
		Inout,
		InputPatch,
		Interface,

		Line,
		Lineadj,
		Linear,
		LineStream,
	
		Matrix,

		Namespace,
		Nointerpolation,
		Noperspective,
		Null,

		Out,
		OutputPatch,
	
		Packoffset,
		Pass,
		Pixelfragment,
		PixelShader,
		Point,
		PointStream,
		Precise,

		RasterizerState,
		RenderTargetView,
		Return,
		Register,
		Row_major,

		RWBuffer,
		RWByteAddressBuffer,
		RWStructedBuffer,
		RWTexture1D,
		RWTexture1DArray,
		RWTexture2D,
		RWTexture2DArray,
		RWTexture3D,

		Sample,
		Sampler,
		SamplerState,
		SamplerComparisonState,
		Shared,
		Snorm,
		Stateblock,
		Stateblock_state,
		Static,
		String,
		Struct,
		StructedBuffer,

		Tbuffer,
		Technique,
		Technique10,
		Technique11,
		True,
		Typedef,
		Triangle,
		Triangleadj,
		TriangleStream,
	
		Uniform,
		Unorm,
		Unsigned,

		Vector,
		Vertexfragment,
		VertexShader,
		Void,
		Volatile,

	};

	bool IsScalarType(Type keyword);
	bool IsTextureType(Type keyword);
}


namespace InterpolationMode
{
	enum Type
	{
		None,
		Linear,
		NoInterpolation,
		NoPerspective,
		InterpolationMask = 0xF,
		
		Centroid = (1 << 4),
		Sample = (1 << 5),
	};
}

namespace OperatorPrecedence
{
	enum Type
	{
		None,
		Comma,
		Assign,
		Ternary,
		BooleanOr,
		BooleanAnd,
		BitwiseOr,
		BitwiseXor,
		BitwiseAnd,
		Equal,
		Compare,
		Bitshift,
		Addition,
		Multiply
	};
}

class HLSLToken
{
public:
	HLSLToken(HLSLTokenType::Type type, const std::string& value, size_t lineNumber);
	~HLSLToken(void);

	bool IsBinaryOperator() const;
	bool IsUnaryOperator() const;
	bool IsPrefixOperator() const;
	bool IsPostfixOperator() const;
	
	bool IsFunctionStorageClass() const;
	bool IsVariableStorageClass() const;
	bool IsVariableTypeModifier() const;
	bool IsKeyword() const;

	OperatorPrecedence::Type GetOperatorPrecedence() const;
	
	HLSLTokenType::Type GetType() const;
	HLSLKeyword::Type GetKeywordType() const;
	const std::string& GetValue() const;
	size_t GetLineNumber() const;

	int GetIntValue() const;

	bool operator==(const HLSLToken& other) const;
private:
	HLSLTokenType::Type mType;
	HLSLKeyword::Type mKeywordType;
	std::string mValue;
	size_t mLineNumber;

	static HLSLKeyword::Type GetKeywordType(const std::string& name);
	static std::map<std::string, HLSLKeyword::Type>* gKeywordMapping;
};

