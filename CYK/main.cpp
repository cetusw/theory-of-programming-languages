#include "CYKValidator.h"
#include "ChomskyNormalForm.h"
#include "EpsilonEliminator.h"
#include "Grammar.h"
#include "GrammarParser.h"
#include "UnitRuleEliminator.h"

#include <fstream>
#include <iostream>
#include <map>

void PrintGrammar(const Grammar& g)
{
	const auto rules = g.GetRules();
	if (rules.empty())
	{
		std::cout << "Грамматика пуста или все символы были удалены как бесполезные." << std::endl;
		return;
	}

	std::map<std::string, std::vector<std::string>> grouped;
	for (const auto& r : rules)
	{
		std::string rhs;
		if (r.rhs.empty())
		{
			rhs = "ε";
		}
		else
		{
			for (const auto& sym : r.rhs)
			{
				rhs += sym.value;
			}
		}
		grouped[r.lhs.value].push_back(rhs);
	}

	const std::string start = g.GetStartSymbol().value;
	if (grouped.count(start))
	{
		std::cout << start << " -> ";
		for (size_t i = 0; i < grouped[start].size(); ++i)
		{
			std::cout << grouped[start][i] << (i == grouped[start].size() - 1 ? "" : " | ");
		}
		std::cout << std::endl;
	}

	for (const auto& [lhs, rhsList] : grouped)
	{
		if (lhs == start)
		{
			continue;
		}
		std::cout << lhs << " -> ";
		for (size_t i = 0; i < rhsList.size(); ++i)
		{
			std::cout << rhsList[i] << (i == rhsList.size() - 1 ? "" : " | ");
		}
		std::cout << std::endl;
	}
}

void ShowUsage()
{
	std::cout << "Использование: CYK_Tool <режим> <файл_грамматики> [слово]\n"
			  << "Режимы:\n"
			  << "  1 - Удаление е-продукций\n"
			  << "  2 - Удаление цепных продукций\n"
			  << "  3 - Проверка принадлежности слова (CYK)\n"
			  << "Пример: ./CYK_Tool 3 grammar.txt aabaa\n";
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		ShowUsage();
		return 1;
	}

	int mode = std::stoi(argv[1]);
	std::string filename = argv[2];

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
		return 1;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line))
	{
		if (!line.empty())
		{
			lines.push_back(line);
		}
	}
	file.close();

	try
	{
		Grammar g = GrammarParser::Parse(lines);

		if (mode == 1)
		{
			std::cout << "--- Удаление е-продукций ---" << std::endl;
			EpsilonEliminator eliminator(g);
			Grammar result = eliminator.Execute();
			PrintGrammar(result);
		}
		else if (mode == 2)
		{
			std::cout << "--- Удаление цепных продукций ---" << std::endl;
			EpsilonEliminator eStep(g);
			Grammar noEps = eStep.Execute();

			UnitRuleEliminator unitEliminator(noEps);
			Grammar result = unitEliminator.Execute();
			PrintGrammar(result);
		}
		else if (mode == 3)
		{
			if (argc < 4)
			{
				std::cerr << "Ошибка: Для режима 3 необходимо указать слово." << std::endl;
				return 1;
			}
			std::string word = argv[3];
			if (word == "ε" || word == "e")
			{
				word = "";
			}

			std::cout << "--- Алгоритм Кока-Янгера-Касами ---" << std::endl;

			ChomskyNormalForm cnf(g);
			Grammar cnfGrammar = cnf.Convert();

			CYKValidator validator(cnfGrammar);
			bool accepted = validator.Validate(word);

			std::cout << "Слово: \"" << (word.empty() ? "ε" : word) << "\"\n";
			std::cout << "Результат: " << (accepted ? "ПРИНАДЛЕЖИТ" : "НЕ ПРИНАДЛЕЖИТ") << " языку." << std::endl;
		}
		else
		{
			std::cerr << "Ошибка: Неверный режим." << std::endl;
			return 1;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Произошла ошибка при обработке: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
