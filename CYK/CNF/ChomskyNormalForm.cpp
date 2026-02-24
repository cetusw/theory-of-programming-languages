#include "ChomskyNormalForm.h"
#include "../Algorithms/ChainRuleEliminator.h"
#include "../Algorithms/EpsilonEliminator.h"
#include "../Algorithms/UselessSymbolEliminator.h"

ChomskyNormalForm::ChomskyNormalForm(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar ChomskyNormalForm::Convert()
{
	LogStep("Converting to CNF");

	Grammar g1 = EpsilonEliminator(m_grammar).Execute();
	Grammar g2 = ChainRuleEliminator(g1).Execute();
	Grammar g3 = UselessSymbolEliminator(g2).Execute();

	InitLetterPool(g3);

	Grammar result = FinalTransform(g3);
	EpsilonEliminator::Cleanup(result);
	LogGrammar(result, "Grammar in CNF");
	return result;
}

void ChomskyNormalForm::InitLetterPool(const Grammar& g)
{
	std::set<std::string> used;
	for (const auto& nonTerminal : g.GetNonTerminals())
	{
		used.insert(nonTerminal.GetValue());
	}

	for (char c = 'A'; c <= 'Z'; ++c)
	{
		std::string s(1, c);
		if (used.find(s) == used.end())
		{
			m_freeLetters.push(s);
		}
	}

	if (m_freeLetters.empty())
	{
		m_freeLetters.emplace("X_NEW");
	}
}

std::string ChomskyNormalForm::GetNextLetter()
{
	if (m_freeLetters.empty())
	{
		return "Z" + std::to_string(rand() % 100);
	}
	std::string res = m_freeLetters.front();
	m_freeLetters.pop();
	return res;
}

Grammar ChomskyNormalForm::FinalTransform(const Grammar& src)
{
	Grammar result;
	result.SetStartSymbol(src.GetStartSymbol());
	for (const auto& rule : src.GetRules())
	{
		ProcessRule(result, rule);
	}
	return result;
}

void ChomskyNormalForm::ProcessRule(Grammar& result, const Production& rule)
{
	if (rule.GetRhs().size() <= 1)
	{
		result.AddRule(rule.GetLhs(), rule.GetRhs());
		return;
	}
	const std::vector<Symbol> binarizedRhs = ReplaceTerminals(result, rule.GetRhs());
	SplitLongRule(result, rule.GetLhs(), binarizedRhs);
}

std::vector<Symbol> ChomskyNormalForm::ReplaceTerminals(Grammar& result, const std::vector<Symbol>& rhs)
{
	std::vector<Symbol> newRhs;
	for (const auto& s : rhs)
	{
		if (s.IsTerminal())
		{
			if (!m_terminalMap.count(s.GetValue()))
			{
				Symbol newNt = { GetNextLetter(), false };
				m_terminalMap[s.GetValue()] = newNt;
				result.AddRule(newNt, { s });
			}
			newRhs.push_back(m_terminalMap[s.GetValue()]);
		}
		else
			newRhs.push_back(s);
	}
	return newRhs;
}

void ChomskyNormalForm::SplitLongRule(Grammar& res, const Symbol& lhs, std::vector<Symbol> rhs)
{
	if (rhs.size() <= 2)
	{
		res.AddRule(lhs, rhs);
		return;
	}

	const std::vector tail(rhs.begin() + 1, rhs.end());

	Symbol nextNt;

	if (m_binarizationMap.count(tail))
	{
		nextNt = m_binarizationMap[tail];
	}
	else
	{
		nextNt = { GetNextLetter(), false };
		m_binarizationMap[tail] = nextNt;
	}

	res.AddRule(lhs, { rhs[0], nextNt });

	SplitLongRule(res, nextNt, tail);
}