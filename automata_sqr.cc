#include <bits/stdc++.h>

using namespace std;

const int N = 5;
#define bs bitset<N>
const bs ZERO_BS(0);

class State {
public:
	State(bs a, bs b, bs br): a{a}, b{b}, br{br} {}
	bs a;
	bs b;
	bs br;
};

ostream& operator<<(ostream& os, const State& state) {
	for (int i = 0; i < N; ++i) {
		os << (state.a[i] ? '1' : '0');
	}
	os << "\t";

	for (int i = 0; i < N; ++i) {
		os << (state.b[i] ? '1' : '0');
	}
	os << "\t";

	for (int i = 0; i < N; ++i) {
		os << (state.br[i] ? '1' : '0');
	}
	return os;
}

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
		br[0] = true;
	}
}

bs bitset_transition_a(const bs& before) {
	bs after(0);
	for (int i = 0; i < N; ++i) {
		if (before[i]) {
			after[(i + 1) % N] = true;
		}
	}
	return after;
}

bs rev_bitset_transition_a(const bs& before) {
	bs after = before;
	return after;
}

bs bitset_transition_b(const bs& before) {
	bs after = before;
	bool temp = after[N-2];
	after[N-2] = after[N-1];
	after[N-1] = temp;
	return after;
}

bs rev_bitset_transition_b(const bs& before) {
	bs after = before;
	if (after[N-2])
		throw "invalid input";
	if (after[N-1]) {
		after[N-2] = true;
	}
	return after;
}

bs bitset_transition_c(const bs& before) {
	bs after = before;
	if (after[1]) {
		after[0] = true;
		after[1] = false;
	}
	return after;
}

bs rev_bitset_transition_c(const bs& before) {
	bs after = before;
	bool temp = after[0];
	after[0] = after[1];
	after[1] = temp;
	return after;
}

bs bitset_transition_d(const bs& before) {
	bs after = before;
	return after;
}

bs rev_bitset_transition_d(const bs& before) {
	return bitset_transition_a(before);
}

// ----------------------------------------------------------------------------

State state_transition_a(const State& before) {
	bs a_bitset_transition_a = bitset_transition_a(before.a);
	bs b_bitset_transition_a = bitset_transition_a(before.b);
	bs br_bitset_transition_a = rev_bitset_transition_a(before.br);
	addEpsilonTrans(a_bitset_transition_a, b_bitset_transition_a, br_bitset_transition_a);
	return State(a_bitset_transition_a, b_bitset_transition_a, br_bitset_transition_a);
}

State state_transition_b(const State& before) {
	bs a_bitset_transition_b = bitset_transition_b(before.a);
	bs b_bitset_transition_b = bitset_transition_b(before.b);
	bs br_bitset_transition_b = rev_bitset_transition_b(before.br);
	addEpsilonTrans(a_bitset_transition_b, b_bitset_transition_b, br_bitset_transition_b);
	return State(a_bitset_transition_b, b_bitset_transition_b, br_bitset_transition_b);
}

State state_transition_c(const State& before) {
	bs a_bitset_transition_c = bitset_transition_c(before.a);
	bs b_bitset_transition_c = bitset_transition_c(before.b);
	bs br_bitset_transition_c = rev_bitset_transition_c(before.br);
	addEpsilonTrans(a_bitset_transition_c, b_bitset_transition_c, br_bitset_transition_c);
	return State(a_bitset_transition_c, b_bitset_transition_c, br_bitset_transition_c);
}

State state_transition_d(const State& before) {
	bs a_bitset_transition_d = bitset_transition_d(before.a);
	bs b_bitset_transition_d = bitset_transition_d(before.b);
	bs br_bitset_transition_d = rev_bitset_transition_d(before.br);
	addEpsilonTrans(a_bitset_transition_d, b_bitset_transition_d, br_bitset_transition_d);
	return State(a_bitset_transition_d, b_bitset_transition_d, br_bitset_transition_d);
}


bool isFinalState(const State& state) {
	return state.b[N-1] || state.br[N-1];
}


// Perform a DFS to find all reachable states. Then, see which states are indistinguishable
// (and print them).
int main() {
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

		// cout << currState << endl;

		{
			State aState = state_transition_a(currState);
			auto f = seen.find(aState);
			if (f == seen.end()) {
				seen.insert(aState);
				s.push(aState);
			}
		}

		{
			if (!currState.br[N-2]) {
				State bState = state_transition_b(currState);
				auto f = seen.find(bState);
				if (f == seen.end()) {
					seen.insert(bState);
					s.push(bState);
				}
			}
		}

		{
			State cState = state_transition_c(currState);
			auto f = seen.find(cState);
			if (f == seen.end()) {
				seen.insert(cState);
				s.push(cState);
			}
		}

		{
			State dState = state_transition_d(currState);
			auto f = seen.find(dState);
			if (f == seen.end()) {
				seen.insert(dState);
				s.push(dState);
			}
		}
	}

	cout << "Number of reachable states: " << seen.size() << endl;

	// ------------------------------------------------------------------------
	// Find all indistinguishable states by repeatedly partitioning the states into equivalence 
	// groups.
	// numIndistStates is actually the number of indistinguishable states - 1 (it is the largest 
	// group number).
	int numIndistStates = 1;
	unordered_map<State,int> stateToGroup;
	for (const State& state: seen) {
		stateToGroup[state] = isFinalState(state) ? 0 : 1;
	}

	// In the first iteration we have all 0-equivalent states, then all 1-equivalent states, etc.
	while (true) {
		// If two states are in the same vector, they are in the same group and transition to
		// states in the same groups (i.e. if s is the state the 4-tuple says which group of
		// states delta(s,a), delta(s,b), delta(s,c) and delta(s,d) are in).
		map<int, map<tuple<int,int,int,int>, vector<State>>> groupAndTransitionsToStates;
		for (auto& pair : stateToGroup) {
			int aGroup = stateToGroup[state_transition_a(pair.first)];
			// The -1 represents a failure state (reject the input), since this transition is 
			// not defined
			int bGroup = pair.first.br[N-2] ? -1 : stateToGroup[state_transition_b(pair.first)];
			int cGroup = stateToGroup[state_transition_c(pair.first)];
			int dGroup = stateToGroup[state_transition_d(pair.first)];
			tuple<int,int,int,int> transitions(aGroup, bGroup, cGroup, dGroup);

			// Add the state (note that if the group or transition hasn't been initialized yet then
			// it will automatically be default constructed)
			groupAndTransitionsToStates[pair.second][transitions].push_back(pair.first);
		}

		// If we ever have two different transitions for the same group number, the states in those
		// transitions are not equivalent and can be split into different groups (so we update the
		// stateToGroup map).
		int newNumIndistStates = 0;
		for (auto& mapping : groupAndTransitionsToStates) {
			for (auto& pair : mapping.second) {
				// go through all states with this group and transition and assign them to their
				// new group
				for (State& state : pair.second) {
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
			break;
		}

		numIndistStates = newNumIndistStates;
	}


	// ------------------------------------------------------------------------
	// Verify that all the states we expect to reach (and only those) are present
	// States of the form {q, empty, empty}
	for (int i = 0; i < N-1; ++i) {
		bs a = ZERO_BS;
		a[i] = true;
		assert(seen.erase(State(a, ZERO_BS, ZERO_BS)));
	}

	// states of the form {q (not final), P not empty, R not empty}
	for (int i = 0; i < N-1; ++i) {
		bs a = ZERO_BS;
		a[i] = true;
		for (int j = 1; j < (1 << N); ++j) {
			bs b(j);
			for (int k = 1; k < (1 << N); ++k) {
				State s(a,b,bs(k));
				// cout << "Erasing " << s << endl;

				assert(seen.erase(s));
			}
		}
	}

	// states of the form {q final, P contains start, R contains start}
	{
		bs a = ZERO_BS;
		a[N-1] = true;
		for (int j = 0; j < (1 << (N-1)); ++j) {
			bs b((j << 1) + 1); 
			for (int k = 0; k < (1 << (N-1)); ++k) {
				assert(seen.erase(State(a, b, bs((k << 1) + 1))));
			}
		}
	}

	if (!seen.empty()) {
		cout << "SEEN NOT EMPTY. SIZE: " << seen.size() << endl;
		cout << "Example element: " << *seen.begin() << endl;
	}
	assert(seen.empty());
}