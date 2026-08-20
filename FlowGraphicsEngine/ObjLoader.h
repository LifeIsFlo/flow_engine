#pragma once
#include <map>
#include <span>
#include <string>
#include "FWD.h"

namespace FlowEngineDX11 {
	class ObjLoader
	{
	public:
		enum ObjFileCommand : uint8_t
		{
			COMMAND_OBJECT_NAME,
			COMMAND_FACE,
			COMMAND_VERTEX,
			COMMAND_TEXTURECOORD,
			COMMAND_NORMAL,
			COMMAND_MATERIAL,
			COMMAND_SMOOTHING,
			COMMENT
		};

		static void LoadModel(std::ifstream& fileStream, std::map<std::string, std::unique_ptr<Model>>* modelsOut, const std::string& assetLocation);
		static HRESULT CompileLine(const std::string& line, ObjFileCommand& obj_file_command, Vector3& value, std::vector<std::string>& paramsOut);

		static const std::string POS;
		static const std::string TEX;
		static const std::string NOR;
		static const std::string MTL;
		static const std::string OBJ;
		static const std::string FACE;
		static const std::string SMOOTHING;
	};
}
