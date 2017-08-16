// CheapestandShortestRoute.cpp
// by Ben Van de Brooke
// On 11/28/16
// Editor: X-Code
// Compiler: GCC
// Description: Parses a file containing information about distances between cities. Uses modified djikstra algorithms to find cheapest and shortest routes between cities

#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <stack>


using namespace std;

struct Vertex
{
    string name;
    int pathPrevious;
    float pathCost;
    bool isVisited;
    list<pair<int,float>> adjacentVertices;

    Vertex():name(" "), pathPrevious(-1), pathCost(-.1),isVisited(false),adjacentVertices(){};
};

struct Edge
{
    int toIndex, fromIndex;
    float totalCost;
    
    bool operator < (const Edge & otherEdge) const
    {
        return totalCost > otherEdge.totalCost;
    }

    Edge(int to, int from, float value): toIndex(to), fromIndex(from), totalCost(value){};//parameterized constructor
};

pair<stack<int>, float> shortestPath(int start, int end, vector<Vertex> vertices)
{
    
    pair <stack<int>, float> shortest;
    priority_queue<Edge> toDo;
    //clear the path parts from the verticies
    //for loop resetting
    
    for(int x = 0; x < vertices.size(); x++)
    {
        vertices[x].isVisited = false;
        vertices[x].pathCost = 1;
        vertices[x].pathPrevious = 0;
    }
    
    
    
    //push start into todo priority que(use edge parameterized constructor)
    Edge begin(start,start,0);
    toDo.push(begin);
    
    while(!toDo.empty())
    {
        Edge current = toDo.top();
        toDo.pop();
        
        if(vertices.at(current.toIndex).pathCost < current.totalCost && vertices.at(current.toIndex).isVisited)
        {
            continue;
        }
        
        vertices.at(current.toIndex).pathPrevious = current.fromIndex;
        vertices.at(current.toIndex).pathCost = current.totalCost;
        vertices.at(current.toIndex).isVisited = true;
        
        if(current.toIndex == end)
        {
            break;
        }
        
        for(pair<int,float> x : vertices.at(current.toIndex).adjacentVertices)
        {
            if(vertices.at(x.first).pathCost > (1 + vertices.at(current.toIndex).pathCost) || !vertices.at(x.first).isVisited )
            {
                Edge add(x.first, current.toIndex, (1 + vertices.at(current.toIndex).pathCost));
                toDo.push(add);
            }
        }
        
        
    }
    
    stack<int> route;
    
    Vertex aVert = vertices[end];
    
    route.push(end);
    
    while(aVert.pathCost != 0)
    {
        route.push(aVert.pathPrevious);
        aVert = vertices[aVert.pathPrevious];
    }
    
    
    shortest.first = route;
    shortest.second = vertices[end].pathCost;
    
    
    return shortest;
}






pair<stack<int>, float> cheapestPath(int start, int end, vector<Vertex> vertices)
{
    
    pair <stack<int>, float> cheapest;
    priority_queue<Edge> toDo;
    //clear the path parts from the verticies
        //for loop resetting
    
    for(int x = 0; x < vertices.size(); x++)
    {
        vertices[x].isVisited = false;
        vertices[x].pathCost = 0;
        vertices[x].pathPrevious = 0;
    }
    
    
    
    //push start into todo priority que(use edge parameterized constructor)
    Edge begin(start,start,0);
    toDo.push(begin);
    
    while(!toDo.empty())
    {
        Edge current = toDo.top();
        toDo.pop();
        
        if(vertices.at(current.toIndex).pathCost < current.totalCost && vertices.at(current.toIndex).isVisited)
        {
           continue;
        }
        
        vertices.at(current.toIndex).pathPrevious = current.fromIndex;
        vertices.at(current.toIndex).pathCost = current.totalCost;
        vertices.at(current.toIndex).isVisited = true;
        
        if(current.toIndex == end)
        {
            break;
        }
        
        for(pair<int,float> x : vertices.at(current.toIndex).adjacentVertices)
        {
            if(vertices.at(x.first).pathCost > (x.second + vertices.at(current.toIndex).pathCost) || !vertices.at(x.first).isVisited )
            {
                Edge add(x.first, current.toIndex, (x.second + vertices.at(current.toIndex).pathCost));
                toDo.push(add);
            }
        }
        
        
    }
    
    stack<int> route;

    Vertex aVert = vertices[end];
    
    route.push(end);
    
    while(aVert.pathCost != 0)
    {
        route.push(aVert.pathPrevious);
        aVert = vertices[aVert.pathPrevious];
    }
    
    
    cheapest.first = route;
    cheapest.second = vertices[end].pathCost;
    
    
    return cheapest;
}

int main()
{
    
    //Universal Program Requirements
    cout << "VandeBrookeBenLab6bDvcSchedule16GraphCheapestandShortest.cpp " << endl;
    cout << "by Ben Van de Brooke" << endl;
    cout << "On 11/28/16" << endl;
    cout << "Editor: X-Code" << endl;
    cout << "Compiler: GCC" << endl;
    cout << "Description: Parses a file containing information about distances between cities. Uses modified djikstra algorithms to find cheapest and shortest routes between cities" << endl;
    
    ifstream fin;
    fin.open("cities.txt");
    if (!fin.good()) throw "I/O error";
    
    // process the input file
    vector<Vertex> database;
    
    //creates graph of cities connected to eachother
    while (fin.good()) // EOF loop
    {
        string fromCity, toCity, cost;
        
        // read one edge
        getline(fin, fromCity);
        getline(fin, toCity);
        getline(fin, cost); // skip the line with distance
        fin.ignore(1000, 10); // skip the separator
        
        // code to eliminate whitespaces
        fromCity.erase(remove_if(fromCity.begin(), fromCity.end(), ::isspace), fromCity.end());
        toCity.erase(remove_if(toCity.begin(), toCity.end(), ::isspace), toCity.end());
        
        float edgeCost = atof(cost.c_str());
        
        
        // add vertices for new cities included in the edge
        int iToVertex = -1, iFromVertex = -1, i;
        for (i = 0; i < database.size(); i++) // seek "to" city
            if (database[i].name == fromCity)
                break;
        if (i == database.size()) // not in database yet
        {
            // store the vertex if it is new
            Vertex fromVertex;
            fromVertex.name = fromCity;
            fromVertex.pathCost = edgeCost;
            database.push_back(fromVertex);
            
        }
        iFromVertex = i;
        
        
        for (i = 0; i < database.size(); i++) // seek "from" city
            if (database[i].name == toCity)
                break;
        if (i == database.size()) // not in vector yet
        {
            // store the vertex if it is new
            Vertex toVertex;
            toVertex.name = toCity;
            toVertex.pathCost = edgeCost;
            database.push_back(toVertex);
        }
        iToVertex = i;
        
        // store bi-directional edges
        database[iFromVertex].adjacentVertices.push_back(pair<int,float> (iToVertex,edgeCost));
        database[iToVertex].adjacentVertices.push_back(pair<int,float> (iFromVertex,edgeCost));
    }
   
    fin.close();
    cout << "Input file processed" << endl << endl;
    
    string input;
    int startIndex = 0, endIndex = 0;
    
    //user input controlled
    while(input != "Q" && input != "q")
    {
        cout << "Enter a city name (without spaces) where you would like to start: ";
        getline(cin,input);
        
        if(input == "Q" || input == "q") continue;
        
        for(int x = 0; x < database.size(); x++)
        {
           if( database[x].name == input)
           {
               startIndex = x;
           }
        }
        
        cout << "Enter the name of the city where you want to go to (without spaces): ";
        getline(cin,input);
        
        if(input == "Q" || input == "q") continue;
        
        for(int x = 0; x < database.size(); x++)
        {
            if( database[x].name == input)
            {
                endIndex = x;
            }
        }
    
        pair<stack<int>,float> path = cheapestPath(startIndex, endIndex, database);

        cout << "Cheapest Path: " << path.second << endl;
        
        while(path.first.size() != 0)
        {
            cout << database[path.first.top()].name << " ";
            path.first.pop();
        }
        
        path = shortestPath(startIndex, endIndex, database);
        
        cout << endl << endl;
        cout << "Shortest Path: " << path.second << endl;
        
        while(path.first.size() != 0)
        {
            cout << database[path.first.top()].name << " ";
            path.first.pop();
        }

        cout << endl << endl;
   
    }

    return 0;
}