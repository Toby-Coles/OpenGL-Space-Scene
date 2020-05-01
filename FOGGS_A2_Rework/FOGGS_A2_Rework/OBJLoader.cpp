#include "OBJLoader.h"
#include <iostream>


LoadOBJ::LoadOBJ()
{
}

LoadOBJ::~LoadOBJ()
{
}

bool LoadOBJ::LoadOBJFile(std::string path)
{
	//Checks if the file is of the correct format
	if (path.substr(path.size() - 4, 4) != ".obj")
	{
		return false;
	}

	std::ifstream file(path);

	mLoadedMeshes.clear();
	mLoadedVertices.clear();
	mLoadedIndices.clear();

	std::vector<Vector3> objectPositions;
	std::vector<Vector2> texCoords;
	std::vector<Vector3> objectNormals;

	std::vector<Vertex> objectVertices;
	std::vector<unsigned int> objectIndices;

	std::vector<std::string> meshMaterialNames;

	bool listening = false;
	std::string meshName;

	Mesh tempMesh;



	//Output to the console the information on the loaded file
	std::string currentLine;
	while (std::getline(file, currentLine))
	{


		//Generate mesh object or prepare for one to be created
		if (algorithm::firstToken(currentLine) == "o" || algorithm::firstToken(currentLine) == "g" || currentLine[0] == 'g')
		{
			if (!listening)
			{
				listening = true;
				if (algorithm::firstToken(currentLine) == "o" || algorithm::firstToken(currentLine) == "g")
				{
					meshName = algorithm::tail(currentLine);

				}
				else {
					meshName == "unnamed";
				}

			}
			else
			{
				//Generate a new mesh to put into the array
				if (!mLoadedIndices.empty() && !mLoadedVertices.empty())
				{
					//Creates a Mesh
					tempMesh = Mesh(mLoadedVertices, mLoadedIndices);
					tempMesh.MeshName = meshName;

					//Insert Mesh
					mLoadedMeshes.push_back(tempMesh);

					//Cleanup
					objectVertices.clear();
					objectIndices.clear();
					meshName.clear();

					meshName = algorithm::tail(currentLine);

				}
				else
				{
					if (algorithm::firstToken(currentLine) == "o" || algorithm::firstToken(currentLine) == "g")
					{
						meshName = algorithm::tail(currentLine);
					}
					else
					{
						meshName = "unnamed";
					}
				}
			}

		}

		//Generate Vertex Position
		else if (algorithm::firstToken(currentLine) == "v") {
			//std::cout << "Vertex Position Generation Initialated" << std::endl;
			GenerateVertexPosition(objectPositions, currentLine);
		}

		//Generate Vertex Texture Coordinate
		else if (algorithm::firstToken(currentLine) == "vt")
		{
			//std::cout << "Vertex Texture Coordinaate Generation Initialated" << std::endl;
			GenerateVertexTextureCoord(texCoords, currentLine);
		}

		//Generate a Vertex Normal
		else if (algorithm::firstToken(currentLine) == "vn")
		{
			//std::cout << "Vertex Normal Generation Initialated" << std::endl;
			GenerateVertexNormal(objectNormals, currentLine);
		}

		//Generate a Face (vertices and indices)
		else if (algorithm::firstToken(currentLine) == "f")
		{
			//std::cout << "Face Generation Initialated" << std::endl;
			GenerateFace(objectVertices, objectIndices, objectPositions, texCoords, objectNormals, currentLine);

		}
		//Get material name
		else if (algorithm::firstToken(currentLine) == "usemt1")
		{
			GetMaterialName(tempMesh, meshName, meshMaterialNames, objectVertices, objectIndices, currentLine);
		}
		else if (algorithm::firstToken(currentLine) == "mtllib")
		{
			LoadMaterials(GetMaterialFilePath(path, currentLine));
		}
		////Insert Mesh
		//mLoadedMeshes.push_back(tempMesh);
		//
		////Cleanup
		//objectVertices.clear();
		//objectIndices.clear();
	}

	if (!objectIndices.empty() && !objectVertices.empty())
	{
		// Create Mesh
		tempMesh = Mesh(objectVertices, objectIndices);
		tempMesh.MeshName = meshName;

		// Insert Mesh
		mLoadedMeshes.push_back(tempMesh);
	}

	file.close();

	// Set Materials for each Mesh
	for (int i = 0; i < meshMaterialNames.size(); i++)
	{
		std::string matname = meshMaterialNames[i];

		// Find corresponding material name in loaded materials
		// when found copy material variables into mesh material
		for (int j = 0; j < mLoadedMaterial.size(); j++)
		{
			if (mLoadedMaterial[j].name == matname)
			{
				mLoadedMeshes[i].MeshMaterial = mLoadedMaterial[j];
				break;
			}
		}
	}

	if (mLoadedMeshes.empty() && mLoadedVertices.empty() && mLoadedIndices.empty())
	{
		return false;
	}
	else
	{
		return true;
	}


}

void LoadOBJ::GenerateVertexPosition(std::vector<Vector3>& inPositions, std::string& inCurrentLine)
{
	//Generate a vertex position

	std::vector<std::string> stringPosition;
	Vector3 vectorPosition;

	algorithm::split(algorithm::tail(inCurrentLine), stringPosition, " ");

	vectorPosition.X = std::stof(stringPosition[0]);
	vectorPosition.Y = std::stof(stringPosition[1]);
	vectorPosition.Z = std::stof(stringPosition[2]);

	inPositions.push_back(vectorPosition);


}

void LoadOBJ::GenerateVertexTextureCoord(std::vector<Vector2>& inTexCoords, std::string& inCurrentLine)
{
	std::vector<std::string> stringTex;
	Vector2 vectorTex;
	algorithm::split(algorithm::tail(inCurrentLine), stringTex, " ");

	vectorTex.X = std::stof(stringTex[0]);
	vectorTex.Y = std::stof(stringTex[1]);

	inTexCoords.push_back(vectorTex);
}

void LoadOBJ::GenerateVertexNormal(std::vector<Vector3>& inNormals, std::string& inCurrentLine)
{
	std::vector<std::string> stringNormal;
	Vector3 vectorNormal;
	algorithm::split(algorithm::tail(inCurrentLine), stringNormal, " ");

	vectorNormal.X = std::stof(stringNormal[0]);
	vectorNormal.Y = std::stof(stringNormal[1]);
	vectorNormal.Z = std::stof(stringNormal[2]);

	inNormals.push_back(vectorNormal);
}

void LoadOBJ::GenerateFace(std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::vector<Vector3>& inPositions, std::vector<Vector2>& inTexCoords, std::vector<Vector3>& inNormals, std::string& inCurrentLine)
{
	//Generate the vertices
	std::vector<Vertex> vectorVertices;

	//std::cout << "Generating Vertices from OBJ Started" << std::endl;

	GenerateVerticesFromRawOBJ(vectorVertices, inPositions, inTexCoords, inNormals, inCurrentLine);
	//std::cout << "Generating Vertices from OBJ Completed" << std::endl;

	//Add Vertices
	for (int i = 0; i < int(vectorVertices.size()); i++)
	{
		inVertices.push_back(vectorVertices[i]);

		mLoadedVertices.push_back(vectorVertices[i]);
	}

	std::vector<unsigned int> intIndices;
	TriangulateVertices(intIndices, vectorVertices);
	//std::cout << "Vertex Triangulation Complete" << std::endl;

	//Add Indices


	for (int i = 0; i < int(intIndices.size()); i++)
	{

		unsigned int intIndicesNumber = (unsigned int)((inVertices.size()) - vectorVertices.size()) + intIndices[i];
		inIndices.push_back(intIndicesNumber);
		//std::cout << " PushBack1 Complete" << std::endl;

		intIndicesNumber = (unsigned int)((mLoadedVertices.size()) - vectorVertices.size()) + intIndices[i];
		mLoadedIndices.push_back(intIndicesNumber);
		//std::cout << "PushBack2 Complete" << std::endl;
		//std::cout << intIndices.size() << std::endl;

	}
	//std::cout << "Face Generation Complete" << std::endl;
}


void LoadOBJ::TriangulateVertices(std::vector<unsigned int>& outIndices, const std::vector<Vertex> inVertices)
{
	//std::cout << "Vertex Triangulation Started" << std::endl;
	//If there are 2 or less vertives, no triangle will be created, then exit
	if (inVertices.size() < 3)
	{
		return;
	}
	//if it is a triangle there is no need to calculate it
	if (inVertices.size() == 3)
	{
		outIndices.push_back(0);
		outIndices.push_back(1);
		outIndices.push_back(2);
		return;
	}

	//Creates a list of vertices
	std::vector<Vertex> tVerts = inVertices;

	while (true)
	{
		//For every vertex...
		for (int i = 0; i < int(tVerts.size()); i++)
		{
			//The previous vertex in the list
			Vertex previousVertex;
			if (i == 0)
			{
				previousVertex = tVerts[tVerts.size() - 1];

			}
			else
			{
				previousVertex = tVerts[i - 1];
			}

			//The current vertex in the list
			Vertex currentVertex = tVerts[i];

			//The Next Vertex in the list
			Vertex nextVertex;
			if (i == tVerts.size() - 1)
			{
				nextVertex = tVerts[0];
			}
			else
			{
				nextVertex = tVerts[i + 1];
			}

			//Check to see if there are only 3 verts left, if so this is the last triangle
			if (tVerts.size() == 3)
			{
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (inVertices[j].Position == currentVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == previousVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == nextVertex.Position)
						outIndices.push_back(j);

				}
				tVerts.clear();
				break;
			}
			if (tVerts.size() == 4)
			{
				// Create a triangle from current, next, and previous vectors
				for (int j = 0; j < int(inVertices.size()); j++)
				{
					if (inVertices[j].Position == currentVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == previousVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == nextVertex.Position)
						outIndices.push_back(j);
				}

				Vector3 tempVector;
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position != currentVertex.Position
						&& tVerts[j].Position != previousVertex.Position
						&& tVerts[j].Position != nextVertex.Position)
					{
						tempVector = tVerts[j].Position;
						break;
					}
				}

				//Create a triangle from current, next, and previous vectors
				for (int j = 0; j < int(inVertices.size()); j++)
				{
					if (inVertices[j].Position == previousVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == nextVertex.Position)
						outIndices.push_back(j);
					if (inVertices[j].Position == tempVector)
						outIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			//if Vertex is not an interior vertex
			float angle = math::AngleBetweenV3(previousVertex.Position - currentVertex.Position, nextVertex.Position - currentVertex.Position) * (180 / 3.14159265359);
			if (angle <= 0 && angle >= 180)
				continue;

			//If any vertices are within this triangle
			bool withinTriangle = false;
			for (int j = 0; j < int(inVertices.size()); j++)
			{
				if (algorithm::inTriangle(inVertices[j].Position, previousVertex.Position, currentVertex.Position, nextVertex.Position)
					&& inVertices[j].Position != previousVertex.Position
					&& inVertices[j].Position != currentVertex.Position
					&& inVertices[j].Position != nextVertex.Position)
				{
					withinTriangle = true;
					break;
				}

			}
			if (withinTriangle)
				continue;

			//Create a triangle from current, previous and next vectors
			for (int j = 0; j < int(inVertices.size()); j++)
			{
				if (inVertices[j].Position == currentVertex.Position)
					outIndices.push_back(j);
				if (inVertices[j].Position == previousVertex.Position)
					outIndices.push_back(j);
				if (inVertices[j].Position == nextVertex.Position)
					outIndices.push_back(j);
			}

			//Delete currentVertex from the list
			for (int j = 0; j < int(tVerts.size()); j++)
			{
				if (tVerts[j].Position == currentVertex.Position)
				{
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}


			i = -1;
		}
		if (outIndices.size() == 0)
		{
			break;
		}
		if (tVerts.size() == 0)
		{
			break;
		}
	}
}

void LoadOBJ::GenerateVerticesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string inCurrentLine)
{
	std::vector<std::string> stringFace, stringVert;
	Vertex vertexVert;

	algorithm::split(algorithm::tail(inCurrentLine), stringFace, " ");

	bool noNormal = false;

	//For every given vertex, run this:
	for (int i = 0; i < int(stringFace.size()); i++)
	{
		int vertexType;

		algorithm::split(stringFace[i], stringVert, "/");

		//Check for just position (v1)
		if (stringVert.size() == 1)
		{
			//Only position
			vertexType = 1;
		}

		//Check for a position and texture (v1/vt1)
		if (stringVert.size() == 2)
		{
			vertexType = 2;
		}

		//Check for position, texture and normal (v1/vt1/vn1)
		if (stringVert.size() == 3)
		{
			if (stringVert[1] != "")
			{
				//Position, texture and normal
				vertexType = 4;
			}
			else
			{
				vertexType = 3;
			}
		}

		switch (vertexType)
		{
		case 1: // P
		{
			vertexVert.Position = algorithm::getElement(inPositions, stringVert[0]);
			vertexVert.TextureCoordinate = Vector2();
			noNormal = true;
			outVerts.push_back(vertexVert);
			break;
		}
		case 2: // P/T
		{
			vertexVert.Position = algorithm::getElement(inPositions, stringVert[0]);
			vertexVert.TextureCoordinate = algorithm::getElement(inTexCoords, stringVert[1]);
			noNormal = true;
			outVerts.push_back(vertexVert);
			break;
		}
		case 3: // P//N
		{
			vertexVert.Position = algorithm::getElement(inPositions, stringVert[0]);
			vertexVert.TextureCoordinate = Vector2();
			vertexVert.Normal = algorithm::getElement(inNormals, stringVert[2]);
			outVerts.push_back(vertexVert);
			break;
		}
		case 4: // P/T/N
		{
			vertexVert.Position = algorithm::getElement(inPositions, stringVert[0]);
			vertexVert.TextureCoordinate = algorithm::getElement(inTexCoords, stringVert[1]);
			vertexVert.Normal = algorithm::getElement(inNormals, stringVert[2]);
			outVerts.push_back(vertexVert);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	//Take care of missing normals
	//These may not be truly accurate, but is better than not compiling mesh with normals

	if (noNormal)
	{
		Vector3 A = outVerts[0].Position - outVerts[1].Position;
		Vector3 B = outVerts[2].Position - outVerts[1].Position;

		Vector3 normal = math::CrossV3(A, B);
		for (int i = 0; i < outVerts.size(); i++)
		{
			outVerts[i].Normal = normal;
		}
	}
}

void LoadOBJ::GetMaterialName(Mesh& inTempMesh, std::string& inMeshName, std::vector<std::string>& inMeshMaterialNames, std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::string& inCurrentLine)
{
	if (algorithm::firstToken(inCurrentLine) == "usemtl")
	{
		inMeshMaterialNames.push_back(algorithm::tail(inCurrentLine));

		//Create a new mesh, if material changes within a group
		if (!inIndices.empty() && !inVertices.empty())
		{
			//Create a Mesh
			inTempMesh = Mesh(inVertices, inIndices);
			inTempMesh.MeshName = inMeshName;
			int i = 2;

			while (1)
			{
				inTempMesh.MeshName = inMeshName + "_" + std::to_string(i);
				for (auto& m : mLoadedMeshes)
				{
					if (m.MeshName == inTempMesh.MeshName)
						continue;
					break;
				}

				//insert Mesh
				mLoadedMeshes.push_back(inTempMesh);

				//Cleanup 
				inVertices.clear();
				inIndices.clear();
			}
		}

	}

}

std::string LoadOBJ::GetMaterialFilePath(std::string path, std::string& inCurrentLine)
{

	//generate a path to the material file
	std::vector<std::string> tempVector;
	algorithm::split(path, tempVector, "/");

	std::string pathTomat = "";
	if (tempVector.size() != 1)
	{
		for (int i = 0; i < tempVector.size() - 1; i++)
		{
			pathTomat += tempVector[i] + "/";
		}

		//std::cout << (pathTomat += algorithm::tail(inCurrentLine));
		return pathTomat += algorithm::tail(inCurrentLine);



		//Load Materials
		//LoadMaterials(inCurrentLine);
	}

}

bool LoadOBJ::LoadMaterials(std::string path)
{
	//if the file is not a material, file returns false
	if (path.substr(path.size() - 4, path.size()) != ".mtl")
	{
		std::cout << "Incorrect file format" << std::endl;
		return false;
	}

	std::ifstream inFile(path);
	if (!inFile.is_open())
	{
		return false;
	}

	Material tempMaterial;

	bool listening = false;

	//Go through each line looking for material variables
	std::string currentLine;

	while (std::getline(inFile, currentLine))
	{
		//new material and material name
		if (algorithm::firstToken(currentLine) == "newmtl")
		{
			if (!listening)
			{
				listening = true;
				if (currentLine.size() > 7)
				{
					tempMaterial.name = algorithm::tail(currentLine);
				}
				else
				{
					tempMaterial.name = "none";

				}
			}
			else
			{
				// Generate the material

						// Push Back loaded Material
				mLoadedMaterial.push_back(tempMaterial);

				// Clear Loaded Material
				tempMaterial = Material();

				if (currentLine.size() > 7)
				{
					tempMaterial.name = algorithm::tail(currentLine);
				}
				else
				{
					tempMaterial.name = "none";
				}
			}

		}

		//Load Ambient Colour
		else if (algorithm::firstToken(currentLine) == "Ka")
		{
			LoadAmbientColor(tempMaterial, currentLine);
		}

		//Load Diffuse Colour
		else if (algorithm::firstToken(currentLine) == "Kd")
		{
			LoadDiffuseColor(tempMaterial, currentLine);
		}

		//Load Specular Colour
		else if (algorithm::firstToken(currentLine) == "ks")
		{
			LoadSpecularColor(tempMaterial, currentLine);
		}

		// Specular Exponent
		else if (algorithm::firstToken(currentLine) == "Ns")
		{
			tempMaterial.Ns = std::stof(algorithm::tail(currentLine));
		}
		// Optical Density
		else if (algorithm::firstToken(currentLine) == "Ni")
		{
			tempMaterial.Ni = std::stof(algorithm::tail(currentLine));
		}
		// Dissolve
		else if (algorithm::firstToken(currentLine) == "d")
		{
			tempMaterial.d = std::stof(algorithm::tail(currentLine));
		}
		// Illumination
		else if (algorithm::firstToken(currentLine) == "illum")
		{
			tempMaterial.illum = std::stoi(algorithm::tail(currentLine));
		}
		// Ambient Texture Map
		else if (algorithm::firstToken(currentLine) == "map_Ka")
		{
			tempMaterial.map_Ka = algorithm::tail(currentLine);
		}
		// Diffuse Texture Map
		else if (algorithm::firstToken(currentLine) == "map_Kd")
		{
			tempMaterial.map_Kd = algorithm::tail(currentLine);
		}
		// Specular Texture Map
		else if (algorithm::firstToken(currentLine) == "map_Ks")
		{
			tempMaterial.map_Ks = algorithm::tail(currentLine);
		}
		// Specular Hightlight Map
		else if (algorithm::firstToken(currentLine) == "map_Ns")
		{
			tempMaterial.map_Ns = algorithm::tail(currentLine);
		}
		// Alpha Texture Map
		else if (algorithm::firstToken(currentLine) == "map_D")
		{
			tempMaterial.map_d = algorithm::tail(currentLine);
		}
		// Bump Map
		else if (algorithm::firstToken(currentLine) == "map_Bump" || algorithm::firstToken(currentLine) == "map_bump" || algorithm::firstToken(currentLine) == "bump")
		{
			tempMaterial.map_bump = algorithm::tail(currentLine);
		}

	}

	mLoadedMaterial.push_back(tempMaterial);
	if (mLoadedMaterial.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}



void LoadOBJ::LoadAmbientColor(Material& inMaterial, std::string inCurrentLine)
{

	std::vector<std::string> tempVector;
	algorithm::split(algorithm::tail(inCurrentLine), tempVector, " ");

	if (tempVector.size() != 3)
	{
		std::cout << "Error in Ambient Vector Size" << std::endl;
		return;
	}


	inMaterial.Ka.X = std::stof(tempVector[0]);
	inMaterial.Ka.Y = std::stof(tempVector[1]);
	inMaterial.Ka.Z = std::stof(tempVector[2]);


}

void LoadOBJ::LoadSpecularColor(Material& inMaterial, std::string inCurrentLine)
{

	std::vector <std::string> tempVector;
	algorithm::split(algorithm::tail(inCurrentLine), tempVector, " ");

	if (tempVector.size() != 3)
		std::cout << "Error in Specular Color Vector Size " << std::endl;

	inMaterial.Ks.X = std::stof(tempVector[0]);
	inMaterial.Ks.Y = std::stof(tempVector[1]);
	inMaterial.Ks.Z = std::stof(tempVector[2]);



}

void LoadOBJ::LoadDiffuseColor(Material& inMaterial, std::string inCurrentLine)
{

	std::vector<std::string> tempVector;
	algorithm::split(algorithm::tail(inCurrentLine), tempVector, " ");

	if (tempVector.size() != 3)
	{
		std::cout << "Error in diffuse vector size" << std::endl;
	}

	inMaterial.Kd.X = std::stof(tempVector[0]);
	inMaterial.Kd.Y = std::stof(tempVector[1]);
	inMaterial.Kd.Z = std::stof(tempVector[2]);



}
