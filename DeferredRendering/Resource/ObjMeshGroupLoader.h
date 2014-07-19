#pragma once

#include "MeshGroupLoader.h"
#include "../Interface/Graphics.h"
#include <vector>
#include <unordered_map>
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"

struct ObjMeshVertex
{
	ObjMeshVertex(const Vector3f& position, const Vector2f& texCoord, const Vector3f& normal);

	Vector3f position;
	Vector2f texCoord;
	Vector3f normal;
};

class ObjMeshVertexData
{
public:
	ObjMeshVertexData();
	~ObjMeshVertexData();

	void AddPosition(const Vector3f& position);
	void AddTexCoord(const Vector2f& texCoord);
	void AddNormal(const Vector3f& normal);

	UINT16 GetVertexBufferIndex(UINT16 vertexIndex, UINT16 texCoordIndex, UINT16 normalIndex); 

	void PopulateVertexBuffer(VertexBuffer& vertexBuffer) const;
private:
	std::vector<Vector3f> mPositions;
	std::vector<Vector2f> mTexCoords;
	std::vector<Vector3f> mNormals;

	std::vector<ObjMeshVertex> mVertexData;

	std::unordered_map<UINT64, UINT16> mIndexMap;
};

class ObjMeshGroupLoader : public MeshGroupLoader
{
public:
	ObjMeshGroupLoader();
	~ObjMeshGroupLoader(void);
	
	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);
private:
	static void SplitIndices(const std::string& input, size_t indices[3]);
	static void ParseLine(std::istream& inputStream, std::vector<std::string>& tokens);

	const static size_t MAX_LINE_LENGTH = 256;
};

