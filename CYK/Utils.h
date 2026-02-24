#pragma once
#include "Types/Grammar.h"
#include <iostream>
#include <unordered_map>

namespace Utils
{

enum class TransformationMode
{
	Epsilon,
	Chain,
	CNF,
	Useless,
};

inline TransformationMode StringToMode(const std::string& mode)
{
	static const std::unordered_map<std::string, TransformationMode> mapper = {
		{ "epsilon", TransformationMode::Epsilon },
		{ "chain", TransformationMode::Chain },
		{ "cnf", TransformationMode::CNF },
		{ "useless", TransformationMode::Useless },
	};

	const auto it = mapper.find(mode);
	if (it != mapper.end())
	{
		return it->second;
	}
	throw std::invalid_argument("Unknown mode: " + mode);
}
} // namespace Utils