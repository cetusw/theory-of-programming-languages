#include "CYKValidator.h"

#include "CNF/ChomskyNormalForm.h"

#include <algorithm>

CYKValidator::CYKValidator(Grammar g)
	: m_grammar(std::move(g))
{
}

bool CYKValidator::Validate(const std::string& input)
{
	LogStep("CYK Validating");

	if (input.empty())
	{
		return CheckEmptyString();
	}

	ChomskyNormalForm cnf(m_grammar);
	m_grammar = cnf.Convert();

	const int n = static_cast<int>(input.length());
	std::vector table(n + 1, std::vector<std::set<Symbol>>(n));

	FillBaseCase(table, input);
	FillRecursiveStep(table, n);

	const Symbol startSymbol = m_grammar.GetStartSymbol();

	return table[n][0].count(startSymbol);
}

void CYKValidator::FillBaseCase(
	std::vector<std::vector<std::set<Symbol>>>& table, const std::string& input) const
{
	for (int i = 0; i < input.length(); ++i)
	{
		std::string terminal(1, input[i]);
		for (const auto& rule : m_grammar.GetRules())
		{
			if (rule.GetRhs().size() == 1 && rule.GetRhs()[0].GetValue() == terminal)
			{
				table[1][i].insert(rule.GetLhs());
			}
		}
	}
}

void CYKValidator::FillRecursiveStep(std::vector<std::vector<std::set<Symbol>>>& table, const int n) const
{
	for (int len = 2; len <= n; ++len)
	{
		for (int i = 0; i <= n - len; ++i)
		{
			for (int k = 1; k < len; ++k)
			{
				ApplyRules(table, len, i, k);
			}
		}
	}
}
void CYKValidator::ApplyRules(
	std::vector<std::vector<std::set<Symbol>>>& table, const int len, const int i, const int k) const
{
	const auto& setB = table[k][i];
	const auto& setC = table[len - k][i + k];

	for (const auto& rule : m_grammar.GetRules())
	{
		if (rule.GetRhs().size() == 2)
		{
			const Symbol B = rule.GetRhs()[0];
			const Symbol C = rule.GetRhs()[1];

			if (setB.count(B) && setC.count(C))
			{
				table[len][i].insert(rule.GetLhs());
			}
		}
	}
}

bool CYKValidator::CheckEmptyString() const
{
	const Symbol start = m_grammar.GetStartSymbol();
	const auto rules = m_grammar.GetRules();
	return std::any_of(rules.begin(), rules.end(), [&start](const Production& rule) {
		return rule.GetLhs() == start && rule.GetRhs().empty();
	});
}