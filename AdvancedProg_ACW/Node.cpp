#include "Node.h"

// Methods, etc.

Node::node::node(const string &name, const int ref, const float latitude, const float longitude)
{
	this->Nodename = name;
	this->refnum = ref;
	this->lat = latitude;
	this->longi = longitude;
}

Node::Node()
{ }
