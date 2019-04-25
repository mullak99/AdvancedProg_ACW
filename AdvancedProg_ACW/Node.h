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
		struct node* next;
		vector<Arc*> m_arcs;
		
	};
	void insertAtEnd(node* head, string Nodename, int refnum, float lat, float longitude)
	{
		// Creates a temp to hold the last node and set last's data and next
		Node::node* last = new Node::node;

		last->Nodename = Nodename;
		last->refnum = refnum;
		last->lat = lat;
		last->longitude = longitude;
		last->next = NULL;

		// If the linked list is empty then set head = last
		if (head == NULL) {
			head = last;
		}
		else {
			// Creates a temp node and sets it to head
			Node::node* temp = new Node::node;

			temp = head;

			// Uses temp to find the last node
			while (temp->next != NULL) {
				temp = temp->next;
			}

			// Appends the last node with last
			temp->next = last;
		}

		// Returns head of linked list
		//return head;
	}
	//vector<Node::node*> nodes;
private:

};

