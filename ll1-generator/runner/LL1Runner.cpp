#include "LL1Runner.h"

std::shared_ptr<ParseTreeNode> LL1Runner::Run(const Table& table, const std::vector<std::string>& tokens)
{
	std::stack<int> addrStack;
	std::stack<std::shared_ptr<ParseTreeNode>> treeStack;

	auto root = std::make_shared<ParseTreeNode>(table[0].currentSymbol);
	treeStack.push(root);

	int rowIdx = 0;
	size_t tokenIdx = 0;

	while (rowIdx >= 0 && rowIdx < static_cast<int>(table.size()))
	{
		const auto& row = table[rowIdx];
		std::string currentToken = (tokenIdx < tokens.size()) ? tokens[tokenIdx] : "⊥";

		if (row.guidingSymbols.contains(currentToken))
		{
			if (ProcessMatch(row, tokenIdx, addrStack, rowIdx, treeStack))
			{
				return root;
			}
		}
		else
		{
			if (row.error)
			{
				std::cerr << "Syntax Error at: " << currentToken << " (Row " << row.id << ")" << std::endl;
				return nullptr;
			}
			rowIdx++;
		}
	}
	return nullptr;
}

bool LL1Runner::ProcessMatch(const TableRow& row, size_t& tIdx,
	std::stack<int>& addrSt, int& rIdx,
	std::stack<std::shared_ptr<ParseTreeNode>>& treeSt)
{
	const auto currentParent = treeSt.top();

	if (row.shift)
	{
		currentParent->children.push_back(std::make_shared<ParseTreeNode>(row.currentSymbol));
		tIdx++;
	}
	else if (row.pointer > 0 && !row.shift)
	{
		if (row.currentSymbol != currentParent->name)
		{
			const auto newNode = std::make_shared<ParseTreeNode>(row.currentSymbol);
			currentParent->children.push_back(newNode);
			if (row.pushStackAddress > 0)
			{
				treeSt.push(newNode);
			}
		}
	}
	if (row.currentSymbol == "ε")
	{
		currentParent->children.push_back(std::make_shared<ParseTreeNode>("ε"));
	}

	if (row.pushStackAddress > 0)
	{
		addrSt.push(row.pushStackAddress);
	}

	if (row.pointer > 0)
	{
		rIdx = row.pointer - 1;
		return false;
	}
	else
	{
		if (addrSt.empty())
		{
			return row.endOfParsing;

		}
		rIdx = addrSt.top() - 1;
		addrSt.pop();
		if (treeSt.size() > 1)
		{
			treeSt.pop();
		}
		return false;
	}
}