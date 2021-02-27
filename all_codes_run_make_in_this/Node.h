//Created by Anirudha Kurhade [anirudha@gatech.edu]
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>

#include <unordered_set>



using namespace std;

class Node;
class Net;

class GaNode;

class Node  {

    public:

        //Variable
        
        static Node* headPtr; //this is static over all Node object instances such that it will accesible to all the Nodes
        static float xRef, yRef; // this is used in InitializeDimensions() and UpdateDimensions() to keep track of the point where the upcoming block should be placed




        // Constructors
        Node(); // default constructor

        Node(Node*); // copy constructor

        Node(const char& st, bool op); // additional constructor [not used often]

        // Destructor
        ~Node(); 

        // Return functions
        // These functions return the private variables to the class. this is done so that in 
        // future the variables that need to be protected stay protected.
        // Naming Convention: all functions start with upper case letters and have the same
        // name as the value they return. This makes their use more intuitive when program
        // structure gets complex.
        Node* Parent();
        Node* Left();
        Node* Right();
        
        bool Operand();
        char* Value();
        
        float Width();
        float Height();
        float X();
        float Y();
        float CentroidX();
        float CentroidY();
        int Id();
        size_t TotalNetsConnected(); // returns the size of vector<Net*> nets
        Net* Nets(size_t); // returns the pointer to Net at some index in vector<Net*> nets
        vector<Net*> Nets(); // returns pointer to vector<Net*> nets
      


        // Store Functions
        // These functions store the private variables in the class. this is done so that in 
        // future the variables that need to be protected stay protected.
        // Naming Convention: Every word in all functions start with upper case letters 
        // and start with the word Store followed by the same name as the value they store. 
        // This makes their use more intuitive when program structure gets complex.
        void StoreParent(Node*);
        void StoreLeft(Node*);
        void StoreRight(Node*);
        
        void StoreOperand(bool);
        void StoreValue(const char*);
        
        void StoreId(size_t);

        void StoreWidth(float);
        void StoreHeight(float);
        void StoreX(float);
        void StoreY(float);
        void StoreCentroidX(float);
        void StoreCentroidY(float);
        void ComputeCentroid();
        void StoreNet(Net*);
        
        // Functions to check operators
        // These can be used only in case of Operators. They return if the operator is 
        // +: Horizontal or *: Vertical. They simply use strcmp. 

        bool IsHorizontal();
        bool IsVertical();

    private:
        Node* parent; // points to parent of current object, headPtr parent in NULL.
        Node* left; // points to left child of object. NULL for operands
        Node* right; // points to right child of object. NULL for operands
        
        bool operand; // operand = 1 if object is an operand
        char* value; // either block name or partition type (+: horizontal, *:vertical)
        
        size_t id; // this is the id of this object in operandIds[] or operatorIds[]
        // depending on which type of object this is. You need to decide whether it is an 
        // operand or operator before using this.

        float width; // width of node. width of operators is the width of the bounding box of 
        // all their children
        float height; // height of node. width of operators is the width of the bounding box of
        // all their children
        float x; // x coordinate. children bounding box considered in case of operators
        float y; // y coordinate. children bounding box considered in case of operators
        float centroidX; // x coordinate of centroid. children bounding box considered in 
        // case of operators
        float centroidY; // y coordinate of centroid. children bounding box considered in case 
        // of operators.

        vector<Net*> nets; // this vector stores pointers to all the objects of nets that 
        // contain this module. since modules are included in variable number of nets, variable
        // length vector data structure is used to avoid unnecessary memory utilization.

        //aspect ratio and area
};

// initialization of static public
Node* Node::headPtr = NULL;
float Node::xRef = 0;
float Node::yRef = 0;

class Net {

    public:
        static float totalHpwl; // //this is static over all Net objects instances such that
        // it will be accessible to all nets

        // Constructors

        Net(); // default 

        Net(Net*); // copy

        // Destructor

        ~Net();

        // Store Functions
        // Stores variables private to this object. similar naming convention as Node class.
        void StoreValue(const char*);
        void StoreHpwl(float);
        void StoreOldHpwl(float);
        void StoreModule(Node*);

        // Retrieve Function
        // returns variables private to object. similar naming convention as Node class
        char* Value();
        float Hpwl();
        float OldHpwl();
        size_t Size();
        Node* Modules(size_t); // returns pointer to Node object at particular index in 
        //  vector<Node*> modules;
        vector<Node*> Modules(); // returns pointer to  vector<Node*> modules;

    private:
        char* value; // net name
        float hpwl; // it's own hpwl
        float oldHpwl; // might have needed but not used
        vector<Node*> modules; // this vector stores pointers to all the objects of nodes that 
        // are connected by this net. since variable number of moduls are connected by the nets, 
        // variable length vector data structure is used to avoid unnecessary memory utilization.
 

};

// initialization of static public
float Net::totalHpwl = 0;



#endif
