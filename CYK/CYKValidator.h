#ifndef CYK_CYKVALIDATOR_H
#define CYK_CYKVALIDATOR_H

#include "Grammar.h"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

class CYKValidator
{
public:
	explicit CYKValidator(Grammar g)
		: m_grammar(std::move(g))
	{
	}

	[[nodiscard]] bool Validate(const std::string& input) const
	{
		if (input.empty())
		{
			return CheckEmptyString();
		}

		const int n = static_cast<int>(input.length());
		std::vector table(n + 1, std::vector<std::set<Symbol>>(n));

		FillBaseCase(table, input);
		FillRecursiveStep(table, n);

		const Symbol startSymbol = m_grammar.GetStartSymbol();
		return table[n][0].count(startSymbol);
	}

private:
	Grammar m_grammar;

	void FillBaseCase(std::vector<std::vector<std::set<Symbol>>>& table, const std::string& input) const
	{
		for (int i = 0; i < input.length(); ++i)
		{
			std::string terminal(1, input[i]);
			for (const auto& rule : m_grammar.GetRules())
			{
				if (rule.rhs.size() == 1 && rule.rhs[0].value == terminal)
				{
					table[1][i].insert(rule.lhs);
				}
			}
		}
	}

	void FillRecursiveStep(std::vector<std::vector<std::set<Symbol>>>& table, const int n) const
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

	void ApplyRules(std::vector<std::vector<std::set<Symbol>>>& table, const int len, const int i, const int k) const
	{
		const auto& setB = table[k][i];
		const auto& setC = table[len - k][i + k];

		for (const auto& rule : m_grammar.GetRules())
		{
			if (rule.rhs.size() == 2)
			{
				const Symbol& B = rule.rhs[0];
				const Symbol& C = rule.rhs[1];

				if (setB.count(B) && setC.count(C))
				{
					table[len][i].insert(rule.lhs);
				}
			}
		}
	}

	[[nodiscard]] bool CheckEmptyString() const
	{
		const Symbol start = m_grammar.GetStartSymbol();
		const auto rules = m_grammar.GetRules();
		return std::any_of(rules.begin(), rules.end(), [&start](const Production& rule) {
			return rule.lhs == start && rule.rhs.empty();
		});
	}
};

#endif // CYK_CYKVALIDATOR_H
