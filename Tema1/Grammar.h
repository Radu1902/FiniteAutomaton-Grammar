#pragma once
#include "ProductionRule.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>


using Symbol = char;
constexpr auto lambda = "*";


class Grammar
{
public: 
	Grammar() = default;

	const std::unordered_set<Symbol>& GetNonterminalSymbols() const;
	const std::unordered_set<Symbol>& GetTerminalSymbols() const;

	Symbol GetStartSymbol() const;

	const std::unordered_map<size_t, ProductionRule>GetProductionRules() const;

	void InsertNonterminalSymbol(Symbol symbol);
	void InsertTerminalSymbol(Symbol symbol);
	void SetStartSymbol(Symbol symbol);
	void InsertProductionRule(size_t index, const ProductionRule& productionRule);

	bool VerifyGrammar() const;
	bool IsRegular() const;
	void GenerateWord();
	std::string PassWord();

	friend std::istream& operator>>(std::istream& in, Grammar& grammar);
	friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);

private:
	int Randomizer(unsigned int min, unsigned int max);
	std::vector<size_t> ProductionRulesToApply(const std::string& word);
	bool OnlyTerminalSymbols(const std::string& word) const;

private:
	std::unordered_set<Symbol> m_nonterminalSymbols;
	std::unordered_set<Symbol> m_terminalSymbols;
	Symbol m_startSymbol;
	std::unordered_map<size_t, ProductionRule> m_productionRules;
};

