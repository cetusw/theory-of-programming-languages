#include "ChainRuleEliminator.h"
#include "EpsilonEliminator.h"
#include "GrammarFactorizer.h"
#include "LeftRecursionEliminator.h"
#include "UselessSymbolEliminator.h"
#include "parser/GrammarParser.h"
#include "printer/GrammarPrinter.h"
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

enum class TransformationMode
{
	EPSILON,
	CHAIN,
	USELESS,
	LEFT_RECURSION,
	LEFT_FACTORIZATION
};

Grammar SimplifyGrammar(Grammar g, const std::set<TransformationMode>& modes)
{
	std::cout << "--- [AUTO] Initial Useless Symbol Elimination ---\n";
	g = UselessSymbolEliminator(std::move(g)).Execute();

	if (modes.contains(TransformationMode::EPSILON))
	{
		std::cout << "\n--- Step: Epsilon Elimination ---\n";
		g = EpsilonEliminator(std::move(g)).Execute();
	}

	if (modes.contains(TransformationMode::CHAIN))
	{
		std::cout << "\n--- Step: Chain Rule Elimination ---\n";
		g = ChainRuleEliminator(std::move(g)).Execute();
	}

	if (modes.contains(TransformationMode::USELESS))
	{
		std::cout << "\n--- Step: Useless Symbol Elimination (Full Clean) ---\n";
		g = UselessSymbolEliminator(std::move(g)).Execute();
	}

	if (modes.contains(TransformationMode::LEFT_RECURSION))
	{
		std::cout << "\n--- Step: Left Recursion Elimination ---\n";
		g = LeftRecursionEliminator(std::move(g)).Execute();
	}

	if (modes.contains(TransformationMode::LEFT_FACTORIZATION))
	{
		std::cout << "\n--- Step: Left Factorization ---\n";
		g = GrammarFactorizer(std::move(g)).Execute();
	}

	std::cout << "\n--- [AUTO] Final Useless Symbol Elimination ---\n";
	g = UselessSymbolEliminator(std::move(g)).Execute();

	return g;
}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <input_file> [flags...]\n"
				  << "Flags (can combine multiple):\n"
				  << "  --epsilon    : Eliminate epsilon productions\n"
				  << "  --chain      : Eliminate chain rules\n"
				  << "  --useless    : Eliminate useless symbols (explicit step)\n"
				  << "  --left-rec   : Eliminate left recursion\n"
				  << "  --factor     : Perform left factorization\n";
		return 1;
	}

	std::string filename = argv[1];
	std::set<TransformationMode> selectedModes;

	for (int i = 2; i < argc; ++i)
	{
		std::string flag = argv[i];
		if (flag == "--epsilon")
			selectedModes.insert(TransformationMode::EPSILON);
		else if (flag == "--chain")
			selectedModes.insert(TransformationMode::CHAIN);
		else if (flag == "--useless")
			selectedModes.insert(TransformationMode::USELESS);
		else if (flag == "--left-rec")
			selectedModes.insert(TransformationMode::LEFT_RECURSION);
		else if (flag == "--factor")
			selectedModes.insert(TransformationMode::LEFT_FACTORIZATION);
		else
		{
			std::cerr << "Warning: Unknown flag ignored: " << flag << std::endl;
		}
	}

	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		std::cerr << "Error: Could not open file " << filename << std::endl;
		return 1;
	}

	try
	{
		Grammar grammar = GrammarParser::Parse(inputFile);
		Grammar result = SimplifyGrammar(std::move(grammar), selectedModes);
		std::cout << "\n================ FINAL RESULT ================\n";
		GrammarPrinter::Print(std::cout, result);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Transformation error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}