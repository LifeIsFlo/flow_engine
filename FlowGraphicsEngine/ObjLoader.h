#pragma once
#include <span>

#include "FWD.h"

namespace FlowEngineDX11 {
	class ObjLoader
	{
	public:
		enum ObjFileCommand : uint8_t
		{
			COMMAND_OBJECT_NAME,
			COMMAND_VERTEX,
			COMMAND_TEXTURECOORD,
			COMMAND_NORMAL,
			COMMENT
		};


		static HRESULT CommandFromLine(std::string& line, ObjFileCommand* obj_file_command, std::span<std::string>* params);
	};
}
