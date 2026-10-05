#pragma once
#include <string>
#include <optional>

namespace talle::env
{
	std::optional<std::string> get(const std::string& name);
}
