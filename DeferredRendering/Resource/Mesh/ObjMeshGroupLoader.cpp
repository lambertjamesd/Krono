
#include "ObjMeshGroupLoader.h"
#include <string>
#include <ctype.h>
#include "Core/Hash.h"
#include "Resource/ResourceManager.h"
#include "MeshMath.h"
#include <cstddef>

using namespace std;

namespace krono
{

ObjMeshVertex::ObjMeshVertex(const Vector3f& position, const Vector3f& normal, const Vector2f& texCoord) :
	position(position),
	normal(normal),
	texCoord(texCoord)
{

}

ObjMeshVertexData::ObjMeshVertexData()
{
	mPositions.push_back(Vector3f());
	mTexCoords.push_back(Vector2f());
	mNormals.push_back(Vector3f());
}

ObjMeshVertexData::~ObjMeshVertexData()
{

}

void ObjMeshVertexData::AddPosition(const Vector3f& position)
{
	mPositions.push_back(position);
}

void ObjMeshVertexData::AddTexCoord(const Vector2f& texCoord)
{
	mTexCoords.push_back(texCoord);
}

void ObjMeshVertexData::AddNormal(const Vector3f& normal)
{
	mNormals.push_back(normal);
}

UInt16 ObjMeshVertexData::GetVertexBufferIndex(UInt16 vertexIndex, UInt16 texCoordIndex, UInt16 normalIndex)
{
	UInt64 digest = ((UInt64)vertexIndex & 0x3FFFFF) | (((UInt64)texCoordIndex & 0x1FFFFF) << 22) | (((UInt64)normalIndex & 0x1FFFFF) << 43);
	auto existingIndex = mIndexMap.find(digest);

	if (existingIndex == mIndexMap.end())
	{
		UInt16 result = (UInt16)mVertexData.size();
		mIndexMap[digest] = result;
		mVertexData.push_back(ObjMeshVertex(mPositions[vertexIndex], mNormals[normalIndex], mTexCoords[texCoordIndex]));
		return result;
	}
	else
	{
		return existingIndex->second;
	}
}

void ObjMeshVertexData::CalculateTangents(const UInt16* indexBuffer, size_t indexCount)
{
	AttributeArray<Vector4f> tangentArray(&mVertexData.front(), offsetof(ObjMeshVertex, tangent), sizeof(ObjMeshVertex));

	MeshMath::AppendTangents<UInt16>(
		tangentArray,
		AttributeArray<Vector3f>(&mVertexData.front(), offsetof(ObjMeshVertex, position), sizeof(ObjMeshVertex)),
		AttributeArray<Vector2f>(&mVertexData.front(), offsetof(ObjMeshVertex, texCoord), sizeof(ObjMeshVertex)),
		AttributeArray<Vector3f>(&mVertexData.front(), offsetof(ObjMeshVertex, normal), sizeof(ObjMeshVertex)),
		indexBuffer, indexCount);

}

void ObjMeshVertexData::NormalizeTangents()
{
	AttributeArray<Vector4f> tangentArray(&mVertexData.front(), offsetof(ObjMeshVertex, tangent), sizeof(ObjMeshVertex));
	MeshMath::NormalizeTangents(tangentArray, AttributeArray<Vector3f>(&mVertexData.front(), offsetof(ObjMeshVertex, normal), sizeof(ObjMeshVertex)), mVertexData.size());
}

BoundingBoxf ObjMeshVertexData::CalculateBoundingBox() const
{
	return MeshMath::CalculateBoundingBox(AttributeArray<Vector3f>((void*)(&mVertexData.front()), offsetof(ObjMeshVertex, position), sizeof(ObjMeshVertex)), mVertexData.size());
}

void ObjMeshVertexData::PopulateVertexBuffer(VertexBuffer& vertexBuffer) const
{	
	vertexBuffer.Set(&mVertexData.front(), mVertexData.size());
}

ObjMeshGroupLoader::ObjMeshGroupLoader()
{

}

ObjMeshGroupLoader::~ObjMeshGroupLoader(void)
{

}

Auto<Resource> ObjMeshGroupLoader::LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName)
{
	ObjMeshVertexData vertexData;

	InputLayout inputLayout;
	inputLayout.AddAttribute(Attribute("POSITION", DataFormat(DataFormat::Float, 3)));
	inputLayout.AddAttribute(Attribute("NORMAL", DataFormat(DataFormat::Float, 3)));
	inputLayout.AddAttribute(Attribute("TANGENT", DataFormat(DataFormat::Float, 4)));
	inputLayout.AddAttribute(Attribute("TEXCOORD", DataFormat(DataFormat::Float, 2)));
	Auto<VertexBuffer> vertexBuffer = Auto<VertexBuffer>(resourceManager.GetGraphics()->CreateVertexBuffer(inputLayout, BufferAccess::Immutable));
	Auto<IndexBuffer> currentIndexBuffer = Auto<IndexBuffer>(resourceManager.GetGraphics()->CreateIndexBuffer(IndexBuffer::UInt16, BufferAccess::Immutable));

	size_t indexStart = 0;
	vector<UInt16> indexData;
	Auto<Mesh> currentMesh;

	Auto<MeshGroup> result(new MeshGroup());

	while (!inputStream.eof())
	{
		vector<string> tokens;
		ParseLine(inputStream, tokens);
		bool checkSubmeshRebuild = false;
		bool buildNewMesh = false;

		if (tokens.size() != 0)
		{
			if (tokens[0] == "v")
			{
				vertexData.AddPosition(Vector3f((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str())));
			}
			else if (tokens[0] == "vn")
			{
				vertexData.AddNormal(Vector3f((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str())));
			}
			else if (tokens[0] == "vt")
			{
				vertexData.AddTexCoord(Vector2f((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str())));
			}
			else if (tokens[0] == "o")
			{
				buildNewMesh = true;
				checkSubmeshRebuild = true;
			}
			else if (tokens[0] == "usemtl")
			{
				checkSubmeshRebuild = true;
			}
			else if (tokens[0] == "f")
			{
				vector<UInt16> indices(tokens.size() - 1);

				for (size_t i = 0; i < indices.size(); ++i)
				{
					size_t triangleIndices[3];
					SplitIndices(tokens[i + 1], triangleIndices);
					indices[i] = vertexData.GetVertexBufferIndex(triangleIndices[0], triangleIndices[1], triangleIndices[2]);
				}

				for (size_t i = 2; i < indices.size(); ++i)
				{	
					indexData.push_back(indices[0]);
					indexData.push_back(indices[i-1]);
					indexData.push_back(indices[i]);
				}
			}
		}

		if (indexData.size() > indexStart && (checkSubmeshRebuild || inputStream.eof()))
		{
			vertexData.CalculateTangents(&indexData.front() + indexStart, indexData.size() - indexStart);
			currentMesh->AddSubMesh(vertexBuffer, currentIndexBuffer, indexStart, indexData.size() - indexStart);
			indexStart = indexData.size();
		}

		if (buildNewMesh)
		{
			currentMesh = Auto<Mesh>(new Mesh());
			result->AddMesh(tokens[1], currentMesh);
		}
	}

	vertexData.NormalizeTangents();
	vertexData.PopulateVertexBuffer(*vertexBuffer);
	
	currentIndexBuffer->Set(&indexData.front(), indexData.size());

	return result;
}

void ObjMeshGroupLoader::SplitIndices(const std::string& input, size_t indices[3])
{
	size_t currentIndex = 0;
	size_t stringIndex = 0;
	
	while (currentIndex < 3 && stringIndex != string::npos)
	{
		size_t stringEnd = input.find('/', stringIndex);

		if (stringEnd == string::npos)
		{
			indices[currentIndex] = atoi(input.substr(stringIndex, string::npos).c_str());
			stringIndex = string::npos;
		}
		else
		{
			indices[currentIndex] = atoi(input.substr(stringIndex, stringEnd - stringIndex).c_str());
			stringIndex = stringEnd + 1;
		}

		++currentIndex;
	}

	while (currentIndex < 3)
	{
		indices[currentIndex] = 0;
		++currentIndex;
	}
}

void ObjMeshGroupLoader::ParseLine(istream& inputStream, vector<string>& tokens)
{
	char line[MAX_LINE_LENGTH];
	inputStream.getline(line, MAX_LINE_LENGTH);

	char *wordStart = line;
	char *wordEnd = wordStart;

	while (*wordStart != '\0')
	{
		while (*wordStart != '\0' && isspace(*wordStart))
		{
			++wordStart;
		}

		wordEnd = wordStart;

		while (*wordEnd != '\0' && !isspace(*wordEnd))
		{
			++wordEnd;
		}

		if (wordStart != wordEnd) {
			tokens.push_back(string(wordStart, wordEnd));
		}

		wordStart = wordEnd;
	}
}

}
