#pragma once
#include "MathsStructures.h"
#include "RenderStructures.h"

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>

class LoadOBJ
{
public:
	LoadOBJ();
	~LoadOBJ();

	bool LoadOBJFile(std::string path);

	std::vector<Mesh> mLoadedMeshes;
	std::vector<Vertex> mLoadedVertices;
	std::vector<unsigned int> mLoadedIndices;
	std::vector<Material> mLoadedMaterial;

private:
	void TriangulateVertices(std::vector<unsigned int>& outIndices, const std::vector<Vertex>inVertices);
	void GenerateVerticesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string inCurrentLine);

	void GetMaterialName(Mesh& inTempMesh, std::string& inMeshName, std::vector<std::string>& inMeshMaterialNames, std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::string& inCurrentLine);
	std::string GetMaterialFilePath(std::string path, std::string& inCurrentLine);
	bool LoadMaterials(std::string path);

	void GenerateVertexPosition(std::vector<Vector3>& inPositions, std::string& inCurrentLine);
	void GenerateVertexTextureCoord(std::vector<Vector2>& inTexCoords, std::string& inCurrentLine);
	void GenerateVertexNormal(std::vector<Vector3>& inNormals, std::string& inCurrentLine);
	void GenerateFace(std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::vector<Vector3>& inPositions, std::vector<Vector2>& inTexCoords, std::vector<Vector3>& inNormals, std::string& inCurrentLine);


	void LoadAmbientColor(Material& inMaterial, std::string inCurrentLine);
	void LoadSpecularColor(Material& inMaterial, std::string inCurrentLine);
	void LoadDiffuseColor(Material& inMaterial, std::string inCurrentLine);

};
