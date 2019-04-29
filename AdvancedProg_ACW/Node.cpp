#include "Node.h"
#include "Arc.h"

// Methods, etc.

Node::node::node(string name, int ref, float latitude, float longitude)
{
	this->Nodename = name;
	this->refnum = ref;
	this->lat = latitude;
	this->longitude = longitude;
}
