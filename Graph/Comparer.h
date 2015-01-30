#ifndef COMPARER_H
#define COMPARER_H

template<class NodeType, class ArcType>
class Comparer {
private:
	typedef GraphNode<NodeType, ArcType> Node;
public:
	bool operator()(Node* n1, Node* n2) {
		return n1->data().second > n2->data().second;
	}
};

#include "GraphNode.h"

#endif