#include "ObjLoader.h"

HRESULT FlowEngineDX11::ObjLoader::CommandFromLine(std::string& line, ObjFileCommand* obj_file_command, std::span<std::string>* params)
{
    if (line.starts_with("v"))
    {
        auto substrParams = line.substr(0, 3);

        

        *obj_file_command = COMMAND_VERTEX;
    }
    if(line.starts_with("vt"))
    {
        *obj_file_command = COMMAND_TEXTURECOORD;
    }
    if (line.starts_with("vn"))
    {
        *obj_file_command = COMMAND_NORMAL;


    }

    return S_OK;
}
