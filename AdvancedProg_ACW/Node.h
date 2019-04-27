// Node.h 
#pragma once
#include <string>
#include <vector>

using namespace std;

class Arc;

class Node
{
public: //this is where the functions go 
	struct node
	{
		string Nodename;
		int refnum;
		float lat;
		float longitude;
		struct node* next = NULL;
		vector<Arc*> m_arcs;
	};
	void insertAtEnd(node* head, string Nodename, int refnum, float lat, float longitude);
	//vector<Node::node*> nodes;
private:

};

