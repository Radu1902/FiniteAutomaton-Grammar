#include "Grammar.h"
#include <random>
#include <chrono>

const std::unordered_set<Symbol>& Grammar::GetNonterminalSymbols() const
{
	return m_nonterminalSymbols;
}

const std::unordered_set<Symbol>& Grammar::GetTerminalSymbols() const
{
	return m_terminalSymbols;
}

Symbol Grammar::GetStartSymbol() const
{
	return m_startSymbol;
}

const std::unordered_map<size_t, ProductionRule> Grammar::GetProductionRules() const
{
	return m_productionRules;
}

void Grammar::InsertNonterminalSymbol(Symbol symbol)
{
	m_nonterminalSymbols.insert(symbol);
}

void Grammar::InsertTerminalSymbol(Symbol symbol)
{
	m_terminalSymbols.insert(symbol);
}

void Grammar::SetStartSymbol(Symbol symbol)
{
	m_startSymbol = symbol;
}

void Grammar::InsertProductionRule(size_t index, const ProductionRule& productionRule)
{
	m_productionRules.insert(std::make_pair(index, productionRule));
}

bool Grammar::VerifyGrammar() const
{
	for (const auto& nonterminalSymbol : GetNonterminalSymbols())
	{
		if (GetTerminalSymbols().find(nonterminalSymbol) != GetTerminalSymbols().end())
			return false;
	}



	if (GetNonterminalSymbols().find(m_startSymbol) == GetNonterminalSymbols().end())
		return false;



	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftSide = productionRule.second.GetLeftSide();

		bool ok = false;

		for (const auto& nonterminalSymbol : GetNonterminalSymbols())
		{
			if (leftSide.find(nonterminalSymbol) != std::string::npos)
				ok = true;
		}

		if (!ok) return false;
	}



	bool ok = false;
	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftSide = productionRule.second.GetLeftSide();
		if (leftSide.find(m_startSymbol) != std::string::npos && leftSide.size() == 1)
		{
			ok = true;
			break;
		}
	}
	if (!ok) return false;



	std::unordered_set<Symbol> allSymbols(GetNonterminalSymbols());
	for (const auto& terminalSymbol : GetTerminalSymbols())
	{
		allSymbols.insert(terminalSymbol);
	}

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftSide = productionRule.second.GetLeftSide();
		const auto& rightSide = productionRule.second.GetRightSide();

		for (const auto& symbol : leftSide)
		{
			if (allSymbols.find(symbol) == allSymbols.end())
				return false;
		}

		for (const auto& symbol : rightSide)
		{
			if (allSymbols.find(symbol) == allSymbols.end() && symbol != lambda[0])
				return false;
		}
	}


	return true;

}

bool Grammar::IsRegular() const
{
	std::unordered_set<Symbol> terminalSymbols(GetTerminalSymbols());
	std::unordered_set<Symbol> nonterminalSymbols(GetNonterminalSymbols());

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& rightSide = productionRule.second.GetRightSide();
		const auto& leftSide = productionRule.second.GetLeftSide();

		if (leftSide.size() != 1) return false;

		if (nonterminalSymbols.find(leftSide[0]) == nonterminalSymbols.end()) return false;

		if (rightSide.size() > 2 || rightSide.size() <= 0) return false;

		if (terminalSymbols.find(rightSide[0]) == terminalSymbols.end()) return false;

		if (rightSide.size() == 2 && nonterminalSymbols.find(rightSide[1]) == nonterminalSymbols.end()) return false;
	}

	return true;
}

void Grammar::GenerateWord()
{
	static std::unordered_set<std::string> usedWords;

	std::string steps;
	std::string word;
	word.push_back(GetStartSymbol());

	while (true)
	{
		steps += word;

		std::vector<size_t> productionRulesToApply = ProductionRulesToApply(word);

		if (productionRulesToApply.empty())
		{
			if (OnlyTerminalSymbols(word) && usedWords.find(word) == usedWords.end())
			{
				usedWords.insert(word);
				std::cout << steps;
				break;
			}
			else
			{
				steps.clear();
				word.clear();
				word.push_back(GetStartSymbol());
			}
		}
		else
		{
			unsigned int random = Randomizer(0, productionRulesToApply.size() - 1);
			size_t pos = productionRulesToApply[random];

			auto leftSide = GetProductionRules().at(pos).GetLeftSide();
			auto rightSide = GetProductionRules().at(pos).GetRightSide();
			if (rightSide == lambda)
				rightSide = "";

			word.replace(word.find(leftSide), leftSide.length(), rightSide);
			steps += " =(" + std::to_string(pos + 1) + ")=> ";
		}
		productionRulesToApply.clear();
	}
}

std::string Grammar::PassWord()
{
	static std::unordered_set<std::string> usedWords;
	std::string word;
	word.push_back(GetStartSymbol());

	while (true)
	{
		std::vector<size_t> productionRulesToApply = ProductionRulesToApply(word);

		if (productionRulesToApply.empty())
		{
			if (OnlyTerminalSymbols(word) && usedWords.find(word) == usedWords.end())
			{
				usedWords.insert(word);
				return word;
			}
			else
			{
				word.clear();
				word.push_back(GetStartSymbol());
			}
		}
		else
		{
			unsigned int random = Randomizer(0, productionRulesToApply.size() - 1);
			size_t pos = productionRulesToApply[random];

			auto leftSide = GetProductionRules().at(pos).GetLeftSide();
			auto rightSide = GetProductionRules().at(pos).GetRightSide();
			if (rightSide == lambda)
				rightSide = "";

			word.replace(word.find(leftSide), leftSide.length(), rightSide);
		}
		productionRulesToApply.clear();
	}
}

int Grammar::Randomizer(unsigned int min, unsigned int max)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::mt19937_64 gen(seed);
	std::uniform_int_distribution<int> distrib(min, max);

	return distrib(gen);
}

std::vector<size_t> Grammar::ProductionRulesToApply(const std::string& word)
{
	std::vector<size_t> productionRulesToApply;

	for (const auto& productionRule : GetProductionRules())
	{
		const auto& leftSide = productionRule.second.GetLeftSide();
		if (word.find(leftSide) != std::string::npos)
		{
			productionRulesToApply.push_back(productionRule.first);
		}
	}

	return productionRulesToApply;
}

bool Grammar::OnlyTerminalSymbols(const std::string& word) const
{
	for (const auto& symbol : word)
	{
		if (GetTerminalSymbols().find(symbol) == GetTerminalSymbols().end()) return false;
	}
	return true;
}

std::istream& operator>>(std::istream& in, Grammar& grammar)
{
	size_t numberOfNonterminalSymbols;
	in >> numberOfNonterminalSymbols;

	for (size_t index = 0; index < numberOfNonterminalSymbols; ++index)
	{
		Symbol symbol;
		in >> symbol;
		grammar.InsertNonterminalSymbol(symbol);
	}

	size_t numberOfTerminalSymbols;
	in >> numberOfTerminalSymbols;

	for (size_t index = 0; index < numberOfTerminalSymbols; ++index)
	{
		Symbol symbol;
		in >> symbol;
		grammar.InsertTerminalSymbol(symbol);
	}

	in >> grammar.m_startSymbol;

	size_t numberOfProductionRules;
	in >> numberOfProductionRules;

	for (size_t index = 0; index < numberOfProductionRules; ++index)
	{
		ProductionRule productionRule;
		in >> productionRule;
		grammar.InsertProductionRule(index, productionRule);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& grammar)
{
	out << "Grammar = ({";

	for (const auto& nonterminal : grammar.GetNonterminalSymbols())
	{
		out << nonterminal << ", ";
	}
	out << "\b\b}, {";

	for (const auto& terminal : grammar.GetTerminalSymbols())
	{
		out << terminal << ", ";
	}
	out << "\b\b}, " << grammar.GetStartSymbol() << ", {";
	for (const auto& productionRule : grammar.GetProductionRules())
	{
		out << productionRule.second << ", ";
	}
	out << "\b\b})";

	return out;
}
