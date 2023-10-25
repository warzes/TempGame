#pragma once

class HashUtil {
public:
	template <typename... Args> static void combine(std::size_t&, const Args&...);
};

template <typename... Args> void HashUtil::combine(std::size_t& seed, const Args&... args) {
	((seed = seed ^ (std::hash<Args>{}(args)+0x9e3779b9 + (seed << 6) + (seed >> 2))), ...);
}
