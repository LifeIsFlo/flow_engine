#pragma once
#include "FWD.h"

namespace FlowEngineDX11 {
	class Util
	{
	public:
		static void StringSplit(const std::string& in, std::vector<std::string>* pOut, const char& sep);
	};
}
