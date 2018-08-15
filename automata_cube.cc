#include <bits/stdc++.h>

using namespace std;

const int N = 4;
#define bs bitset<N>
const bs ZERO_BS(0);

class State {
public:
	State(bs a, bs b, bs br, bs c, bs cr): a{a}, b{b}, br{br}, c{c}, cr{cr} {}
	bs a;
	bs b;
	bs br;
	bs c;
	bs cr;
};


// Make it easy to print a state by doing cout << state
// We need the for loops because the standard operator<< for bitset puts the 
// LSD (the 0th index) on the right (like how we write binary numbers), but
// we want to read 0 to N-1 from left to right.
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
	os << "\t";

	for (int i = 0; i < N; ++i) {
		os << (state.c[i] ? '1' : '0');
	}
	os << "\t";

	for (int i = 0; i < N; ++i) {
		os << (state.cr[i] ? '1' : '0');
	}

	return os;
}

bool operator==(const State& s1, const State& s2) {
	return s1.a == s2.a and s1.b == s2.b and s1.br == s2.br and s1.c == s2.c and s1.cr == s2.cr;
}

namespace std {
	template<>
	struct hash<State> {
		size_t operator()(const State& obj) const {
			return hash<bs>()(obj.a) ^ hash<bs>()(obj.b) ^ hash<bs>()(obj.br) ^ hash<bs>()(obj.c) ^ hash<bs>()(obj.cr);
		}
	};
}

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Helper functions for automata transitions
void addEpsilonTrans(bs& a, bs& b, bs& br, bs& c, bs& cr) {
	if (a[N-1]) {
		b[0] = true;
		br[0] = true;
	}
	if (b[N-1] || br[N-1]) {
		c[0] = true;
		cr[0] = true;
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
	bs c_bitset_transition_a = bitset_transition_a(before.c);
	bs cr_bitset_transition_a = rev_bitset_transition_a(before.cr);
	addEpsilonTrans(a_bitset_transition_a, b_bitset_transition_a, br_bitset_transition_a, c_bitset_transition_a, cr_bitset_transition_a);
	return State(a_bitset_transition_a, b_bitset_transition_a, br_bitset_transition_a, c_bitset_transition_a, cr_bitset_transition_a);
}

State state_transition_b(const State& before) {
	bs a_bitset_transition_b = bitset_transition_b(before.a);
	bs b_bitset_transition_b = bitset_transition_b(before.b);
	bs br_bitset_transition_b = rev_bitset_transition_b(before.br);
	bs c_bitset_transition_b = bitset_transition_b(before.c);
	bs cr_bitset_transition_b = rev_bitset_transition_b(before.cr);
	addEpsilonTrans(a_bitset_transition_b, b_bitset_transition_b, br_bitset_transition_b, c_bitset_transition_b, cr_bitset_transition_b);
	return State(a_bitset_transition_b, b_bitset_transition_b, br_bitset_transition_b, c_bitset_transition_b, cr_bitset_transition_b);
}

State state_transition_c(const State& before) {
	bs a_bitset_transition_c = bitset_transition_c(before.a);
	bs b_bitset_transition_c = bitset_transition_c(before.b);
	bs br_bitset_transition_c = rev_bitset_transition_c(before.br);
	bs c_bitset_transition_c = bitset_transition_c(before.c);
	bs cr_bitset_transition_c = rev_bitset_transition_c(before.cr);
	addEpsilonTrans(a_bitset_transition_c, b_bitset_transition_c, br_bitset_transition_c, c_bitset_transition_c, cr_bitset_transition_c);
	return State(a_bitset_transition_c, b_bitset_transition_c, br_bitset_transition_c, c_bitset_transition_c, cr_bitset_transition_c);
}

State state_transition_d(const State& before) {
	bs a_bitset_transition_d = bitset_transition_d(before.a);
	bs b_bitset_transition_d = bitset_transition_d(before.b);
	bs br_bitset_transition_d = rev_bitset_transition_d(before.br);
	bs c_bitset_transition_d = bitset_transition_d(before.c);
	bs cr_bitset_transition_d = rev_bitset_transition_d(before.cr);
	addEpsilonTrans(a_bitset_transition_d, b_bitset_transition_d, br_bitset_transition_d, c_bitset_transition_d, cr_bitset_transition_d);
	return State(a_bitset_transition_d, b_bitset_transition_d, br_bitset_transition_d, c_bitset_transition_d, cr_bitset_transition_d);
}


bool isFinalState(const State& state) {
	return state.c[N-1] || state.cr[N-1];
}


// Perform a DFS to find all reachable states. Then, see which states are indistinguishable (and print them).
int main() {
	unordered_set<State> seen;

	stack<State> s;

	// 1 is only the first start
	State startState = State(bs(1), ZERO_BS, ZERO_BS, ZERO_BS, ZERO_BS);
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
			// The b transition is forbidden from the N-2 in the reversal NFAs
			if (!currState.br[N-2] && !currState.cr[N-2]) {
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
	// classes.
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
			int bGroup = (pair.first.br[N-2] || pair.first.cr[N-2]) ? -1 : stateToGroup[state_transition_b(pair.first)];
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


	// ====================== IGNORE ALL CODE BELOW HERE ======================

	// Case 1
	// for (int i = 0; i < N-1; ++i) {
	// 	bs a = ZERO_BS;
	// 	a[i] = true;
	// 	for (int j = 0; j < (1 << (N-2)); ++j) {
	// 		bs b((j << 1) + 1);
	// 		b[N-1] = true;
	// 		for (int k = 0; k < (1 << (N-2)); ++k) {
	// 			// br[N-1] is false since we don't want the final state
	// 			bs br((k << 1) + 1);
	// 			for (int l = 0; l < (1 << (N-1)); ++l) {
	// 				bs c((l << 1) + 1);
	// 				for (int m = 0; m < (1 << (N-1)); ++m) {
	// 					bs cr((m << 1) + 1);

	// 					State s(a,b,br,c,cr);
	// 					cout << s << endl;
	// 					assert(seen.erase(s) == 1);
	// 				}
	// 			}
	// 		}
	// 	}
	// }


	// for(int i = 0; i < N; ++i) {
	// 	bs a = ZERO_BS;
	// 	a[i] = true;
	// 	bs b = ZERO_BS;
	// 	b[i] = true;
	// 	bs c((1 << N) - 1);

	// 	bool caseHolds = true;
	// 	for (int j = 0; j < (1 << N); ++j) {
	// 		for (int k = 0; k < (1 << N); ++k) {
	// 			bs br(j);
	// 			bs cr(k);
	// 			if (seen.count(State(a,b,br,c,cr)) != 0) {
	// 				caseHolds = false;
	// 				// cout << a << " " << b << " " << br << " " << c << " " << cr << endl;
	// 			}
	// 		}
	// 	}
	// 	if (caseHolds) {
	// 		cout << "Works for " << i << endl;
	// 	}
	// 	else {
	// 		cout << "Did not work for " << i << endl;
	// 	}
	// }

	// if (!seen.empty()) {
	// 	cout << "SEEN NOT EMPTY. SIZE: " << seen.size() << endl;
	// 	cout << "Example element: " << *seen.begin() << endl;
	// }
	// assert(seen.empty());
}