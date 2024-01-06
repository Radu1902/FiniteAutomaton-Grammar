#include <iostream>
#include <fstream>
#include "Grammar.h"
#include "FiniteAutomaton.h"

void generateAutomaton(const Grammar& grammar, FiniteAutomaton& automaton)
{
	for (auto neterminal : grammar.GetNonterminalSymbols())
	{
		automaton.InsertState(std::string(1, neterminal));
	}

	for (auto terminal : grammar.GetTerminalSymbols())
	{
		automaton.InsertSymbol(terminal);
	}

	automaton.SetInitialState(std::string(1, grammar.GetStartSymbol()));

	automaton.InsertFinalState("T");

	for (auto& [index, prodRule] : grammar.GetProductionRules())
	{
		State state = prodRule.GetLeftSide();
		std::string rhs = prodRule.GetRightSide();
		Symbol symbol = rhs[0];

		if (rhs.length() == 2)
		{
			State state2 = std::string(1, rhs[1]);
			automaton.InsertTransition(std::make_pair(state, symbol), state2);
		}
		else
		{
			automaton.InsertTransition(std::make_pair(state, symbol), "T");
		}
	}
}

int main()
{
	Grammar grammar;
	std::ifstream fin("elements.txt");
	fin >> grammar;

	if (grammar.VerifyGrammar())
	{
		std::cout << "It's a generative grammar!\n";

		if (grammar.IsRegular())
		{
			FiniteAutomaton automaton;

			size_t option;
			do
			{
				std::cout << "Generate word and verify it using the automaton.... 5 \n";
				std::cout << "Verify word using the automaton.................... 4 \n";
				std::cout << "Generate automaton................................. 3 \n";
				std::cout << "Generate words..................................... 2 \n";
				std::cout << "Show grammar....................................... 1 \n";
				std::cout << "Exit............................................... 0 \n";
				std::cout << "Choose your option: ";
				std::cin >> option;
				switch (option)
				{
				case 0:
					break;
				case 1:
					std::cout << grammar << std::endl;
					break;
				case 2:
					size_t numberOfWords;
					std::cout << "Enter number of words : ";
					std::cin >> numberOfWords;

					for (size_t index = 0; index < numberOfWords; ++index)
					{
						std::cout << index + 1 << ". ";
						grammar.GenerateWord();
						std::cout << std::endl;
					}
					break;
				case 3:

					generateAutomaton(grammar, automaton);
					std::cout << automaton << std::endl;

					break;
				case 4:

					try
					{
						std::string word;
						std::cout << "Write a word: ";
						std::cin >> word;
						automaton.CheckWord(automaton.GetInitialState(), word, 0);
					}
					catch (const char* message)
					{
						std::cout << message;
					}
					break;

				case 5:
					std::string genWord = grammar.PassWord();
					FiniteAutomaton automata;
					generateAutomaton(grammar, automata);
					automaton.CheckWord(automata.GetInitialState(), genWord, 0);
					break;
				}
				std::cout << "\n\n";

			} while (option != 0);


		}
		else
		{
			std::cout << "The grammar is not regular!\n";
		}
	}
	else
	{
		std::cout << "It's not a generative grammar !\n";
	}

	return 0;
}
