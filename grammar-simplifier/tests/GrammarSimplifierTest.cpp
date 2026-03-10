#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "../ChainRuleEliminator.h"
#include "../EpsilonEliminator.h"
#include "../UselessSymbolEliminator.h"
#include "parser/GrammarParser.h"
#include "printer/GrammarPrinter.h"

namespace fs = std::filesystem;

struct FileTestCase
{
	std::string testName;
	fs::path inputPath;
	fs::path outputPath;
};

std::string Normalize(std::string str)
{
	auto trim = [](std::string& s) {
		const auto whitespace = " \t\n\r";
		const size_t first = s.find_first_not_of(whitespace);
		if (first == std::string::npos)
		{
			s = "";
			return;
		}
		const size_t last = s.find_last_not_of(whitespace);
		s = s.substr(first, last - first + 1);
	};
	trim(str);
	return str;
}

std::vector<FileTestCase> GetTestCasesFromDir(const fs::path& testDataDir)
{
	std::vector<FileTestCase> cases;

	if (!fs::exists(testDataDir))
	{
		std::cerr << "Warning: Data directory not found at " << testDataDir << std::endl;
		return cases;
	}

	const fs::path inputDir = testDataDir / "input";
	const fs::path outputDir = testDataDir / "output";

	if (!fs::exists(inputDir) || !fs::exists(outputDir))
		return cases;

	for (const auto& entry : fs::directory_iterator(inputDir))
	{
		std::string filename = entry.path().filename().string();
		std::string inSuffix = "-in.txt";
		std::string outSuffix = "-out.txt";

		if (filename.size() > inSuffix.size() && filename.compare(filename.size() - inSuffix.size(), inSuffix.size(), inSuffix) == 0)
		{
			std::string testPrefix = filename.substr(0, filename.size() - inSuffix.size());
			std::string outFilename = testPrefix + outSuffix;
			fs::path outPath = outputDir / outFilename;

			if (fs::exists(outPath))
			{
				cases.push_back({ testPrefix, entry.path(), outPath });
			}
		}
	}
	return cases;
}

class EpsilonEliminatorFileTest : public ::testing::TestWithParam<FileTestCase>
{
};

TEST_P(EpsilonEliminatorFileTest, ProcessesCorrectly)
{
	const auto& [name, inputPath, outputPath] = GetParam();

	std::ifstream inputFile(inputPath);
	Grammar grammar = GrammarParser::Parse(inputFile);

	EpsilonEliminator eliminator(std::move(grammar));
	Grammar result = eliminator.Execute();

	std::stringstream resultStream;
	GrammarPrinter::Print(resultStream, result);

	std::ifstream outputFile(outputPath);
	std::string expectedOutput((std::istreambuf_iterator<char>(outputFile)), std::istreambuf_iterator<char>());

	EXPECT_EQ(Normalize(resultStream.str()), Normalize(expectedOutput)) << "Mismatch in Epsilon test: " << name;
}

INSTANTIATE_TEST_SUITE_P(
	EpsilonTests,
	EpsilonEliminatorFileTest,
	::testing::ValuesIn(GetTestCasesFromDir("../../grammar-simplifier/tests/epsilon-eliminator-data")),
	[](const ::testing::TestParamInfo<FileTestCase>& info) {
		std::string name = info.param.testName;
		for (char& c : name)
			if (!std::isalnum(c))
				c = '_';
		return name;
	});

class ChainRuleEliminatorFileTest : public ::testing::TestWithParam<FileTestCase>
{
};

TEST_P(ChainRuleEliminatorFileTest, ProcessesCorrectly)
{
	const auto& [name, inputPath, outputPath] = GetParam();

	std::ifstream inputFile(inputPath);
	Grammar grammar = GrammarParser::Parse(inputFile);

	ChainRuleEliminator eliminator(std::move(grammar));
	Grammar result = eliminator.Execute();

	std::stringstream resultStream;
	GrammarPrinter::Print(resultStream, result);

	std::ifstream outputFile(outputPath);
	std::string expectedOutput((std::istreambuf_iterator(outputFile)), std::istreambuf_iterator<char>());

	EXPECT_EQ(Normalize(resultStream.str()), Normalize(expectedOutput)) << "Mismatch in Chain test: " << name;
}

INSTANTIATE_TEST_SUITE_P(
	ChainTests,
	ChainRuleEliminatorFileTest,
	::testing::ValuesIn(GetTestCasesFromDir("../../grammar-simplifier/tests/chain-rule-eliminator-data")),
	[](const ::testing::TestParamInfo<FileTestCase>& info) {
		std::string name = info.param.testName;
		for (char& c : name)
			if (!std::isalnum(c))
				c = '_';
		return name;
	});

class UselessSymbolEliminatorFileTest : public ::testing::TestWithParam<FileTestCase>
{
};

TEST_P(UselessSymbolEliminatorFileTest, ProcessesCorrectly)
{
	const auto& [name, inputPath, outputPath] = GetParam();

	std::ifstream inputFile(inputPath);
	Grammar grammar = GrammarParser::Parse(inputFile);

	UselessSymbolEliminator eliminator(std::move(grammar));
	Grammar result = eliminator.Execute();

	std::stringstream resultStream;
	GrammarPrinter::Print(resultStream, result);

	std::ifstream outputFile(outputPath);
	std::string expectedOutput((std::istreambuf_iterator(outputFile)), std::istreambuf_iterator<char>());

	EXPECT_EQ(Normalize(resultStream.str()), Normalize(expectedOutput)) << "Mismatch in Chain test: " << name;
}

INSTANTIATE_TEST_SUITE_P(
	UselessSymbolTests,
	UselessSymbolEliminatorFileTest,
	::testing::ValuesIn(GetTestCasesFromDir("../../grammar-simplifier/tests/useless-symbol-eliminator-data")),
	[](const ::testing::TestParamInfo<FileTestCase>& info) {
		std::string name = info.param.testName;
		for (char& c : name)
			if (!std::isalnum(c))
				c = '_';
		return name;
	});