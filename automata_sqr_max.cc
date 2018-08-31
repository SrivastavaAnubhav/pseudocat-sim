#include <bits/stdc++.h>

using namespace std;

const long long N = 5;
#define bs bitset<N>
const bs ZERO_BS(0);

class State {
public:
	State(bs a, bs b, bs br): a{a}, b{b}, br{br} {}
	bs a;
	bs b;
	bs br;
};

// Make it easy to print a state by doing cout << state
// We need the for loops because the standard operator<< for bitset puts the 
// LSD (the 0th index) on the right (like how we write binary numbers), but
// we want to read 0 to N-1 from left to right.
ostream& operator<<(ostream& os, const State& state) {
	for (long long i = 0; i < N; ++i) {
		os << (state.a[i] ? '1' : '0');
	}
	os << "\t";

	for (long long i = 0; i < N; ++i) {
		os << (state.b[i] ? '1' : '0');
	}
	os << "\t";

	for (long long i = N-1; i >= 0; --i) {
		os << (state.br[i] ? '1' : '0');
	}
	return os;
}


// The following two methods are needed to use unordered_map
bool operator==(const State& s1, const State& s2) {
	return s1.a == s2.a and s1.b == s2.b and s1.br == s2.br;
}

namespace std {
	template<>
	struct hash<State> {
		size_t operator()(const State& obj) const {
			return hash<bs>()(obj.a) ^ hash<bs>()(obj.b) ^ hash<bs>()(obj.br);
		}
	};
}

// ------------------------------------------------------------------------------------------------
// Helper functions for automata transitions
void addEpsilonTrans(bs& a, bs& b, bs& br) {
	if (a[N-1]) {
		b[0] = true;
		// WARNING: This changed from br[0] to br[N-1] on Aug 30 2018
		br[N-1] = true;
	}
}

bs bitset_transition(const bs& before, const vector<long long>& character) {
	bs after(0);
	for (long long i = 0; i < N; ++i) {
		if (before[i]) {
			after[character[i]] = true;
		}
	}
	return after;
}

bs rev_bitset_transition(const bs& before, const vector<long long>& character) {
	bs after(0);
	for (long long i = 0; i < N; ++i) {
		bool didTransition = false;
		if (before[i]) {
			// The indices which go to i are the destinations in the reverse
			for (long long j = 0; j < N; ++j) {
				if (character[j] == i) {
					didTransition = true;
					after[j] = 1;
				}
			}

			if (!didTransition) {
				throw runtime_error("Cannot transition the given state on the given character.");
			}
		}
	}
	return after;	
}

bool isValidTransition(const bs& before, const vector<long long>& character) {
	for (long long i = 0; i < N; ++i) {
		bool didTransition = false;
		if (before[i]) {
			for (long long j = 0; j < N; ++j) {
				if (character[j] == i) {
					didTransition = true;
					break;
				}
			}
			if (!didTransition) return false;
		}		
	}
	return true;
}

// ----------------------------------------------------------------------------

State state_transition(const State& before, const vector<long long>& character) {
	bs a_bitset_transition = bitset_transition(before.a, character);
	bs b_bitset_transition = bitset_transition(before.b, character);
	bs br_bitset_transition = rev_bitset_transition(before.br, character);
	addEpsilonTrans(a_bitset_transition, b_bitset_transition, br_bitset_transition);
	return State(a_bitset_transition, b_bitset_transition, br_bitset_transition);
}

bool isFinalState(const State& state) {
	// WARNING: This changed from br[N-1] to br[0] on Aug 30 2018
	return state.b[N-1] || state.br[0];
}

// Perform a DFS to find all reachable states. Then, see which states are indistinguishable
// (and print them).
int main() {
	// Generate the alphabet (should be 0000, 0001, ..., 3333 for N = 4)
	vector<long long> powersOfN;
	powersOfN.push_back(1);

	for (long long i = 0; i < N-1; ++i) {
		powersOfN.push_back(powersOfN.back() * N);
	}

	long long nToTheN = powersOfN.back() * N;
	vector<vector<long long>> alphabet;
	alphabet.reserve(nToTheN);
	for (long long i = 0; i < nToTheN; ++i) {
		// Stores the base-N representation of i
		vector<long long> character(N,0);

		long long num = i;
		for (long long j = 0; j < N; ++j) {
			character[j] = num/powersOfN[N-j-1];
			num %= powersOfN[N-j-1];
		}
		alphabet.push_back(move(character));
	}

	// ----------------------------------------------------------------------------------

	// Find reachable states
	unordered_set<State> seen;
	stack<State> s;
	// bs(1) is 0...01, which represents "only the starting state in the first automaton"
	State startState = State(bs(1), ZERO_BS, ZERO_BS);

	// Only the start state is reachable to start off
	s.push(startState);
	seen.insert(startState);

	while (!s.empty()) {
		State currState = s.top();
		s.pop();

		// const reference since the alphabet should never be changing
		for (const vector<long long>& character: alphabet) {
			if (isValidTransition(currState.br, character)) {
				State stateAfterTransition = state_transition(currState, character);
				auto f = seen.find(stateAfterTransition);
				if (f == seen.end()) {
					seen.insert(stateAfterTransition);
					s.push(stateAfterTransition);
				}
			}
		}
	}

	cout << "Number of reachable states: " << seen.size() << endl;

	// ------------------------------------------------------------------------
	// Find all indistinguishable states by repeatedly partitioning the states into equivalence 
	// groups.
	long long numIndistStates = 1;
	unordered_map<State,long long> stateToGroup;
	for (const State& state: seen) {
		stateToGroup[state] = isFinalState(state) ? 0 : 1;
	}

	// In the first iteration we have all 0-equivalent states, then all 1-equivalent states, etc.
	while (true) {
		// If two states are in the same state vector, they are in the same group and transition to
		// states in the same groups.
		map<long long, map<vector<long long>, vector<State>>> groupAndTransitionsToStates;

		// pair is a pair of state and group
		for (auto& pair : stateToGroup) {
			vector<long long> transitionStateGroups;
			for (const vector<long long>& character : alphabet) {
				if (isValidTransition(pair.first.br, character)) {
					transitionStateGroups.push_back(stateToGroup[state_transition(pair.first, character)]);
				}
				else {
					transitionStateGroups.push_back(-1);
				}
			}

			// Add the state (note that if the group or transition hasn't been initialized yet then
			// it will automatically be default constructed)
			groupAndTransitionsToStates[pair.second][transitionStateGroups].push_back(pair.first);
		}

		// If we ever have two different transitions for the same group number, the states in those
		// transitions are not equivalent and can be split into different groups (so we update the
		// stateToGroup map).
		long long newNumIndistStates = 0;
		for (auto& mapping : groupAndTransitionsToStates) {
			for (auto& transitionsAndStates : mapping.second) {
				// go through all states with this group and transition and assign them to their
				// new group
				for (State& state : transitionsAndStates.second) {
					stateToGroup[state] = newNumIndistStates;
				}
				++newNumIndistStates;
			}
		}

		// Didn't split any groups, we are done
		if (newNumIndistStates == numIndistStates) {
			for (auto& mapping : groupAndTransitionsToStates) {
				for (auto& pair : mapping.second) {
					if (pair.second.size() != 1) {
						cout << "The following states are indistinguishable: " << endl;
						for (State& state : pair.second) {
							cout << state << endl;
						}
					}
				}
			}

			cout << "Number of reachable and indistinguishable states: " << newNumIndistStates << endl;
			return 0;
		}

		numIndistStates = newNumIndistStates;
	}
}