#ifndef CYK_LOGGER_H
#define CYK_LOGGER_H

#include "../Types/Grammar.h"
#include <iostream>
#include <map>
#include <string>

class Logger
{
public:
	static void LogStep(const std::string& message);
	static void LogGrammar(const Grammar& grammar, const std::string& title);

	static constexpr std::string_view RULE_OPERATOR = " -> ";
	static constexpr std::string_view OR_OPERATOR = " | ";

private:
	static void PrintGrammar(const Grammar& g);
	static std::map<std::string, std::vector<std::string>> GetStringRules(const std::vector<Production>& rules);
	static void PrintStartRule(std::map<std::string, std::vector<std::string>> stringRules, const std::string& start);
	static void PrintRules(const std::map<std::string, std::vector<std::string>>& stringRules, const std::string& start);
};

#endif // CYK_LOGGER_H
