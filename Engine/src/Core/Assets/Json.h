#pragma once
#include "spch.h"
#include <json.hpp>
#include "FileSystem.h"

namespace Six {
	using Json = nlohmann::json;

	Json SIXAPI SLoadJS(const SPath& path);
	bool SIXAPI SLoadJS(const SPath& path, Json& out);
	Json SIXAPI SLoadJS(SPath& path);
	bool SIXAPI SLoadJS(SPath& path, Json& out);
	std::string SIXAPI SDumpJS(const Json& in);
	bool SIXAPI SDumpJS(const SPath& path, const Json& in);

}