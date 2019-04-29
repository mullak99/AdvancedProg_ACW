#include "Node.h"
#include "Arc.h"

Arc::arc::arc(int linkRef1, int linkRef2, string transportMode)
{
	this->linkref1 = linkRef1;
	this->linkref2 = linkRef2;
	this->transportmode = transportMode;
}
