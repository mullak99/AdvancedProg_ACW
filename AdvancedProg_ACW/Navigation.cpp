#include "Navigation.h"
#include "ACW_Wrapper.h"
#include "Node.h"
#include "Arc.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

vector<Node::node> nodes;

vector<int> adj[1024];
vector<string> adjMode[1024];

ofstream outputFile;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);

Navigation::Navigation() : _outFile("Output.txt")
{
	InitFile();
}

Navigation::~Navigation()
{
}

const void Navigation::InitFile()
{
	outputFile.open("Output.txt");

	outputFile << fixed << setprecision(3);
}

/*
	CREDITS:

	The 'add_edge', 'BFS' and 'findShortestPath' methods were based on the 'Shortest path in an unweighted graph'
	code that can be found on this page: https://www.geeksforgeeks.org/shortest-path-unweighted-graph/

	Variations in the code was needed to add support for Transport Mode checking.
*/

const void add_edge(vector<int> adj[], vector<string> adjMode[], int src, int dest, string mode)
{
	adj[src].push_back(dest);
	adj[dest].push_back(src);

	adjMode[src].push_back(mode);
	adjMode[dest].push_back(mode);
}

const bool BFS(vector<int> adj[], vector<string> adjMode[], int src, int dest, string mode, int v, int pred[], int dist[])
{
	list<int> queue;
	bool* visited = new bool[v];

	for (int i = 0; i < v; i++)
	{
		visited[i] = false;
		dist[i] = INT_MAX;
		pred[i] = -1;
	}

	visited[src] = true;
	dist[src] = 0;
	queue.push_back(src);

	while (!queue.empty())
	{
		int u = queue.front();
		queue.pop_front();
		for (int i = 0; i < adj[u].size(); i++)
		{
			if (!visited[adj[u][i]])
			{
				visited[adj[u][i]] = true;
				dist[adj[u][i]] = dist[u] + 1;
				pred[adj[u][i]] = u;
				queue.push_back(adj[u][i]);

				if (mode == "" && adj[u][i] == dest)
					return true;
				else if (adj[u][i] == dest && adjMode[u][i] == mode)
					return true;
			}
		}
	}
	return false;
}

bool findShortestPath(vector<int> adj[], vector<string> adjMode[], int s, int dest, string mode, int v, list<int> &routePath, int &pathLen)
{
	int* pred = new int[v];
	int* dist = new int[v];

	if (!BFS(adj, adjMode, s, dest, mode, v, pred, dist)) return false;

	vector<int> path;
	int crawl = dest;
	path.push_back(crawl);
	while (pred[crawl] != -1)
	{
		path.push_back(pred[crawl]);
		crawl = pred[crawl];
	}

	pathLen = dist[dest];
	for (long i = static_cast<long>(path.size()) - 1; i >= 0; i--)
		routePath.push_back(path[i]);

	return true;
}

const bool Navigation::ProcessCommand(const string& commandString) const
{
	istringstream inString(commandString);
	string command;
	inString >> command;

	if (command == "MaxDist")
		return Navigation::MaxDist();
	else if (command == "MaxLink")
		return Navigation::MaxLink();
	else if (command == "FindDist")
		return Navigation::FindDist(commandString);
	else if (command == "FindNeighbour")
		return Navigation::FindNeighbour(commandString);
	else if (command == "Check")
		return Navigation::Check(commandString);
	else if (command == "FindRoute")
		return Navigation::FindRoute(commandString);
	else if (command == "FindShortestRoute")
		return Navigation::FindShortestRoute(commandString);
	else return false;
}

double GetDistance(const double x1, const double y1, const double x2, const double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) / 1000;
}

const bool Navigation::MaxDist() const
{
	Node::node* linkStartNode = nullptr;
	Node::node* linkEndNode = nullptr;
	double largestDist = 0;

	for (auto& node1 : nodes)
	{
		linkStartNode = &node1;

		for (auto& node2 : nodes)
		{
			linkEndNode = &node2;
			double lati1, longi1, lati2, longi2;

			LLtoUTM(linkStartNode->latitude, linkStartNode->longitude, lati1, longi1);
			LLtoUTM(linkEndNode->latitude, linkEndNode->longitude, lati2, longi2);

			const double dist = GetDistance(lati1, longi1, lati2, longi2);
			if (dist > largestDist) largestDist = dist;
		}
	}
	if (largestDist > 0)
	{
		outputFile << "MaxDist" << endl << linkStartNode->nodeName << "," << linkEndNode->nodeName << "," << setprecision(3) << largestDist << endl << endl;
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

		for (auto& arc : element.nodeArcs)
		{
			for (auto& nodeatlink : nodes)
			{
				if (nodeatlink.refNum == arc.linkRef2)
					linkEndNode = &nodeatlink;
			}
			if (linkEndNode != nullptr)
			{
				double lati1, longi1, lati2, longi2;

				LLtoUTM(linkStartNode->latitude, linkStartNode->longitude, lati1, longi1);
				LLtoUTM(linkEndNode->latitude, linkEndNode->longitude, lati2, longi2);

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
		outputFile << "MaxLink" << endl << largestArc->linkRef1 << "," << largestArc->linkRef2 << "," << setprecision(3) << largestDist << endl << endl;
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

	string node1Name, node2Name;

	if (command != "" && linkRef1 != NULL && linkRef2 != NULL)
	{
		int i, j;
		for (i = 0; i < static_cast<int>(nodes.size()); i++)
		{
			if (nodes[i].refNum == linkRef1)
			{
				node1Name = nodes[i].nodeName;
				break;
			}
		}
		for (j = 0; j < static_cast<int>(nodes.size()); j++)
		{
			if (nodes[j].refNum == linkRef2)
			{
				node2Name = nodes[j].nodeName;
				break;
			}
		}

		list<int> posPath;
		int posLen;
		vector<int> refs;

		findShortestPath(adj, adjMode, i, j, "", static_cast<int>(nodes.size()), posPath, posLen);

		if (posLen > 0)
		{
			for (auto& path : posPath)
				refs.push_back(nodes[path].refNum);

			const int refsSize = static_cast<int>(refs.size());
			if (refsSize > 0)
			{
				double totalDist = 0;
				for (int i = 0; i < refsSize - 1; i++)
				{
					const int j = i + 1;
					const int refBegin = refs[i];
					const int refEnd = refs[j];

					Node::node* linkStartNode = nullptr;
					Node::node* linkEndNode = nullptr;

					for (auto& node : nodes)
					{
						if (node.refNum == refBegin) linkStartNode = &node;
						else if (node.refNum == refEnd) linkEndNode = &node;
					}

					if (linkStartNode != nullptr && linkEndNode != nullptr)
					{
						double lati1, longi1, lati2, longi2;

						LLtoUTM(linkStartNode->latitude, linkStartNode->longitude, lati1, longi1);
						LLtoUTM(linkEndNode->latitude, linkEndNode->longitude, lati2, longi2);

						totalDist += GetDistance(lati1, longi1, lati2, longi2);
					}
					else return false;
				}
				outputFile << params << endl;
				outputFile << node1Name << "," << node2Name << "," << setprecision(3) << totalDist << endl << endl;

				return true;
			}
			else return false;
		}
		else return false;
	}
	return false;
}

const bool Navigation::FindNeighbour(const std::string& params) const
{
	istringstream inString(params);
	string command;
	int linkRef;
	inString >> command;
	inString >> linkRef;

	outputFile << params << endl;

	bool done = false;
	for (auto& element : nodes)
	{
		for (const auto& arc : element.nodeArcs)
		{
			if (arc.linkRef1 == linkRef)
			{
				done = true;
				outputFile << arc.linkRef2 << endl;
			}
			else if (arc.linkRef2 == linkRef)
			{
				done = true;
				outputFile << arc.linkRef1 << endl;
			}
		}
	}
	outputFile << endl;

	return done;
}

const bool Navigation::Check(const std::string& params) const
{
	istringstream inString(params);
	string command, mode;
	vector<int> refs;
	inString >> command;
	inString >> mode;

	for (int i = 0; inString >> i; )
		refs.push_back(i);

	vector<short> valid;
	const int refsSize = static_cast<int>(refs.size());
	if (refsSize > 0)
	{
		outputFile << params << endl;

		for (int i = 0; i < refsSize - 1; i++)
		{
			const int j = i + 1;
			const int refBegin = refs[i];
			const int refEnd = refs[j];

			bool refBeginRan = false;
			bool refEndRan = false;
			for (auto& element : nodes)
			{
				if (element.refNum == refBegin || element.refNum == refEnd)
				{
					if (element.refNum == refBegin) refBeginRan = true;
					else if (element.refNum == refEnd) refEndRan = true;
					for (const auto& arcs : element.nodeArcs)
					{
						if (arcs.linkRef1 == refBegin && arcs.linkRef2 == refEnd && arcs.transportMode == mode)
						{
							refBeginRan = false;
							refEndRan = false;
							valid.push_back(1);
							break;
						}
						else if (arcs.linkRef1 == refEnd && arcs.linkRef2 == refBegin && arcs.transportMode == mode)
						{
							refBeginRan = false;
							refEndRan = false;
							valid.push_back(1);
							break;
						}
					}
					if ((valid.empty() || static_cast<int>(valid.size()) < i + 1) && refBeginRan && refEndRan)
					{
						valid.push_back(0);
						break;
					}
				}
			}
		}

		for (int i = 0; i < refsSize - 1; i++)
		{
			const int j = i + 1;
			if (valid[i] == 1)
				outputFile << refs[i] << "," << refs[j] << ",PASS" << endl;
			else
			{
				outputFile << refs[i] << "," << refs[j] << ",FAIL" << endl;
				break;
			}
		}
		outputFile << endl;
		return true;
	}
	return false;
}

const bool Navigation::FindRoute(const std::string& params) const
{
	return FindShortestRoute(params);
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
	
	if (command != "" && mode != "" && linkRef1 != NULL && linkRef2 != NULL)
	{
		int i, j;
		for (i = 0; i < static_cast<int>(nodes.size()); i++)
			if (nodes[i].refNum == linkRef1) break;
		for (j = 0; j < static_cast<int>(nodes.size()); j++)
			if (nodes[j].refNum == linkRef2) break;

		list<int> posPath;
		int posLen;

		findShortestPath(adj, adjMode, i, j, mode, static_cast<int>(nodes.size()), posPath, posLen);

		outputFile << params << endl;

		if (posLen > 0)
		{
			for (auto& path : posPath)
				outputFile << nodes[path].refNum << endl;
		}
		else outputFile << "FAIL" << endl;
			
		outputFile << endl;
		return true;
	}
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
				}
				finPlaces.close();
			}
			
			{
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
						if (element.refNum == linkRef1) element.nodeArcs.push_back(Arc::arc(linkRef1, linkRef2, linkMode));

					int i, j;
					for (i = 0; i < static_cast<int>(nodes.size()); i++)
						if (nodes[i].refNum == linkRef1) break;

					for (j = 0; j < static_cast<int>(nodes.size()); j++)
						if (nodes[j].refNum == linkRef2) break;

					add_edge(adj, adjMode, i, j, linkMode);
				}
				finLinks.close();
			}
		}
	}
	return true;
}
