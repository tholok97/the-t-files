#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct State;
bool legalState(const State &s);
bool isGoal(const State &s);

class State {
friend bool legalState(const State &s);
friend bool isGoal(const State &s);
private: 
	vector<pair<int, int>> pairs;
	int elevator;

public: 
	static const int BOTTOM_FLOOR;
	static const int TOP_FLOOR;
	State(const vector<pair<int, int>> &ps, int e): pairs(ps), elevator(e) { }

	string getString() const {
		string str;
		auto sortedPairs = pairs;
		sort(sortedPairs.begin(), sortedPairs.end());
		for (const auto &p : sortedPairs) {
			str.append(to_string(p.first));
			str.append(to_string(p.second));
		}
		str.append(to_string(elevator));
		return str;
	}

	State applyMove(bool up, const pair<int, bool> &a, 
			const pair<int, bool> &b) const {
		State s(*this);
		int diff = (up ? 1 : -1);

		s.elevator += diff;

		if (a.second)
			s.pairs[a.first].first += diff;
		else
			s.pairs[a.first].second += diff;

		if (b.second)
			s.pairs[b.first].first += diff;
		else
			s.pairs[b.first].second += diff;

		return s;
	}

	State applyMove(bool up, const pair<int, bool> &a) const {
		State s(*this);
		int diff = (up ? 1 : -1);

		s.elevator += diff;

		if (a.second)
			s.pairs[a.first].first += diff;
		else
			s.pairs[a.first].second += diff;

		return s;
	}

	vector<State> getNeighboors() const {
		vector<State> neighboors;

		vector<pair<int, bool>> moveables;
		for (size_t i = 0; i < pairs.size(); ++i) {
			auto p = pairs[i];
			if (p.first == elevator) {
				moveables.push_back(make_pair(i, true));
			}
			if (p.second == elevator) {
				moveables.push_back(make_pair(i, false));
			}
		}

		for (size_t i = 0; i < moveables.size(); ++i) {
			if (elevator < TOP_FLOOR) {
				State potNeigh(applyMove(true, moveables[i]));
				if (legalState(potNeigh))
					neighboors.push_back(potNeigh);
			} if (elevator > BOTTOM_FLOOR) {
				State potNeigh(applyMove(false, moveables[i]));
				if (legalState(potNeigh))
					neighboors.push_back(potNeigh);
			}

			for (size_t j = i+1; j < moveables.size(); ++j) {
				if (elevator < TOP_FLOOR) {
					State potNeigh(applyMove(true, moveables[i], 
							moveables[j]));
					if (legalState(potNeigh))
						neighboors.push_back(potNeigh);
				} if (elevator > BOTTOM_FLOOR) {
					State potNeigh(applyMove(false, moveables[i], 
							moveables[j]));
					if (legalState(potNeigh))
						neighboors.push_back(potNeigh);
				}
			}
		}

		return neighboors;
	}

	void print() const {
		for (size_t i = 0; i < pairs.size(); ++i) {
			cout << i << ": (" << pairs[i].first << ", " << pairs[i].second <<
					")\n";
		}
		cout << "E: " << elevator << "\n\n";
	}
};

const int State::BOTTOM_FLOOR = 0;
const int State::TOP_FLOOR = 3;

bool legalState(const State &s) {
	for (const auto &p : s.pairs)
		if (p.first != p.second)
			for (const auto &p2 : s.pairs)
				if (p.second == p2.first)
					return false;
	return true;
}

bool isGoal(const State &s) {
	for (auto p : s.pairs)
		if (p.first != State::TOP_FLOOR || p.second != State::TOP_FLOOR)
			return false;
	return true;
}

int search(const State &start) {
	queue<State> q;
	unordered_map<string, int> dist;

	q.push(start);
	dist[start.getString()] = 0;

	State current(start);
	while (!q.empty() && !isGoal(current)) {
		current = q.front();
		q.pop();

		for (const State &n : current.getNeighboors()) {
			string stateStr = n.getString();
			if (dist.find(stateStr) == dist.end()) {
				q.push(n);
				dist.insert(make_pair(stateStr, 
						dist.at(current.getString())+1));	
			}
		}
	}

	return dist.at(current.getString());
}

int main() {
	State initial({ {0,0}, {1,2}, {1,2}, {1,2}, {1,2} }, 0);
	cout << "Answer to part 1: " << search(initial) << endl;
	
	State initial2({ {0,0}, {0,0}, {0,0}, {1,2}, {1,2}, {1,2}, {1,2} }, 0);
	cout << "Answer to part 2: " << search(initial2) << endl;

	return 0;
}
