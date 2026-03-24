#include "TableGenerator.h"

#include "AddressMap.h"

Table TableGenerator::Generate(const Grammar& grammar)
{
	GuidingSetProvider provider(grammar);
	const auto guidingSets = provider.Execute();

	AddressMap addr;
	addr.Calculate(grammar.GetRules());

	Table table;
	GenerateNTEntries(grammar, guidingSets, addr, table);
	GenerateRhsRows(grammar, guidingSets, addr, table, provider, addr.acceptRowId);

	TableRow acceptRow;
	acceptRow.id = static_cast<int>(table.size()) + 1;
	acceptRow.currentSymbol = "⊥";
	acceptRow.guidingSymbols = { "⊥" };
	acceptRow.shift = false;
	acceptRow.error = true;
	acceptRow.pointer = 0;
	acceptRow.pushStackAddress = 0;
	acceptRow.endOfParsing = true;
	table.push_back(acceptRow);

	return table;
}

void TableGenerator::GenerateNTEntries(const Grammar& g, const GuidingSetsMap& sets, const AddressMap& addr, Table& table)
{
	const auto rules = g.GetRules();
	for (int i = 0; i < rules.size(); ++i)
	{
		TableRow row;
		row.id = static_cast<int>(table.size()) + 1;
		row.currentSymbol = rules[i].GetLhs().GetValue();

		for (const auto& sym : sets[i].selectSet)
		{
			row.guidingSymbols.insert(sym.GetValue());
		}

		row.shift = false;
		row.error = IsLastAlternative(rules, i);
		row.pointer = addr.rhsRows.at(i);
		row.pushStackAddress = 0;
		row.endOfParsing = false;

		table.push_back(row);
	}
}

void TableGenerator::GenerateRhsRows(const Grammar& g, const GuidingSetsMap& sets, const AddressMap& addr, Table& table, const GuidingSetProvider& provider, const int acceptId)
{
	const auto rules = g.GetRules();
	const Symbol startSymbol = g.GetStartSymbol();

	for (size_t i = 0; i < rules.size(); ++i)
	{
		auto rhs = rules[i].GetRhs();
		const bool isStartRule = rules[i].GetLhs() == startSymbol;

		if (rhs.empty())
		{
			table.push_back(CreateEpsilonRow(static_cast<int>(table.size()) + 1, sets[i]));
			continue;
		}

		for (size_t j = 0; j < rhs.size(); ++j)
		{
			const bool isLast = j == rhs.size() - 1;
			const int forcedReturn = (isStartRule && isLast) ? acceptId : 0;
			table.push_back(CreateSymbolRow(static_cast<int>(table.size()) + 1, rhs[j], isLast, isStartRule, addr, provider, forcedReturn));
		}
	}
}

TableRow TableGenerator::CreateSymbolRow(const int id, const Symbol& s, const bool isLast, const bool isStartRule, const AddressMap& addr, const GuidingSetProvider& provider, int forcedReturn)
{
	TableRow r{};
	r.id = id;
	r.currentSymbol = s.GetValue();
	r.shift = s.IsTerminal();
	r.error = true;
	r.endOfParsing = false;

	if (s.IsTerminal())
	{
		r.guidingSymbols = { s.GetValue() };
		r.pointer = isLast ? 0 : id + 1;
		r.pushStackAddress = isStartRule && isLast ? forcedReturn : 0;
	}
	else
	{
		const auto firsts = provider.GetFirstSets().at(s);
		for (auto& f : firsts)
		{
			if (!f.IsEpsilon())
			{
				r.guidingSymbols.insert(f.GetValue());
			}
		}

		if (provider.GetNullableSets().contains(s))
		{
			const auto follows = provider.GetFollowSets().at(s);
			for (auto& f : follows)
			{
				r.guidingSymbols.insert(f.GetValue());
			}
		}

		r.pointer = addr.ntRows.at(s.GetValue());

		if (isLast)
		{
			r.pushStackAddress = isStartRule ? forcedReturn : 0;
		}
		else
		{
			r.pushStackAddress = id + 1;
		}
	}
	return r;
}

TableRow TableGenerator::CreateEpsilonRow(const int id, const GuidingSet& gs)
{
	TableRow r;
	r.id = id;
	r.currentSymbol = "ε";
	for (const auto& sym : gs.selectSet)
	{
		r.guidingSymbols.insert(sym.GetValue());
	}
	r.shift = false;
	r.error = true;
	r.pointer = 0;
	r.pushStackAddress = 0;
	r.endOfParsing = false;
	return r;
}

bool TableGenerator::IsLastAlternative(const std::vector<Production>& rules, const size_t index)
{
	if (index == rules.size() - 1)
	{
		return true;
	}
	return rules[index].GetLhs() != rules[index + 1].GetLhs();
}