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
#include <list> 
#include <array>

using namespace std;

vector<Node::node> nodes;
int v = 0;

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

void add_edge(vector<int> adj[], int src, int dest)
{
	adj[src].push_back(dest);
	adj[dest].push_back(src);
}


bool BFS(vector<int> adj[], int src, int dest, int v, int pred[], int dist[])
{
	list<int> queue;

	bool* visited = new bool[v];


	for (int i = 0; i < v; i++) {
		visited[i] = false;
		dist[i] = INT_MAX;
		pred[i] = -1;
	}
	visited[src] = true;
	dist[src] = 0;
	queue.push_back(src);

	while (!queue.empty()) {
		int u = queue.front();
		queue.pop_front();
		for (int i = 0; i < adj[u].size(); i++) {
			if (visited[adj[u][i]] == false) {
				visited[adj[u][i]] = true;
				dist[adj[u][i]] = dist[u] + 1;
				pred[adj[u][i]] = u;
				queue.push_back(adj[u][i]);

				if (adj[u][i] == dest)
					return true;
			}
		}
	}

	return false;
}

vector<int> getShortestDistance(vector<int> adj[], int s, int dest, int v, double &distance)
{
	int* pred = new int[v];
	int* dist = new int[v];
	vector<int> path;

	if (BFS(adj, s, dest, v, pred, dist) == false)
	{
		return path;
	}
	
	int crawl = dest;
	path.push_back(crawl);
	while (pred[crawl] != -1)
	{
		path.push_back(pred[crawl]);
		crawl = pred[crawl];
	}

	distance = dist[dest];

	return path;
}


const bool Navigation::ProcessCommand(const string& commandString) const
{
	istringstream inString(commandString);
	string command;
	inString >> command;


	if (command == "MaxDist")
	{
		return Navigation::MaxDist();
	}
	else if (command == "MaxLink")
	{
		return Navigation::MaxLink();
	}
	else if (command == "FindDist")
	{
		return Navigation::FindDist(commandString);
	}
	else if (command == "FindNeighbour")
	{
		return Navigation::FindNeighbour(commandString);
	}
	else if (command == "Check")
	{
		return Navigation::Check(commandString);
	}
	else if (command == "FindRoute")
	{
		return Navigation::FindRoute(commandString);
	}
	else if (command == "FindShortestRoute")
	{
		return Navigation::FindShortestRoute(commandString);
	}
	else return false;
}

double GetDistance(const double x1, const double y1, const double x2, const double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) / 1000;
}

const bool Navigation::MaxDist() const
{
	Node::node* largestNode1 = nullptr;
	Node::node* largestNode2 = nullptr;
	double largestDist = 0;
	
	for (auto& element1 : nodes)
	{
		for (auto& element2 : nodes)
		{
			if (element1.refnum != element2.refnum)
			{
				double lati1, longi1, lati2, longi2;

				LLtoUTM(element1.lat, element1.longi, lati1, longi1);
				LLtoUTM(element2.lat, element2.longi, lati2, longi2);

				const double dist = GetDistance(lati1, longi1, lati2, longi2);

				if (dist > largestDist)
				{
					largestDist = dist;
					largestNode1 = &element1;
					largestNode2 = &element2;
				}
			}
		}
	}
	if (largestDist > 0 && largestNode1 != nullptr && largestNode2 != nullptr)
	{
		cout << setprecision(3) << "MaxDist" << endl << largestNode1->Nodename << "," << largestNode2->Nodename << "," << largestDist << endl << endl;
		return true;
	}
	return false;
}

const bool Navigation::MaxLink() const
{
	Node::node* linkStartNode = nullptr;
	Node::node* linkEndNode = nullptr;
	Arc::arc* largestArc = nullptr;
	double largestDist = 0;

	for (auto& element : nodes)
	{
		linkStartNode = &element;

		for (auto& arc : element.m_arcs)
		{
			for (auto& nodeatlink : nodes)
			{
				if (nodeatlink.refnum == arc.linkref2)
					linkEndNode = &nodeatlink;
			}
			if (linkEndNode != nullptr)
			{
				double lati1, longi1, lati2, longi2;

				LLtoUTM(linkStartNode->lat, linkStartNode->longi, lati1, longi1);
				LLtoUTM(linkEndNode->lat, linkEndNode->longi, lati2, longi2);

				const double dist = GetDistance(lati1, longi1, lati2, longi2);

				if (dist > largestDist)
				{
					largestDist = dist;
					largestArc = &arc;
				}
			}
		}
	}
	if (largestDist > 0 && largestArc != nullptr)
	{
		cout << setprecision(3) << "MaxLink" << endl << largestArc->linkref1 << "," << largestArc->linkref2 << "," << largestDist << endl << endl;
		return true;
	}
	return false;
}

const bool Navigation::FindDist(const std::string& params) const
{
	istringstream inString(params);
	string command;
	int linkRef1, linkRef2;
	inString >> command;
	inString >> linkRef1;
	inString >> linkRef2;

	

	return false;
}

const bool Navigation::FindNeighbour(const std::string& params) const
{
	istringstream inString(params);
	string command;
	int linkRef1;
	inString >> command;
	inString >> linkRef1;

	cout << "FindNeighbour " << linkRef1 << endl;

	bool done = false;
	for (auto& element : nodes)
	{
		if (!done)
		{
			if (element.refnum == linkRef1)
			{
				for (auto& arc : element.m_arcs)
				{
					done = true;
					cout << arc.linkref2 << endl;
				}
			}
		}
		else break;
	}
	cout << endl;

	return done;
}

const bool Navigation::Check(const std::string& params) const
{
	istringstream inString(params);
	string command;
	inString >> command;

	return false;
}

const bool Navigation::FindRoute(const std::string& params) const
{
	istringstream inString(params);
	string command, mode;
	int linkRef1, linkRef2;
	inString >> command;
	inString >> mode;
	inString >> linkRef1;
	inString >> linkRef2;

	return false;
}

const bool Navigation::FindShortestRoute(const std::string& params) const
{
	istringstream inString(params);
	string command, mode;
	int linkRef1, linkRef2;
	inString >> command;
	inString >> mode;
	inString >> linkRef1;
	inString >> linkRef2;

	/*
	vector<int> adj[1024];

	for (auto& element : nodes)
	{
		for (auto& arc : element.m_arcs)
		{
			add_edge(adj, arc.linkref1, arc.linkref2);
		}
	}

	double distance;
	vector<int> routePath = getShortestDistance(adj, linkRef1, linkRef2, v, distance);

	if (distance > 0)
	{
		cout << "FindShortestPath " << mode << " " << linkRef1 << " " << linkRef2 << endl;

		for (auto& route : routePath)
		{
			cout << route << endl;
		}

		return true;
	}*/
	return false;
}

const bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks) const
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
		{
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

					nodes.push_back(Node::node(placeName, placesRef, placesLat, placesLong));
					v++;

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

					for (auto& element : nodes)
					{
						if (element.refnum == linkRef1)
							element.m_arcs.push_back(Arc::arc(linkRef1, linkRef2, linkMode));
					}

					if (debug && showloading) cout << "LinkMode: " << linkMode << ", Ref1: " << linkRef1 << ", Ref2: " << linkRef2 << endl;
				}

				if (debug && showloading) cout << "Finished Loading Links." << endl << endl;

				finLinks.close();
			}

			if (debug)
			{
				int i = 0;
				for (auto& element : nodes)
				{
					cout << "Node[" << i << "] Nodename: " << element.Nodename << ", Ref: " << element.refnum << ", Long: " << element.longi << ", Lat: " << element.lat << endl;
					for (const auto& arc : element.m_arcs)
					{
						cout << "	LinkMode: " << arc.transportmode << ", Ref1: " << arc.linkref1 << ", Ref2: " << arc.linkref2 << endl;
					}
					cout << endl;
					i++;
				}
			}
		}
	}
	return true;
}

// Add your code here
