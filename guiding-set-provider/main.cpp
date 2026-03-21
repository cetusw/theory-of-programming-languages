#include "GuidingSetProvider.h"
#include "parser/GrammarParser.h"
#include <fstream>
#include <iomanip>
#include <iostream>

void PrintResults(const GuidingSetsMap& sets)
{
	if (sets.empty())
	{
		std::cout << "No rules found or empty grammar." << std::endl;
		return;
	}

	for (const auto& gs : sets)
	{
		std::cout << gs.ruleIndex << ". " << gs.lhs.GetValue() << " -> ";

		if (gs.rhs.empty())
		{
			std::cout << "e";
		}
		else
		{
			for (size_t i = 0; i < gs.rhs.size(); ++i)
			{
				std::cout << gs.rhs[i].GetValue() << (i == gs.rhs.size() - 1 ? "" : " ");
			}
		}

		std::cout << " [ ";
		for (const auto& sym : gs.selectSet)
		{
			std::cout << sym.GetValue() << " ";
		}
		std::cout << "]\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <INPUT_FILE>" << std::endl;
		return 1;
	}

	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
		return 1;
	}

	try
	{
		Grammar grammar = GrammarParser::Parse(inputFile);

		GuidingSetProvider provider(std::move(grammar));
		GuidingSetsMap result = provider.Execute();

		PrintResults(result);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error during processing: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}