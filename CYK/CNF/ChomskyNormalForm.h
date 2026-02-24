#ifndef CYK_CHOMSKYNORMALFORM_H
#define CYK_CHOMSKYNORMALFORM_H

#include "../Logger/Logger.h"
#include "../Types/Grammar.h"
#include <map>
#include <queue>

class ChomskyNormalForm : public Logger
{
public:
	explicit ChomskyNormalForm(Grammar g);

	Grammar Convert();

private:
	Grammar m_grammar;
	int m_newNtCounter = 0;
	std::map<std::string, Symbol> m_terminalMap;
	std::queue<std::string> m_freeLetters;
	std::map<std::vector<Symbol>, Symbol> m_binarizationMap;

	void InitLetterPool(const Grammar& g);
	std::string GetNextLetter();
	Grammar FinalTransform(const Grammar& src);
	void ProcessRule(Grammar& result, const Production& rule);
	std::vector<Symbol> ReplaceTerminals(Grammar& result, const std::vector<Symbol>& rhs);
	void SplitLongRule(Grammar& res, const Symbol& lhs, std::vector<Symbol> rhs);
};

#endif // CYK_CHOMSKYNORMALFORM_H
