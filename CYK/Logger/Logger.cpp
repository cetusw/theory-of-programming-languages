#include "Logger.h"
#include <map>

void Logger::LogStep(const std::string& message)
{
	std::cout << "[STEP] " << message << "\n";
}

void Logger::LogGrammar(const Grammar& grammar, const std::string& title)
{
	std::cout << "--- " << title << " ---\n";
	PrintGrammar(grammar);
	std::cout << std::endl;
}

void Logger::PrintGrammar(const Grammar& g)
{
	const auto rules = g.GetRules();
	if (rules.empty())
	{
		std::cerr << "Grammar is empty or all symbols was removed as useless" << std::endl;
		return;
	}
	const auto stringRules = GetStringRules(rules);
	const std::string start = g.GetStartSymbol().GetValue();
	PrintStartRule(stringRules, start);
	PrintRules(stringRules, start);

	std::cout << std::endl;
}

std::map<std::string, std::vector<std::string>> Logger::GetStringRules(const std::vector<Production>& rules)
{
	std::map<std::string, std::vector<std::string>> stringRules;
	for (const auto& rule : rules)
	{
		std::string rhs;
		if (rule.IsEpsilon())
		{
			rhs = "ε";
		}
		else
		{
			for (const auto& symbol : rule.GetRhs())
			{
				rhs += symbol.GetValue();
			}
		}
		stringRules[rule.GetLhs().GetValue()].push_back(rhs);
	}

	return stringRules;
}

void Logger::PrintStartRule(
	std::map<std::string, std::vector<std::string>> stringRules, const std::string& start)
{
	if (stringRules.count(start))
	{
		std::cout << start << RULE_OPERATOR;
		for (size_t i = 0; i < stringRules[start].size(); ++i)
		{
			std::cout << stringRules[start][i] << (i == stringRules[start].size() - 1 ? "" : OR_OPERATOR);
		}
		std::cout << std::endl;
	}
}

void Logger::PrintRules(
	const std::map<std::string, std::vector<std::string>>& stringRules, const std::string& start)
{
	for (const auto& rule : stringRules)
	{
		if (rule.first == start)
		{
			continue;
		}
		std::cout << rule.first << RULE_OPERATOR;
		for (size_t i = 0; i < rule.second.size(); ++i)
		{
			std::cout << rule.second[i] << (i == rule.second.size() - 1 ? "" : OR_OPERATOR);
		}
		std::cout << std::endl;
	}
}
