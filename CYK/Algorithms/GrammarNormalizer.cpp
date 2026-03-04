#include "GrammarNormalizer.h"

#include <algorithm>

GrammarNormalizer::GrammarNormalizer(std::shared_ptr<ILogger> logger)
	: m_logger(std::move(logger))
{
}

Grammar GrammarNormalizer::Execute(const Grammar& grammar)
{
	Grammar result = grammar;
	auto rules = result.GetRules();
	std::sort(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
		if (a.GetLhs().GetValue() != b.GetLhs().GetValue())
		{
			return a.GetLhs().GetValue() < b.GetLhs().GetValue();
		}
		return a.GetRhs() < b.GetRhs();
	});
	rules.erase(std::unique(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
		return a.GetLhs() == b.GetLhs() && a.GetRhs() == b.GetRhs();
	}),
		rules.end());
	result.SetRules(rules);
	m_logger->LogGrammar(result, "After Normalization");
	return result;
}