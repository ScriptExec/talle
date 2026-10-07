#include <talle/utils/env.hpp>

namespace talle::env
{
	std::optional<std::string> get(const std::string& name)
	{
#ifdef _MSC_VER
		size_t required_size = 0;
		if (getenv_s(&required_size, nullptr, 0, name.c_str()) != 0 or required_size == 0) return std::nullopt;

		std::string buffer(required_size - 1, '\0');
		if (getenv_s(&required_size, buffer.data(), required_size, name.c_str()) != 0) return std::nullopt;
		return buffer;
#else
		std::string value = std::getenv(name.c_str());
		if (value == nullptr) return std::nullopt;
		return std::string{ value };
#endif
	}
}
