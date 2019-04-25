#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Node.h"

using namespace std;

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

	// Add your code here

	return false;
}

bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)
{
	Node n;
	Node::node* node = new Node::node;//Struct for the node to hold the data from the csv file Places

	node->next = nullptr;

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
				cout << "Loading Places..." << endl;

				string line;
				while (getline(finPlaces, line))
				{
					//Node::node* temp = new Node::node;

					string placeName;
					int placesRef;
					float placesLat, placesLong;

					istringstream s(line);
					string field;

					getline(s, field, ',');
					placeName = field;
					//temp->Nodename = placeName;

					getline(s, field, ',');
					istringstream getRef(field);
					getRef >> placesRef;
					//temp->refnum = placesRef;

					getline(s, field, ',');
					istringstream getLat(field);
					getLat >> placesLat;
					//temp->lat = placesLat;

					getline(s, field, ',');
					istringstream getLong(field);
					getLong >> placesLong;
					//temp->longitude = placesLong;

					n.insertAtEnd(node, placeName, placesRef, placesLat, placesLong);

					cout << "Place: " << placeName << ", Ref: " << placesRef << ", Long: " << placesLong << ", Lat: " << placesLat << endl;
				}
				//temp->next = NULL;

				cout << "Finished Loading Places." << endl << endl;

				finPlaces.close();

				cout << "Node stuff." << endl;

				{
					int i = 0;
					for (; node; node = node->next)
					{
						cout << "Node[" << i << "] Nodename: " << node->Nodename << ", Ref: " << node->refnum << ", Long: " << node->longitude << ", Lat: " << node->lat << endl;
						i++;
					}
				}

				cout << "Finished Node stuff." << endl << endl;
			}

			{
				cout << "Loading Links..." << endl;

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

					cout << "LinkMode: " << linkMode << ", Ref1: " << linkRef1 << ", Ref2: " << linkRef2 << endl;
				}

				cout << "Finished Loading Links." << endl << endl;

				finLinks.close();
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
