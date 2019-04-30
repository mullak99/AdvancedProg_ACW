#include "Node.h"
#include "Arc.h"

Arc::arc::arc(const int linkRef1, const int linkRef2, const string &transportMode)
{
	this->linkref1 = linkRef1;
	this->linkref2 = linkRef2;
	this->transportmode = transportMode;
}

Arc::Arc()
{ }
