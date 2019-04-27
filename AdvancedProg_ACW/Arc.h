// Arc.h 
#pragma once 
#include <string>
using namespace std;

class Node;

class Arc
{
public:
	struct arc
	{
		int linkref1;
		int linkref2;
		string transportmode;
		struct arc* next = NULL;
	};
	void insertAtEnd(arc* head, int linkref1, int linkref2, string transportmode);

private: Node* m_destination; //node pointer of destination variable 
		 string mode;
		 // Methods and other stuff 
};
