#include "Algorithms/ChainRuleEliminator.h"
#include "Algorithms/EpsilonEliminator.h"
#include "CNF/ChomskyNormalForm.h"
#include "CYKValidator.h"
#include "Parser/GrammarParser.h"
#include <fstream>
#include <iostream>

void ShowUsage()
{
	std::cout << "Использование: CYK_Tool <режим> <файл_грамматики> [слово]\n"
			  << "Режимы:\n"
			  << "  1 - Удаление е-продукций\n"
			  << "  2 - Удаление цепных продукций\n"
			  << "  3 - Проверка принадлежности слова (CYK)\n"
			  << "  4 - Приведение к нормальной форме Хомского (НФХ)\n"
			  << "Пример: ./CYK 3 grammar.txt aabaa\n";
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
			EpsilonEliminator eliminator(g);
			Grammar result = eliminator.Execute();
		}
		else if (mode == 2)
		{
			EpsilonEliminator eStep(g);
			Grammar noEps = eStep.Execute();
			ChainRuleEliminator unitEliminator(noEps);
			Grammar result = unitEliminator.Execute();
		}
		else if (mode == 3)
		{
			if (argc < 4)
			{
				std::cerr << "Error: word required." << std::endl;
				return 1;
			}
			std::string word = argv[3];
			if (word == "ε" || word == "e")
			{
				word = "";
			}

			CYKValidator validator(g);
			bool accepted = validator.Validate(word);

			std::cout << "Word: \"" << (word.empty() ? "ε" : word) << "\"\n";
			std::cout << "Result: " << (accepted ? "BELONGS" : "DOES NOT BELONGS") << " to the language." << std::endl;
		}
		else if (mode == 4)
		{
			ChomskyNormalForm cnf(g);
			Grammar result = cnf.Convert();
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
