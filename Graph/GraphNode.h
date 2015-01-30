#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
private:    
// typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;
// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
    NodeType m_data;

// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<Arc> m_arcList;
	int mX, mY;
// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;
	sf::CircleShape m_cs;

	GraphNode* m_lastNode;
	string m_name;
	int m_estimate;
public:
    // Accessor functions
    list<Arc> const & arcList() const {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    NodeType const & data() const {
        return m_data;
    }
    // Manipulator functions
    void setData(NodeType data) {
        m_data = data;
    }
	
    void setEstimate(int estimate) { m_estimate = estimate; };
    int getEstimate() { return m_estimate; };
    
    void setMarked(bool mark) {
        m_marked = mark;
    }
           
    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );
	
	GraphNode* getLast() const { return m_lastNode; }

	void setLast(GraphNode* node) { m_lastNode = node; }

	GraphNode():m_lastNode(0){ m_cs.setRadius(20); m_cs.setFillColor(sf::Color(100,100,100)); }
	GraphNode(GraphNode* last)
	{
		m_lastNode = last;
	}
	void setPosition(int x, int y) { mX = x; mY = y; m_cs.setPosition(sf::Vector2f(20,20) + sf::Vector2f(x, y)); };
	void setColor(sf::Color col) { m_cs.setFillColor(col); };
	sf::Vector2f getPosition() { return sf::Vector2f(mX,mY); };
	sf::CircleShape getCircle() { return m_cs; };
};

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* pNode ) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     // returns null if not found
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* pNode, ArcType weight ) {
   // Create a new arc.
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);   
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* pNode ) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

#include "GraphArc.h"

#endif
