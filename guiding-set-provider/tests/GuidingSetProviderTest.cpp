#include "guiding-set-provider/GuidingSetProvider.h"
#include "parser/GrammarParser.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

struct FileTestCase
{
	std::string testName;
	std::filesystem::path inputPath;
	std::filesystem::path outputPath;
};

std::string PrintGuidingSets(const GuidingSetsMap& sets)
{
	std::stringstream ss;
	for (const auto& gs : sets)
	{
		ss << gs.ruleIndex << ". " << gs.lhs.GetValue() << " -> ";
		if (gs.rhs.empty())
		{
			ss << "e";
		}
		else
		{
			for (size_t i = 0; i < gs.rhs.size(); ++i)
			{
				ss << gs.rhs[i].GetValue() << (i == gs.rhs.size() - 1 ? "" : " ");
			}
		}
		ss << " [ ";
		for (const auto& sym : gs.selectSet)
		{
			ss << sym.GetValue() << " ";
		}
		ss << "]\n";
	}
	return ss.str();
}

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

class GuidingSetFileTest : public testing::TestWithParam<FileTestCase>
{
};

TEST_P(GuidingSetFileTest, ProcessesCorrectly)
{
	const auto& [name, inputPath, outputPath] = GetParam();

	std::ifstream inputFile(inputPath);
	Grammar grammar = GrammarParser::Parse(inputFile);

	GuidingSetProvider provider(std::move(grammar));
	GuidingSetsMap result = provider.Execute();

	std::string resultStr = PrintGuidingSets(result);

	std::ifstream outputFile(outputPath);
	std::string expectedOutput((std::istreambuf_iterator(outputFile)), std::istreambuf_iterator<char>());

	EXPECT_EQ(Normalize(resultStr), Normalize(expectedOutput)) << "Mismatch in Guiding Set test: " << name;
}

INSTANTIATE_TEST_SUITE_P(
	GuidingSetTests,
	GuidingSetFileTest,
	::testing::ValuesIn(GetTestCasesFromDir("../../guiding-set-provider/tests/guiding-set-data")),
	[](const ::testing::TestParamInfo<FileTestCase>& info) {
		std::string name = info.param.testName;
		for (char& c : name)
			if (!std::isalnum(c))
				c = '_';
		return name;
	});