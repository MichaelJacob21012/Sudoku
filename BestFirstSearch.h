#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
using std::unique_ptr;

#include <queue>
using std::priority_queue;

#include <vector>
using std::vector;

bool heuristicComparison(const unique_ptr<Searchable> & a, const unique_ptr<Searchable> & b ){
	if (a->heuristicValue() > b->heuristicValue()){
		return true;
	}
	return false;
}

class BestFirstSearch {

protected:

	/// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue
	int nodes = 0;

	priority_queue<unique_ptr<Searchable>, vector<unique_ptr<Searchable>>, decltype(&heuristicComparison)> Q;
public:

	BestFirstSearch(std::unique_ptr<Searchable> && startFrom)
: Q(heuristicComparison){
		// TODO Put startFrom onto the queue:
		Q.push(std::move(startFrom));
	}

	int getNodesExpanded() const {
		return nodes;
	}

	Searchable * solve() {

		// TODO Implement search, returning a pointer to the solution (if one is found)
		while (!Q.empty()) {
			if (Q.top()->isSolution()) {
				return Q.top().get();
			}
			++nodes;
			vector<unique_ptr<Searchable>> successors = Q.top()->successors();
			Q.pop();
			for (auto & successor : successors) {
				Q.push(std::move(successor));
			}
		}
		return nullptr;
	}
};


// Do not edit below this line

#endif


