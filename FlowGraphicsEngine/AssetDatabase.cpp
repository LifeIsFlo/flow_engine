#include "AssetDatabase.h"
#include <fstream>
#include <iostream>
#include "Mesh.h"

FlowEngineDX11::Mesh* FlowEngineDX11::AssetDatabase::LoadMesh(std::string fileName)
{
	std::ifstream meshFile(fileName.c_str(), std::ios::in);

	std::string currentLine;

	while (std::getline(meshFile, currentLine))
	{
		
	}
}