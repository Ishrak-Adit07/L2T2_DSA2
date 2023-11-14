#include <bits/stdc++.h>
#include <fstream>

using namespace std;

//Helper functions and classes
int max(int a, int b) { return a>b?a:b; }
class Node{
    int key, height;
    Node* left;
    Node *right;

public:
    Node(int k){
        key = k; height = 1;
        left = NULL; right = NULL;
    }
    int getKey() { return key; }
    void setKey(int k) { key = k; }

    int getHeight(){ return height; }
    void setHeight(int H) { height = H; }

    Node* getLeft() { return left; }
    Node* getRight() { return right; }

    void setLeft(Node* node) { left = node; }
    void setRight(Node* node) { right = node; }

    bool isLeft() {
        if(left) return true;
        return false; }
    bool isRight() {
        if(right) return true;
        return false; }
};

//AVL Tree class
class AVLtree{
    Node* head;
    double totalInsertionTime, totalDeletionTime;
    double totalFindTime, totalTraverseTime;

public:
    AVLtree(){
        head = NULL;
        totalInsertionTime = totalDeletionTime = 0;
        totalFindTime = totalTraverseTime = 0;
    }

    void reportAVL(ofstream &timestream){
        timestream<<"operation time(ms)"<<endl;
        timestream<<"insert "<<totalInsertionTime<<endl;
        timestream<<"delete "<<totalDeletionTime<<endl;
        timestream<<"search "<<totalFindTime<<endl;
        timestream<<"trav "<<totalTraverseTime<<endl;
        timestream<<"total "<<(totalInsertionTime+totalDeletionTime+totalFindTime+totalTraverseTime)<<endl;
    }

    //Getting Height of a Node
    int getHeight(Node* node){
        if(node == NULL) return 0;
        return node->getHeight();
    }

    //Getting Balance Factor of a Node
    int getBalance(Node *node){
        if(node == NULL) return 0;
        return ( getHeight(node->getLeft()) - getHeight(node->getRight()) );
    }

    //Right Rotation of a Node
    Node* rightRotation(Node* node){
        Node* leftChild = node->getLeft();
        Node* rightGrandChild = leftChild->getRight();

        //Updating tree
        leftChild->setRight(node);
        node->setLeft(rightGrandChild);

        //Updating Heights
        node->setHeight( max(getHeight(node->getLeft()), getHeight(node->getRight()) ) +1 );
        leftChild->setHeight( max( getHeight(leftChild->getLeft()), getHeight(leftChild->getRight()) )+1 );

        //Tree update
        return leftChild;
    }

    Node* leftRotation(Node *node){
        Node* rightChild = node->getRight();
        Node *leftGrandChild = rightChild->getLeft();

        //Updating Tree
        rightChild->setLeft(node);
        node->setRight(leftGrandChild);

        //Updating Heights
        node->setHeight( max( getHeight(node->getLeft()), getHeight(node->getRight()) ) +1 );
        rightChild->setHeight( max( getHeight(rightChild->getLeft()), getHeight(rightChild->getRight()) ) +1 );

        //Tree update
        return rightChild;
    }

    Node *insertion(Node *node, int val){
        //Also handles the case of no previous node in the tree
        if(node == NULL) return new Node(val);

        //Sending new node in the proper way
        if(val < node->getKey()) node->setLeft( insertion(node->getLeft(), val) );
        else if(val > node->getKey()) node->setRight( insertion(node->getRight(), val) );
        else return node;//Not allowing duplicate node in tree

        //Updating node height
        node->setHeight( 1+ max( getHeight(node->getLeft()), getHeight(node->getRight()) ) );

        //Checking node Balance Factor
        int bFactor = getBalance(node);

        //Left-Left Case
        if(bFactor > 1 && val < node->getLeft()->getKey())
            return rightRotation(node);

        //Left-Right Case
        if(bFactor > 1 && val > node->getLeft()->getKey()){
            node->setLeft( leftRotation(node->getLeft()) );
            return rightRotation(node);
        }

        //Right-Right Case
        if(bFactor < -1 && val > node->getRight()->getKey())
            return leftRotation(node);

        //Right-Left Case
        if(bFactor < -1 and val < node->getRight()->getKey()){
            node->setRight( rightRotation(node->getRight()) );
            return leftRotation(node);
        }

        return node;
    }//Insertion Function

    void Insert(int key, ofstream &outstream){
        auto start_time = std::chrono::high_resolution_clock::now();

        head = insertion(head, key);//Calling Insertion

        auto stop_time = std::chrono::high_resolution_clock::now();
        double lapse = chrono::duration_cast<chrono::milliseconds>(stop_time-start_time).count();
        lapse *= 1e-6;
        totalInsertionTime += lapse;

        //Display after insertion
        traverseInorder(head, outstream);
        outstream<<endl;
    }//Insertion Driver and Display Function

    //Minimum node of Right Subtree
    Node* inorderSuccessor(Node *node){
        Node* temp = node->getRight();
        while(temp->isLeft()) temp = temp->getLeft();
        return temp;
    }//Successor(inorder) Function

    Node* deletion(Node* node, int val){
        //Tree is empty/Value is not in the tree
        if(node == NULL) return node;

        //Checking the tree in the proper way
        if(val < node->getKey()) { node->setLeft( deletion(node->getLeft(), val) ); }
        else if(val > node->getKey()) { node->setRight( deletion(node->getRight(), val) ); }

        //If we reach here, this is the node to be deleted
        else{
            //No child
            if(!node->isLeft() && !node->isRight())
                node = NULL;

            //Only Left Child
            else if(!node->isRight())
                node = node->getLeft();

            //Only Right Child
            else if(!node->isLeft())
                node = node->getRight();

            //Have Both Children
            else{
                Node *successor = inorderSuccessor(node);
                node->setKey(successor->getKey());
                node->setRight( deletion(node->getRight(), successor->getKey()) );
            }
        }

        //If node itself is deleted
        if(node == NULL)
            return node;

        //Updating Height of node
        node->setHeight( 1+ max( getHeight(node->getLeft()), getHeight(node->getRight()) ));

        //Checking Balance Factor of the node
        int bFactor = getBalance(node);

        //Left-Left Case
        if( bFactor > 1 && getBalance(node->getLeft()) >=0 )
            return rightRotation(node);
        //Left-Right Case
        if( bFactor > 1 && getBalance(node->getLeft()) < 0 ){
            node->setLeft( leftRotation(node->getLeft()) );
            return rightRotation(node);
        }
        //Right-Right Case
        if( bFactor < -1 && getBalance(node->getRight()) <=0 )
            return leftRotation(node);
        //Right-Left Case
        if( bFactor < -1 && getBalance(node->getRight()) > 0 ){
            node->setRight( rightRotation(node->getRight()) );
            return leftRotation(node);
        }

        return node;
    }//Deletion Function

    void Delete(int key, ofstream &outstream){
        auto start_time = std::chrono::high_resolution_clock::now();

        head = deletion(head, key);//Calling Deletion

        auto stop_time = std::chrono::high_resolution_clock::now();
        double lapse = chrono::duration_cast<chrono::milliseconds>(stop_time-start_time).count();
        lapse *= 1e-6;
        totalDeletionTime += lapse;

        //Display after deletion
        traverseInorder(head, outstream);
        outstream<<endl;
    }//Delete Driver and Display Function

    bool findVal(Node* node, int val){
        if(node == NULL) return false;
        if( val == node->getKey() ) return true;
        else if( val > node->getKey() ) return findVal(node->getRight(), val);
        else if( val < node->getKey() ) return findVal(node->getLeft(), val);
        return false;
    }//Find Key Operation

    bool findKey(int val){
        auto start_time = std::chrono::high_resolution_clock::now();

        bool keyCheck = findVal(head, val);

        auto stop_time = std::chrono::high_resolution_clock::now();
        double lapse = chrono::duration_cast<chrono::milliseconds>(stop_time-start_time).count();
        lapse *= 1e-6;
        totalFindTime += lapse;

        return keyCheck;
    }//Find Key Driver Function

    //To traverse after traverse command
    void inorder(Node *node, ofstream &outstream){
        if(node == NULL) return;
        inorder(node->getLeft(), outstream);
        outstream<<node->getKey()<<"  ";
        inorder(node->getRight(), outstream);
    }//Inorder Function

    void inorderDriver(ofstream &outstream){
        auto start_time = std::chrono::high_resolution_clock::now();

        inorder(head, outstream);

        auto stop_time = std::chrono::high_resolution_clock::now();
        double lapse = chrono::duration_cast<chrono::milliseconds>(stop_time-start_time).count();
        lapse *= 1e-6;
        totalTraverseTime += lapse;
    }//Inorder Driver Function

    //To traverse after insertion/deletion
    void traverseInorder(Node *node, ofstream &outstream){
        if(node == NULL) return;
        outstream<<node->getKey();
        if(node->isLeft() || node->isRight()) outstream<<"(";
        if(node->isLeft()){
            traverseInorder(node->getLeft(), outstream);
            if(!node->isRight()) outstream<<",";
        }

        if(node->isRight()){
            outstream<<",";
            traverseInorder(node->getRight(), outstream);
        }
        if(node->isLeft() || node->isRight()) outstream<<")";
    }//Traverse Order Function

    //Function Generator based on Operation code and Parameter
    void functionGenerator(char operation, int param, ofstream &outstream){
        if(operation == 'I') Insert(param, outstream);

        if(operation == 'D') Delete(param, outstream);

        if(operation == 'T'){ inorderDriver(outstream); outstream<<endl; }

        if(operation == 'F'){
            if(findKey(param)) outstream<<"found"<<endl;
            else outstream<<"not found"<<endl;
        }
    }//Function
};

int main(void){
    ifstream instream("in.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("out_avl.txt");
    ofstream timestream("report_avl.txt");
    AVLtree myTree;

    char operation;
    int param = 0;
    while(!instream.eof()){
        instream>>operation;
        if(operation != 'T') instream>>param;

        if(!instream.eof())
            myTree.functionGenerator(operation, param, outstream);
    }

    myTree.reportAVL(timestream);
    instream.close();
    outstream.close(); timestream.close();
    return 0;
}


//Adelson-Velsky-Landis
//g++ -o avl avlTree.cpp