#include "Node.h"
#include "Arc.h"

void Arc::insertAtEnd(arc* head, int linkref1, int linkref2, string transportmode)
{
	// Creates a temp to hold the last node and set last's data and next		
	Arc::arc* last = new Arc::arc;


	last->linkref1 = linkref1;
	last->linkref2 = linkref2;
	last->transportmode = transportmode;

	last->next = NULL;

	// If the linked list is empty then set head = last
	if (head == NULL) {
		head = last;
	}
	else
	{
		// Creates a temp node and sets it to head
		Arc::arc* temp = new Arc::arc;
		temp = head;
		// Uses temp to find the last node
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		// Appends the last node with last
		temp->next = last;
	}
	// Returns head of linked list
	//return head;
}