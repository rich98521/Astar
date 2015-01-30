
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#define _USE_MATH_DEFINES

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "Graph.h"
using namespace std;


typedef GraphArc<pair<string,int>, int> GArc;
typedef GraphNode<pair<string,int>, int> Node;

void visit( Node * pNode ) {
	pNode->setColor(sf::Color(180,100,0));
}

int main(int argc, char *argv[]) {
   
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "A* PathFinding"); 
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");
    Graph<pair<string,int>, int> graph( 30 );
	ifstream myfile;
	myfile.open("nodes.txt");
	
	string name, x, y; 
	int i = 0;
	while ( myfile >> name >> x >> y ) {
		graph.addNode(pair<string,int>(name,0), i, atoi(x.c_str()), atoi(y.c_str()));
		i++;
	}
    myfile.close();
	myfile.open("arcs.txt");
	int from, to, weight; 
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
		graph.addArc(to, from, weight);
	}
    myfile.close();

	bool first = true;
	Node* selectedNode;
	std::vector<Node*> nodePath;
	
	//map<string, list<pair<string,int>>> allPaths;
	//for(int i = 0; i < graph.nodeCount() - 1; i++)
	//{
	//	for(int i2 = i + 1; i2 < graph.nodeCount(); i2++)
	//	{
	//		std::vector<Node*> nodePath;
	//		list<pair<string,int>> pairPath;
	//		graph.uniformCostSearch(graph.nodeArray()[i], graph.nodeArray()[i2], visit, nodePath);
	//		for(int i3 = 0; i3 < nodePath.size(); i3++)
	//		{
	//			pairPath.push_back(nodePath[i3]->data());
	//		}
	//		allPaths[graph.nodeArray()[i]->data().first + "-" + graph.nodeArray()[i2]->data().first] = pairPath;
	//	}
	//}

	//for(map<string, list<pair<string,int>>>::iterator it = allPaths.begin(); it != allPaths.end(); it++)
	//{
	//	string out = it->first + "	";
	//	list<pair<string, int>>::iterator it2 = it->second.begin();
	//	for(; it2 != it->second.end(); it2++)
	//	{
	//		out+= it2->first + "(" + to_string(it2->second) + ")" + ", ";
	//	}
	//	out = out.substr(0, out.length() - 2);
	//	cout << out << endl;
	//}

	while (window.isOpen()) 
    {
        sf::Event Event; 
        while (window.pollEvent(Event)) 
        {
            if (Event.type == sf::Event::Closed) 
                window.close(); 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P)) 
			{
				std::vector<Node*> nodePath;
				graph.uniformCostSearch(graph.nodeArray()[0], graph.nodeArray()[18], visit, nodePath);
			}
			if ((Event.type == sf::Event::MouseButtonPressed) && (Event.key.code == sf::Mouse::Left)) 
			{
				for(int i = 0; i < graph.nodeCount(); i++)
				{
					Node* node = graph.nodeArray()[i];
					if(Event.mouseButton.x-20>node->getPosition().x&&Event.mouseButton.x-20<node->getPosition().x+40&&Event.mouseButton.y-20>node->getPosition().y&&Event.mouseButton.y-20<node->getPosition().y+40)
						{
							if(first)
							{
								selectedNode = node;
								for(int i2 = 0; i2 < graph.nodeCount(); i2++)
								{
									graph.nodeArray()[i2]->setColor(sf::Color(100,100,100));
								}
								node->setColor(sf::Color(250,100,100));
								first = !first;
							}
							else
							{
								if(node != selectedNode)
								{
									selectedNode->setColor(sf::Color(100,100,100));
									graph.uniformCostSearch(selectedNode, node, visit, nodePath);
									for(int i2 = 0; i2 < nodePath.size(); i2++)
									{
										nodePath[i2]->setColor(sf::Color(50,150,50));
									}
									first = !first;
								}
							}
						}
				}
			}
		}
		window.clear();
		for(int i = 0; i < graph.nodeCount(); i++)
		{
			Node* node = graph.nodeArray()[i];
			list<GArc>::const_iterator iter = node->arcList().begin();
			for(int i2 = 0;i2<node->arcList().size();i2++)
			{
				sf::Vector2f difference = node->getPosition()-iter->node()->getPosition();
				sf::Vector2f midPoint = node->getPosition() - sf::Vector2f(difference.x/2,difference.y/2);
				sf::RectangleShape rs;
				rs.setPosition(sf::Vector2f(40,40) + midPoint);
				rs.setSize(sf::Vector2f(sqrt(pow(difference.x,2)+pow(difference.y,2)),10));
				rs.setOrigin(rs.getSize().x/2,rs.getSize().y/2);
				rs.setRotation(atan2(difference.y,difference.x)/M_PI*180);
				sf::Text weight(std::to_string(iter->weight()), font, 12);
				weight.setColor(sf::Color(180,180,180));
				weight.setStyle(sf::Text::Bold);
				weight.setPosition(sf::Vector2f(40-weight.getLocalBounds().width/2,33) + midPoint);
				sf::Color col(50,50,50);
				if(node->getCircle().getFillColor() == sf::Color(50,150,50) && node->getCircle().getFillColor() == iter->node()->getCircle().getFillColor())
					col = node->getCircle().getFillColor();
				rs.setFillColor(col);
				window.draw(rs);
				window.draw(weight);
				iter++;
			}
		}
		for(int i = 0; i < graph.nodeCount(); i++)
		{
			Node* node = graph.nodeArray()[i];
			
			sf::Text name(node->data().first, font, 16);
			sf::Text value(std::to_string(node->data().second<10000?node->data().second:0), font, 16);
			value.setColor(sf::Color(100,180,200));
			value.setStyle(sf::Text::Bold);
			value.setPosition(sf::Vector2f(40-value.getLocalBounds().width/2,34) + node->getPosition());
			name.setColor(sf::Color(100,180,200));
			name.setStyle(sf::Text::Bold);
			name.setPosition(sf::Vector2f(40-name.getLocalBounds().width/2,18) + node->getPosition());

			window.draw(node->getCircle());
			window.draw(name);
			window.draw(value);
		}

		window.display();
    } 
    return EXIT_SUCCESS; 
}
