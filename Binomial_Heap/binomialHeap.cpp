#include <bits/stdc++.h>
#include <fstream>

using namespace std;

class Node{
    int key, degree;
    Node *parent, *leftChild, *rightSibling;

public:
    Node(int val){
        key = val; degree = 0;
        parent = NULL;
        leftChild = NULL;
        rightSibling = NULL;
    }
    Node(){}

    //Setters
    void setKey(int val) { key = val; }
    void setDegree(int deg) { degree = deg; }
    void setParent(Node *node) { parent = node; }
    void setChild(Node *node) { leftChild = node; }
    void setSibling(Node *node) { rightSibling = node; }
    //Getters
    int getKey() { return key; }
    int getDegree() { return degree; }
    Node *getParent() { return parent; }
    Node *getChild() { //cout<<"GetChild Executed"<<endl;
                       return leftChild; }
    Node *getSibling() { //cout<<"GetSibling Executed"<<endl;
                         return rightSibling; }
};

class BinomialHeap{
    list<Node*> heap;

public:
    BinomialHeap(){}

    list<Node*> getHeap() { return heap; }

    Node* mergeTrees(Node *firstRoot, Node *secondRoot){
        if(firstRoot->getKey() > secondRoot->getKey())
            swap(firstRoot, secondRoot);

        //We merge the trees and make the smaller root
        //the root of the new tree
        secondRoot->setParent(firstRoot);
        secondRoot->setSibling(firstRoot->getChild());
        firstRoot->setChild(secondRoot);
        firstRoot->setDegree(firstRoot->getDegree()+1);
        return firstRoot;
    }

    list<Node*> heapUnion(list<Node*> firstHeap, list<Node*> secondHeap){
        list<Node*> tempHeap;
        list<Node*>::iterator firstIt = firstHeap.begin();
        list<Node*>::iterator secondIt = secondHeap.begin();

        while(firstIt != firstHeap.end() && secondIt != secondHeap.end()){

            //Putting the lower keys first
            if((*firstIt)->getDegree() <= (*secondIt)->getDegree()){
                tempHeap.push_back(*firstIt);
                firstIt++;
            }
            else{
                tempHeap.push_back(*secondIt);
                secondIt++;
            }
        }

        //Filling out the rest
        while(firstIt != firstHeap.end()){
            tempHeap.push_back(*firstIt);
            firstIt++;
        }
        while(secondIt != secondHeap.end()){
            tempHeap.push_back(*secondIt);
            secondIt++;
        }

        return tempHeap;
    }//Heap Union Function


    list<Node*> binomialHeapify(list<Node*> tempHeap){
        //Base case
        if(tempHeap.size() <= 1)
            return tempHeap;

        list<Node*>::iterator firstIt, secondIt, thirdIt;
        firstIt = secondIt = thirdIt = tempHeap.begin();


        secondIt++;
        thirdIt = secondIt;
        thirdIt++;

        while(firstIt != tempHeap.end()){

            //Only one element left
            if(secondIt == tempHeap.end())
                firstIt++;

            //Merging Not Possible, nor necessary/ Moving forward
            else if((*firstIt)->getDegree() < (*secondIt)->getDegree()){
                firstIt++; secondIt++;
                if(thirdIt != tempHeap.end())
                    thirdIt++;
            }

            //If 3 components have same degree, we leave the first and merge the next two
            //More than 3 components cannot have the same degree at the same time
            //Two bits and one borrow********************************************
            else if(thirdIt != tempHeap.end() && (*firstIt)->getDegree()==(*secondIt)->getDegree() && (*firstIt)->getDegree()==(*thirdIt)->getDegree() ){
                firstIt++; secondIt++; thirdIt++;
            }

            //We eliminate one tree having same degree and merge it with the other
            else if((*firstIt)->getDegree() == (*secondIt)->getDegree()){
                *firstIt = mergeTrees(*firstIt, *secondIt);
                secondIt = tempHeap.erase(secondIt);
                if(thirdIt != tempHeap.end())
                    thirdIt++;
            }

        }
        //Returning the modified heap
        return tempHeap;
    }//Binomial Heapify Function


    list<Node*> insertion(Node *node){
        list<Node*> tempHeap;
        tempHeap.push_back(node);

        tempHeap = heapUnion(heap, tempHeap);
        return binomialHeapify(tempHeap);
    }//Insertion Function

    void Insert(int key){
        heap = insertion(new Node(key));
    }//Insertion Driver


    void skipNodes(Node *rootNode){
        if(!rootNode) return;
        while(rootNode){
            skipNodes(rootNode->getSibling());
            rootNode = rootNode->getChild();
        }
    }

    Node* findMinNode(){
        list<Node*>::iterator it = heap.begin();
        Node *minNode = *it;
        while(it != heap.end()){
            if((*it)->getKey() < minNode->getKey()) minNode = *it;
            skipNodes(*it);//Not necessary
            it++;
        }

        return minNode;
    }//Finding Min Node

    int findMin(){
        return findMinNode()->getKey();
    }//Finding Min Value

    list<Node*> removeMinNode(Node* minNode){
        list<Node*> heapOffMinNode;
        Node *minNodeChild = minNode->getChild();
        Node *temp;

        while(minNodeChild){
            temp = minNodeChild;
            minNodeChild = minNodeChild->getSibling();
            temp->setSibling(NULL);
            heapOffMinNode.push_front(temp);
        }
        return heapOffMinNode;
    }

    Node* extractMin(){
        Node *minNode = findMinNode();
        list<Node*> extractedHeap, tempHeap;
        list<Node*>::iterator it = heap.begin();

        while(it!=heap.end()){
            if((*it)!=minNode)
                extractedHeap.push_back(*it);
            it++;
        }

        tempHeap = removeMinNode(minNode);
        heap = heapUnion(extractedHeap, tempHeap);
        heap = binomialHeapify(heap);
        return minNode;
    }


    void unionOfNewHeap(vector<int> parameters){
        BinomialHeap tempBinHeap;
        for(auto i: parameters)
            tempBinHeap.Insert(i);

        heap = heapUnion(heap, tempBinHeap.getHeap());
        heap = binomialHeapify(heap);
    }


    void printTree(Node *node, int CurrentLevel, ofstream &outstream){
        if(!node){
             //cout<<"This is Null"<<endl;
             return;
        }
        int checkLevel = 0;
        while(node){
            if(checkLevel == CurrentLevel){
                outstream<<endl<<"Level "<<CurrentLevel<<" : ";
                checkLevel++;
            }
            outstream<<node->getKey()<<"  ";
            printTree(node->getSibling(), CurrentLevel, outstream);
            CurrentLevel++;
            node = node->getChild();
            //outstream<<endl;
            //if(!node) cout<<"This is Null"<<endl;
        }
    }


    void printHeap(ofstream &outstream){
        list<Node*> ::iterator it;
        it = heap.begin();

        outstream<<"Printing Binomial Heap...";
        int startLevel = 0;
        while (it != heap.end()){
            outstream<<endl<<"Binomial Tree, B"<<(*it)->getDegree();;
            printTree(*it, startLevel, outstream);
            it++;
        }
    }

    void functionGenerator(string operation, vector<int> parameters, ofstream &outstream){
        if(operation=="I")
            Insert(parameters[0]);
        if(operation=="F")
            outstream<<"Find-Min returned "<<findMin()<<endl;
        if(operation=="E")
            outstream<<"Extract-Min returned "<<(extractMin())->getKey()<<endl;
        if(operation=="U")
            unionOfNewHeap(parameters);
        if(operation=="P")
            printHeap(outstream); outstream<<endl;
        return;
    }
};

int main(void){
    BinomialHeap bHeap;

    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("Out.txt");

    string operation; int param;
    vector<int> parameters;
    string line, word;

    while(!instream.eof()){
        getline(instream, line);
        int len = line.length();
        if(line[len-1] == ' ') line = line.substr(0, len-1);
        stringstream str(line);

        getline(str, word, ' ');
        operation = word;

        while(!str.eof()){
            getline(str, word, ' ');
            parameters.push_back(stoi(word));
        }

        bHeap.functionGenerator(operation, parameters, outstream);
        parameters.clear();
    }

    instream.close();
    outstream.close();
    return 0;
}
