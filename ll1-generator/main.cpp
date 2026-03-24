#include "./runner/LL1Runner.h"
#include "./table-processing/CsvExporter.h"
#include "./table-processing/TableGenerator.h"
#include "grammar-parser/parser/GrammarParser.h"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> LexInput(const std::string& path)
{
	std::ifstream f(path);
	std::vector<std::string> tokens;
	std::string t;
	while (f >> t)
	{
		tokens.push_back(t);
	}
	return tokens;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		return 1;
	}

	std::string grammarFile = argv[1];
	std::ifstream input(grammarFile);
	auto grammar = GrammarParser::Parse(input);

	Table table = TableGenerator::Generate(grammar);

	CsvExporter::Export(table, "table.csv");

	if (argc == 4 && std::string(argv[2]) == "--run")
	{
		auto tokens = LexInput(argv[3]);
		auto root = LL1Runner::Run(table, tokens);
		if (root)
		{
			std::cout << "Ok. Parse Tree:" << std::endl;
			root->Print();
		}
		else
		{
			std::cout << "Error" << std::endl;
		}
	}

	return 0;
}