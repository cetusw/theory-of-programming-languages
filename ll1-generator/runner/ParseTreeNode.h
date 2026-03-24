#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct ParseTreeNode
{
	std::string name;
	std::vector<std::shared_ptr<ParseTreeNode>> children;

	explicit ParseTreeNode(std::string val)
		: name(std::move(val))
	{
	}

	void Print(const std::string& indent = "", bool isLast = true) const
	{
		std::cout << indent << (isLast ? "└── " : "├── ") << name << std::endl;
		for (size_t i = 0; i < children.size(); ++i)
		{
			children[i]->Print(indent + (isLast ? "    " : "│   "), i == children.size() - 1);
		}
	}
};
