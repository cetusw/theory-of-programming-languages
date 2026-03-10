#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "../EpsilonEliminator.h"
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
		s.erase(s.find_last_not_of(" \t\n\r") + 1);
		s.erase(0, s.find_first_not_of(" \t\n\r"));
	};
	trim(str);
	return str;
}

class EpsilonEliminatorFileTest : public ::testing::TestWithParam<FileTestCase>
{
};

TEST_P(EpsilonEliminatorFileTest, ProcessesCorrectly)
{
	const auto& [name, inputPath, outputPath] = GetParam();

	std::ifstream inputFile(inputPath);
	ASSERT_TRUE(inputFile.is_open()) << "Could not open input file: " << inputPath;

	Grammar grammar = GrammarParser::Parse(inputFile);

	EpsilonEliminator eliminator(std::move(grammar));
	Grammar result = eliminator.Execute();

	std::stringstream resultStream;
	GrammarPrinter::Print(resultStream, result);
	std::string actualOutput = Normalize(resultStream.str());

	std::ifstream outputFile(outputPath);
	ASSERT_TRUE(outputFile.is_open()) << "Could not open output file: " << outputPath;

	std::string expectedOutput((std::istreambuf_iterator<char>(outputFile)),
		std::istreambuf_iterator<char>());
	expectedOutput = Normalize(expectedOutput);

	EXPECT_EQ(actualOutput, expectedOutput) << "Mismatch in test case: " << name;
}

std::vector<FileTestCase> GetTestCases()
{
	std::vector<FileTestCase> cases;
	const fs::path testDataDir = "../../grammar-simplifier/tests/data";

	if (!fs::exists(testDataDir))
	{
		std::cerr << "Warning: Test data directory not found at " << testDataDir << std::endl;
		return cases;
	}

	const fs::path inputDir = testDataDir / "input";
	const fs::path outputDir = testDataDir / "output";

	if (!fs::exists(inputDir) || !fs::exists(outputDir))
	{
		return cases;
	}

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
			else
			{
				std::cerr << "Warning: Found input " << filename
						  << " but NO output in " << outPath << std::endl;
			}
		}
	}
	return cases;
}

INSTANTIATE_TEST_SUITE_P(
	FileSystemTests,
	EpsilonEliminatorFileTest,
	::testing::ValuesIn(GetTestCases()),
	[](const ::testing::TestParamInfo<FileTestCase>& info) {
		std::string name = info.param.testName;
		for (char& c : name)
		{
			if (!std::isalnum(c))
				c = '_';
		}
		return name;
	});