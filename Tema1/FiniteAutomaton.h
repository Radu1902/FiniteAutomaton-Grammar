#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

class Hash
{
public:
	template<class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) + std::hash<T2>()(pair.second);
	}
};

using State = std::string;
using Symbol = char;
using TransitionTable = std::unordered_map<std::pair<State, Symbol>, std::vector<State>, Hash>;

class FiniteAutomaton
{
public:
	FiniteAutomaton() = default;

	bool VerifyAutomaton();
	bool IsDeterministic();
	void CheckWord(State state, const std::string& word, size_t index);

	friend std::istream& operator>>(std::istream& in, FiniteAutomaton& automaton);
	friend std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton);

	const std::unordered_set<State>& GetStates() const;
	const std::unordered_set<Symbol>& GetSymbols() const;
	const TransitionTable& GetTransitionTable() const;
	const std::vector<State>& GetTransition(const State& state, Symbol symbol);
	const State& GetInitialState() const;
	const std::unordered_set<State>& GetFinalStates() const;

	void SetInitialState(const State& state);
	void InsertState(const State& state);
	void InsertSymbol(const Symbol& symbol);
	void InsertFinalState(const State& state);
	void InsertTransition(const std::pair<State, Symbol>& pair, const State& state);


private:

	std::unordered_set<State> m_states;
	std::unordered_set<Symbol> m_symbols;
	TransitionTable m_transitionTable;
	State m_initialState;
	std::unordered_set<State> m_finalStates;
	

};

