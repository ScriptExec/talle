#include <talle/utils/env.hpp>

namespace talle::env
{
	std::optional<std::string> get(const std::string& name)
	{
		auto value = std::getenv(name.c_str());
		if (value == nullptr) return std::nullopt;
		return std::string{ value };
	}
}
