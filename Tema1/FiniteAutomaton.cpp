#include "FiniteAutomaton.h"

bool FiniteAutomaton::VerifyAutomaton()
{
	if (m_states.find(m_initialState) == m_states.end())
		return false;

	for (const auto& finalState : m_finalStates)
		if (m_states.find(finalState) == m_states.end())
			return false;

	for (const auto& transition : m_transitionTable)
	{
		if (m_states.find(transition.first.first) == m_states.end())
			return false;

		if (m_symbols.find(transition.first.second) == m_symbols.end())
			return false;

		for (const auto& state : transition.second)
			if (m_states.find(state) == m_states.end())
				return false;
	}

	return true;
}

bool FiniteAutomaton::IsDeterministic()
{
	for (const auto& transition : m_transitionTable)
	{
		if (transition.second.size() > 1)
			return false;
	}

	return true;
}

void FiniteAutomaton::CheckWord(State state, const std::string& word, size_t index)

{
	if (word.size() == index)
	{
		std::cout << "(" << state << ", *) -> ";
		if (m_finalStates.find(state) != m_finalStates.end())
		{
			throw "The word is accepted ! \n";
		}

		std::cout << "The word is not accepted ! \n";
		return;
	}
	else
	{
		std::cout << "(" << state << ", " << word.substr(index) << ") -> ";

		const auto& transition = GetTransition(state, word[index]);
		if (transition.empty())
		{
			std::cout << "The automaton has blocked...\n";
			return;
		}

		for (auto state : transition)
		{
			CheckWord(state, word, index + 1);
		}
	}
}

const std::unordered_set<State>& FiniteAutomaton::GetStates() const
{
	return m_states;
}

const std::unordered_set<Symbol>& FiniteAutomaton::GetSymbols() const
{
	return m_symbols;
}

const TransitionTable& FiniteAutomaton::GetTransitionTable() const
{
	return m_transitionTable;
}

const std::vector<State>& FiniteAutomaton::GetTransition(const State& state, Symbol symbol)
{
	return m_transitionTable[make_pair(state, symbol)];
}

const State& FiniteAutomaton::GetInitialState() const
{
	return m_initialState;
}

const std::unordered_set<State>& FiniteAutomaton::GetFinalStates() const
{
	return m_finalStates;
}

void FiniteAutomaton::SetInitialState(const State& state)
{
	m_initialState = state;
}

void FiniteAutomaton::InsertState(const State& state)
{
	m_states.insert(state);
}

void FiniteAutomaton::InsertSymbol(const Symbol& symbol)
{
	m_symbols.insert(symbol);
}

void FiniteAutomaton::InsertFinalState(const State& state)
{
	m_finalStates.insert(state);
}

void FiniteAutomaton::InsertTransition(const std::pair<State, Symbol>& pair, const State& state)
{
	m_transitionTable[pair].push_back(state);
}


std::istream& operator>>(std::istream& in, FiniteAutomaton& automaton)
{
	size_t numberOfStates;
	in >> numberOfStates;
	for (size_t index = 0; index < numberOfStates; ++index)
	{
		State state;
		in >> state;
		automaton.InsertState(state);
	}

	size_t numberOfTransitions;
	in >> numberOfTransitions;
	for (size_t index = 0; index < numberOfTransitions; ++index)
	{
		State state, state2;
		Symbol symbol;
		in >> state >> symbol >> state2;
		automaton.InsertTransition(make_pair(state, symbol), state2);
		automaton.InsertSymbol(symbol);
	}

	in >> automaton.m_initialState;

	size_t numberOfFinalStates;
	in >> numberOfFinalStates;
	for (size_t index = 0; index < numberOfFinalStates; ++index)
	{
		State state;
		in >> state;
		automaton.InsertFinalState(state);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton)
{
	out << "M = ({";
	for (const auto& state : automaton.GetStates())
		out << state << ", ";
	out << "\b\b}, {";
	for (const auto& symbol : automaton.GetSymbols())
		out << symbol << ", ";
	out << "\b\b}, d, " << automaton.GetInitialState() << ", {";
	for (const auto& finalState : automaton.GetFinalStates())
		out << finalState << ", ";
	out << "\b\b})\nd = {\n";
	for (const auto& transition : automaton.GetTransitionTable())
	{
		std::pair<State, Symbol> pair(transition.first.first, transition.first.second);
		out << "	(" << pair.first << ", " << pair.second << ") -> {";
		for (const auto& state : transition.second)
			out << state << ", ";
		out << "\b\b}\n";
	}
	out << "    }\n\n";

	return out;
}
