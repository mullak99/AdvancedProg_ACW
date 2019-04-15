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
			vector<Node*> nodes;

			{
				cout << "Loading Places..." << endl;

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

					cout << "Place: " << placeName << ", Ref: " << placesRef << ", Long: " << placesLong << ", Lat: " << placesLat << endl;
				}

				cout << "Finished Loading Places." << endl << endl;

				finPlaces.close();
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

				finPlaces.close();
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
