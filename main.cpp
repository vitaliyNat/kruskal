#include <fstream>
#include "linked-list.h"
#include <string>

class Vertex{
public:
    double x;
    double y;

    Vertex(double x, double y){
        this->x = x;
        this->y = y;

    }
    string getVertixString(){
        return "( " + to_string(x) + ", " + to_string(y) + " ), ";
    }


};


class Edge{
public:
    Vertex * first;
    Vertex * second;
    double distance;
    Edge(Vertex * first, Vertex * second, double  distance){
        this->first = first;
        this->second = second;
        this->distance = distance;
    }
    double operator- (Edge  edge2) const{
        return this->distance - edge2.distance;
    }
    string getEdgeString(){
        return "[" +first->getVertixString() + second->getVertixString() + "],";
    }
};



class Graph{
public:
    linkedList<Vertex> * vertices;
    linkedList<Edge> * edges;
    unsigned int elements;


    Graph(){
        elements = 0;
        vertices =  new linkedList<Vertex>();
        edges = new linkedList<Edge>();
    }
    explicit Graph(Edge * edge){
        elements = 2;
        vertices = new linkedList<Vertex>();
        edges = new linkedList<Edge>();
        edges->push_tail(edge);
        vertices->push_tail(edge->first);
        vertices->push_tail(edge->second);
    }
    explicit Graph(Vertex * vertex){
        elements = 1;
        vertices = new linkedList<Vertex>();
        edges = new linkedList<Edge>();
        vertices->push_tail(vertex);
    }

    void merge(Graph * graph,Edge * edge){
        elements += graph->elements;
        auto *tmp1 = graph->vertices->getHead();
        while (tmp1 != nullptr) {
            this->vertices->push_tail(tmp1->data);
            tmp1 = tmp1->next;
        }

        auto *tmp2 = graph->edges->getHead();
        while (tmp2 != nullptr) {
            this->edges->push_tail(tmp2->data);
            tmp2 = tmp2->next;
        }
        this->edges->push_tail(edge);

    }

    void add(Edge * edge,Vertex * vertex){
        edges->push_tail(edge);
        vertices->push_tail(vertex);
        elements++;

    }
    void add(Edge * edge,Vertex * vertex1, Vertex * vertex2){
        edges->push_tail(edge);
        vertices->push_tail(vertex1);
        vertices->push_tail(vertex2);
        elements+=2;

    }
    void add(Edge * edge){
        edges->push_tail(edge);
    }

    string getVerticesString(){
        auto * tmp = this->vertices->getHead();
        string result = "[";
        while(tmp){
            result += tmp->data->getVertixString();
            tmp = tmp->next;
        }
        result+=" ]";
        return result;
    }
    string getEdgesString(){
        auto * tmp = this->edges->getHead();
        string result = "[";
        while(tmp){
            result += tmp->data->getEdgeString();
            tmp = tmp->next;
        }
        result+=" ]";
        return result;
    }


};

bool isUnion(Graph * first , Graph * second){
    auto *tmp1 = first->vertices->getHead();
    while (tmp1 != nullptr) {
        if(second->vertices->searchNode(tmp1->data)){
            return false;
        }
        tmp1 = tmp1->next;
    }
    return  true;
}

Graph * Kruskal(Graph * graph){
    auto * tmp = graph->vertices->getHead();
    auto * unionGraphs = new linkedList<Graph>();
    while(tmp  != nullptr){
        unionGraphs->push_tail( new Graph(tmp->data));
        tmp = tmp->next;
    }
    auto * edges = new linkedList<Edge>();
    auto * tmp1 = graph->edges->getHead();
    while(tmp1  != nullptr){
        edges->orderedInsert(tmp1->data);
        tmp1 = tmp1->next;
    }
    tmp1 = edges->getHead();
    while(unionGraphs->getSize() > 1){
        tmp1 = edges->getHead();
        auto * tempFirstVertex = graph->vertices->searchNode(tmp1->data->first);
        auto * tempSecondVertex = graph->vertices->searchNode(tmp1->data->second);
        auto * tempUnion = unionGraphs->getHead();
        Graph * tempFirstUnion;
        Graph * tempSecondUnion;
        while(tempUnion) {
            if(tempUnion->data->vertices->searchNode(tempFirstVertex->data)){
                tempFirstUnion = tempUnion->data;
            }
            if(tempUnion->data->vertices->searchNode(tempSecondVertex->data)){
                tempSecondUnion = tempUnion->data;
            }
            tempUnion = tempUnion->next;
        }
        if(isUnion(tempFirstUnion,tempSecondUnion)){
            tempFirstUnion->merge(tempSecondUnion,tmp1->data);
            unionGraphs->deleteNodeValue(tempSecondUnion);
        }
        tmp1  = tmp1->next;
        edges->deleteFirst();

    }

    return unionGraphs->getHead()->data;

}
Graph * readGraphFromFile(const string fileName){
    auto * graph = new Graph();
    ifstream inputFile("./"+fileName+".txt",ifstream::in);

    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return nullptr;
    }
    int numberOfVertex;
    inputFile>> numberOfVertex;
    for(int i = 0; i< numberOfVertex;i++){
        double x,y;
        inputFile >> x >> y;
        graph->vertices->push_tail(new Vertex(x,y));
        graph->elements++;
    }

    int numberOfEdges;
    inputFile >> numberOfEdges;
    for(int i = 0; i< numberOfEdges;i++){
        double dist;
        int x, y;
        inputFile>> x >> y >> dist;
        graph->edges->push_tail(new Edge(graph->vertices->index(x)->data,graph->vertices->index(y)->data, dist));
    }
    inputFile.close();

    return graph;
}


int main() {
    auto * graph = readGraphFromFile("g1");
    auto * temp1 = graph->edges->getHead();
    double  sum1 = 0;
    while(temp1){
        sum1+=temp1->data->distance;
        temp1 = temp1->next;
    }
    cout<<sum1 <<endl;
    auto * kruskal = Kruskal(graph);

    auto * temp = kruskal->edges->getHead();
    double  sum = 0;
    while(temp){
        sum+=temp->data->distance;
        temp = temp->next;
    }
    cout<< sum<<endl;
    cout<< kruskal->getVerticesString()<<endl;
    cout<< kruskal->getEdgesString();

    return 0;
}
