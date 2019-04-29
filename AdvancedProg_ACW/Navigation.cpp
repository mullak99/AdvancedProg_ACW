#include "Navigation.h"
#include "ACW_Wrapper.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Node.h"
#include "Arc.h"
#include <iterator>
#include <string>
#include <numeric>

using namespace std;

Node n;
//Node::node* nodes = new Node::node;

vector<Node::node> nodes;

Arc a;
//Arc::arc* arcs = new Arc::arc;

const bool showloading = false;
const bool debug = false;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);
	string command;
	inString >> command;

	if (commandString == "MaxDist")
	{
		return Navigation::MaxDist();
	}
	else if (commandString == "MaxLink")
	{
		return Navigation::MaxLink();
	}
	else if (commandString == "FindDist")
	{
		return Navigation::FindDist("");
	}
	else if (commandString == "FindNeighbour")
	{
		return Navigation::FindNeighbour("");
	}
	else if (commandString == "Check")
	{
		return Navigation::Check("");
	}
	else if (commandString == "FindRoute")
	{
		return Navigation::FindRoute("");
	}
	else if (commandString == "FindShortestRoute")
	{
		return Navigation::FindShortestRoute("");
	}
	else return false;
}

double GetDistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool Navigation::MaxDist()
{
	Node::node* largestNode1 = NULL;
	Node::node* largestNode2 = NULL;
	double largestDist = 0;
	
	for (auto& element1 : nodes)
	{
		for (auto& element2 : nodes)
		{
			if (element1.refnum != element2.refnum)
			{
				double lati1, longd1, lati2, longd2;
				LLtoUTM(element1.lat, element1.longitude, lati1, longd1);
				LLtoUTM(element2.lat, element2.longitude, lati2, longd2);

				double dist = GetDistance(lati1, longd1, lati2, longd2);

				if (dist > largestDist)
				{
					largestDist = dist;
					largestNode1 = &element1;
					largestNode2 = &element2;
				}
			}
		}
		largestDist = largestDist / 1000;
	}
	if (largestDist > 0 && largestNode1 != NULL && largestNode2 != NULL)
	{
		cout << setprecision(3) << "MaxDist" << endl << largestNode1->Nodename << "," << largestNode2->Nodename << "," << largestDist << endl << endl;
		return true;
	}
	return false;
}

bool Navigation::MaxLink()
{
	return false;
}

bool Navigation::FindDist(const std::string& params)
{
	cout << "FindDist" << endl;
	cout << "Params passed: " << params << endl;
	return false;
}

bool Navigation::FindNeighbour(const std::string& params)
{
	cout << "FindNeighbour" << endl;
	cout << "Params passed: " << params << endl;
	return false;
}

bool Navigation::Check(const std::string& params)
{
	cout << "Check" << endl;
	cout << "Params passed: " << params << endl;
	return false;
}

bool Navigation::FindRoute(const std::string& params)
{
	cout << "FindRoute" << endl;
	cout << "Params passed: " << params << endl;
	return false;
}

bool Navigation::FindShortestRoute(const std::string& params)
{
	cout << "FindShortestRoute" << endl;
	cout << "Params passed: " << params << endl;
	return false;
}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)
{
	//node->next = nullptr;
	//arc->next = nullptr;

	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);
	if (finPlaces.fail() || finLinks.fail())
	{
		cout << "Load Fail!" << endl;
		return false;
	}
	else
	{
		try
		{
			//vector<Node*> nodes;

			{
				if (debug && showloading) cout << "Loading Places..." << endl;

				string line;
				while (getline(finPlaces, line))
				{
					string placeName;
					int placesRef;
					float placesLat, placesLong;

					istringstream s(line);
					string field;

					getline(s, field, ',');
					placeName = field;

					getline(s, field, ',');
					istringstream getRef(field);
					getRef >> placesRef;

					getline(s, field, ',');
					istringstream getLat(field);
					getLat >> placesLat;

					getline(s, field, ',');
					istringstream getLong(field);
					getLong >> placesLong;

					//n.insertAtEnd(nodes, placeName, placesRef, placesLat, placesLong);

					nodes.push_back(Node::node(placeName, placesRef, placesLat, placesLong));

					if (debug && showloading) cout << "Place: " << placeName << ", Ref: " << placesRef << ", Long: " << placesLong << ", Lat: " << placesLat << endl;
				}

				if (debug && showloading) cout << "Finished Loading Places." << endl << endl;

				finPlaces.close();
			}
			
			{
				if (debug && showloading) cout << "Loading Links..." << endl;

				string line;
				while (getline(finLinks, line))
				{
					string linkMode;
					int linkRef1, linkRef2;

					istringstream s(line);
					string field;

					getline(s, field, ',');
					istringstream getRef1(field);
					getRef1 >> linkRef1;

					getline(s, field, ',');
					istringstream getRef2(field);
					getRef2 >> linkRef2;

					getline(s, field, ',');
					linkMode = field;

					//a.insertAtEnd(arcs, linkRef1, linkRef2, linkMode);

					for (auto& element : nodes) {
						if (element.refnum == linkRef1)
						{
							element.m_arcs.push_back(Arc::arc(linkRef1, linkRef2, linkMode));
						}
					}

					if (debug && showloading) cout << "LinkMode: " << linkMode << ", Ref1: " << linkRef1 << ", Ref2: " << linkRef2 << endl;
				}

				if (debug && showloading) cout << "Finished Loading Links." << endl << endl;

				finLinks.close();
			}

			if (debug)
			{
				int i = 0;
				for (auto& element : nodes) {
					cout << "Node[" << i << "] Nodename: " << element.Nodename << ", Ref: " << element.refnum << ", Long: " << element.longitude << ", Lat: " << element.lat << endl;
					for (auto& arc : element.m_arcs) {
						cout << "	LinkMode: " << arc.transportmode << ", Ref1: " << arc.linkref1 << ", Ref2: " << arc.linkref2 << endl;
					}
					cout << endl;
					i++;
				}
			}
		}
		catch (exception)
		{
			return false;
		}

	}

	return true;
}

// Add your code here
