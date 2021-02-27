//Created by Anirudha Kurhade [anirudha@gatech.edu]
#ifndef PE_H
#define PE_H

#include <unistd.h>
#include "Node.h"
#include "Node.cc"

#include <unordered_map>

#define LAMBDA 0.5;

using namespace std;

class PE {

    
    public:

        //Variable

        
        // Constructors
        PE(); // default
        
        PE(PE*); // copy
        
        PE(size_t, size_t); // other, not used often

        // Destructor
        ~PE(); 

        // Overloaded Operators

        PE& operator=(PE&); // this is used to copy one PE object into another already-constructed
        // object

        // Access Functions
        // returns values of private variables.this is done so that in
        // future the variables that need to be protected stay protected.
        // Naming Convention: all functions start with upper case letters and have the same
        // name as the value they return. This makes their use more intuitive when program
        // structure gets complex.
        Node** PePtr();
        Node* PePtr(size_t);
        Net** NetPtr();
        Net* NetPtr(size_t);
        size_t PeLength();
        size_t AllNetsLength();

        unordered_map<string, size_t>* FastAccessModules(); 

        int* OperandIds();
        int* OperatorIds();
        int* ValidityArray();


        float ParSaTempInit();
        float ParSaTemp();
        float ParSaTempDelta();

        Node* HeadPtr();
        float TotalHpwl();
        float TotalArea();
        float TotalAreaOccupied();
        
        // Store Functions
        // These functions store the private variables in the class. this is done so that in
        // future the variables that need to be protected stay protected.
        // Naming Convention: Every word in all functions start with upper case letters
        // and start with the word Store followed by the same name as the value they store.
        // This makes their use more intuitive when program structure gets complex.
        void StorePeLength(size_t);
        void StoreAllNetsLength(size_t);

        // Return Function
        Node* NodeAt(size_t);
        Net* NetAt(size_t);

        // Functions

        void CreateTree();
        void InitializeDimensions(Node*, Node*, int*);
        void InitializeDimensions();
        void UpdateDimensions(Node*);
        void CreateFastAccessModule();

        void UpdateNetlist(size_t, const char*);

        void ComputeHpwl();
        void InitializeTotalHpwl();

        // Moves
        void Move1(size_t, size_t); // swap any two modules
        void Move2(size_t); // complement an operator chain which includes the operator whose 
        // location in Polish expression is passed as an argument
        void Move3(size_t); // swap the elements which is located at the position passed as an
        // argument with either one of its neighbors. which neighbor to select is done randomly 
        void Move4(size_t); // rotate the block by 90 degrees


        // Algorithms Appendix

        int SelectSaMove(); // selects some move among the 4 randomly. it doesn't check the 
        // validity

        // move 1 is always valid
        bool Move2Validity(int&); // as long as operator node is selected this move is always
        // valid, so this was not implemented.
        bool Move3Validity(int&); // check if the move 3 is valid for the node located at
        // location in polish expression which is passed

        void SelectSaModules(int&, int&, int&); // it returns values of move, m1 and m2(if needed)
        // in that order through the variables passed by reference

        float Cost(float, float); // it returns the total cost of whole floorplan considering
        // wirelength and area. values passed were needed before but aren't used now. 

        float CustomizeStatic(); // since the headPtr and totalHpwl variables in the Node and Net
        // classes respectively are static, we need to assign the associated values for these
        // variables for the specific PE class. this is needed when multiple PEs like this, NE 
        // best are simultaneously used. since these variables are common between ALL the
        // instances of Node and Net objects, we need to specify the right values depending 
        // on which PE is used.   

        // Algorithms
        
        void Sa(); // implements the entire algorithm as per the course slides ECE 6133 Georgia
        // Tech

        void Stockmeyer(); // implemented the entire Stockmeyer algorithm
        void Stockmeyer(vector<vector<vector<float>>>&, Node*); // this overloaded function is
        // used to implement the recursion needed for forward traversing in the Stockmeyer
        // Algorithm using Post Order Traversal. first variable passed is a complex 
        // 3D vector according to the report.
        void Stockmeyer(float, float, Node*, vector<vector<vector<float>>>&); // This overloaded
        // function is used for back tracing in algo using Pre Order Traversal

        // Debugging
        void PrintPostOrder(Node*); // print all attributes of pe in post order 
        // (needed for recursion)
        void PrintPostOrder();
        void PrintNetlist(); //Checks modules in Net
        void PrintNetsPerModule(); //Checks all nets per module
        void PrintAllHpwl();


        //Retrieve Functions

        unordered_map<string, size_t>* GetFastAccessModule();
        
     private:

        Node** pePtr; // pointer to array of nodes
        Net** netPtr; // pointer to array of nets
        size_t peLength; // length of pe
        size_t allNetsLength; // total number of nets

        unordered_map<string, size_t>* fastAccessModules; // this is used to access modules(Node)
        // using their value as access variable. their location in Polish Expression is returned. 

        int* operandIds; // location of operand in array operandIds
        int* operatorIds; // location of operators in array operatorIds
        int* validityArray; // keeps track if the ballotting property

        // Parameters

        float parSaTempInit; // initial temperature for SA
        float parSaTemp; // changing temperature for SA
        float parSaTempDelta; // used for making Move 4 probability inversely proportional to
        // temperature

        Node* headPtr; // pointer to the head node in tree (=> last node in pe array)
        float totalHpwl; 

		GaNode** Ga1PE; // not used
		vector<GaNode**> GaPEList; // not used
		vector<float> PE_cost; // total cost



        
        


};



#endif
