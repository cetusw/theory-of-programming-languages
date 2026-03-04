#include "TransformationPipeline.h"

void TransformationPipeline::AddStep(std::unique_ptr<IGrammarTransformer> step)
{
	m_steps.push_back(std::move(step));
}

Grammar TransformationPipeline::Run(const Grammar& startGrammar) const
{
	Grammar current = startGrammar;
	for (const auto& step : m_steps)
	{
		current = step->Execute(current);
	}
	return current;
}