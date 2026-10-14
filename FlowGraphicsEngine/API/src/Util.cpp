#include "Util.h"

void FlowEngineDX11::Util::StringSplit(const std::string& in, std::vector<std::string>* pOut, const char& sep)
{
	std::string paramBuffer;

	for (auto character : in)
	{
		if (character == sep)
		{
			if (!paramBuffer.empty())
			{
				pOut->push_back(paramBuffer);
				paramBuffer.clear();
			}
		}
		else
		{
			paramBuffer.push_back(character);
		}
	}

	if (!paramBuffer.empty())
	{
		pOut->push_back(paramBuffer);
	}
}
