#pragma once

#include "../Types/Grammar.h"

class IGrammarTransformer
{
public:
	virtual ~IGrammarTransformer() = default;
	virtual Grammar Execute(const Grammar& g) = 0;
};