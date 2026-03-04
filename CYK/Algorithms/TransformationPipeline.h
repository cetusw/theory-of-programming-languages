#pragma once

#include "IGrammarTransformer.h"
#include <memory>
#include <vector>

class TransformationPipeline
{
public:
	void AddStep(std::unique_ptr<IGrammarTransformer> step);
	[[nodiscard]] Grammar Run(const Grammar& startGrammar) const;

private:
	std::vector<std::unique_ptr<IGrammarTransformer>> m_steps;
};
