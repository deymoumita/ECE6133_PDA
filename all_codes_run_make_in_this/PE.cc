//Created by Anirudha Kurhade [anirudha@gatech.edu]
#ifndef PE_CC
#define PE_CC

#include <iostream> // debugging
#include <fstream>
#include "PE.h"
#include <string>
#include <cstring>
#include <algorithm>
#include <typeinfo>
#include <limits>
#include <cmath>
#include <ctime>

using namespace std;
// Constructor Definition

// MD


PE::PE() {
}

PE::PE(PE* rhs) {

    peLength = rhs->PeLength();
    allNetsLength = rhs->AllNetsLength();
    parSaTempInit = rhs->ParSaTempInit();
    parSaTemp = rhs->ParSaTemp();
    parSaTempDelta = rhs->ParSaTempDelta();
    totalHpwl = rhs->TotalHpwl();
    
    operandIds = (int*)malloc(((peLength+1)/2)*sizeof(int));
    memcpy(operandIds, (const int*)rhs->OperandIds(), ((peLength+1)/2)*sizeof(int));

    operatorIds = (int*)malloc(((peLength-1)/2)*sizeof(int));
    memcpy(operatorIds, rhs->OperatorIds(), ((peLength-1)/2)*sizeof(int));

    validityArray = (int*)malloc(peLength*sizeof(int));
    memcpy(validityArray, rhs->ValidityArray(), peLength*sizeof(int));

    pePtr = (Node**)malloc(peLength*sizeof(Node*));

    for(int i=0; i<peLength; i++) {

        pePtr[i] = new Node(rhs->PePtr(i));
           

    }

    
    for(int i=0; i<peLength; i++) {

        if(rhs->PePtr(i)->Parent())
            if(rhs->PePtr(i)->Parent()->Operand())
                pePtr[i]->StoreParent(*(pePtr + operandIds[rhs->PePtr(i)->Parent()->Id()]));
            else
                pePtr[i]->StoreParent(*(pePtr + operatorIds[rhs->PePtr(i)->Parent()->Id()]));
        else
            pePtr[i]->StoreParent(NULL);



        if(rhs->PePtr(i)->Left())
            if(rhs->PePtr(i)->Left()->Operand())
                pePtr[i]->StoreLeft(*(pePtr + operandIds[rhs->PePtr(i)->Left()->Id()]));
            else
                pePtr[i]->StoreLeft(*(pePtr + operatorIds[rhs->PePtr(i)->Left()->Id()]));
        else
            pePtr[i]->StoreLeft(NULL);


        if(rhs->PePtr(i)->Right())
            if(rhs->PePtr(i)->Right()->Operand())
                pePtr[i]->StoreRight(*(pePtr + operandIds[rhs->PePtr(i)->Right()->Id()]));
            else
                pePtr[i]->StoreRight(*(pePtr + operatorIds[rhs->PePtr(i)->Right()->Id()]));
        else
            pePtr[i]->StoreRight(NULL);


    }

    
    //pePtr[0]->headPtr = *(pePtr +  operatorIds[rhs->PePtr(0)->headPtr->Id()]);

    //pePtr[0]->headPtr = pePtr[peLength-1]; 
    //pePtr[0]->xRef = rhs->PePtr(0)->xRef;
    //pePtr[0]->yRef = rhs->PePtr(0)->yRef;



    netPtr = (Net**)malloc(allNetsLength*sizeof(Net*));

    for(int i=0; i<allNetsLength; i++) {
    
        netPtr[i] = new Net(rhs->NetPtr(i));

    }

    for(int i=0; i<allNetsLength; i++) {

        for(int j=0; j<rhs->NetPtr(i)->Size(); j++)

            netPtr[i]->StoreModule(pePtr[operandIds[rhs->NetPtr(i)->Modules(j)->Id()]]);
    
    }

    
    for(int i=0; i<allNetsLength; i++) {

        for(int j=0; j<rhs->NetPtr(i)->Size(); j++)

            netPtr[i]->Modules(j)->StoreNet(netPtr[i]);

    }



    fastAccessModules = new unordered_map<string, size_t>(); //IMP might not be working

    (*fastAccessModules) = *(rhs->FastAccessModules());

    headPtr = pePtr[peLength-1];
}

PE::PE(size_t n, size_t m) {

    pePtr = (Node**)malloc(m*sizeof(Node*));

    for(int i=0; i<m; i++) {

        pePtr[i] = new Node();

    }

    peLength = m;

    operandIds = (int*)malloc(((m+1)/2)*sizeof(int));
    operatorIds = (int*)malloc(((m-1)/2)*sizeof(int));
    validityArray = (int*)malloc(m*sizeof(int));

    netPtr = (Net**)malloc(n*sizeof(Net*));

    for(int i=0; i<n; i++) {
    
        netPtr[i] = new Net();

    }

    allNetsLength = n;

    fastAccessModules = new unordered_map<string, size_t>();

}

// Destructor Definition

PE::~PE() {

    for(int i=0; i<peLength; i++) {

        delete pePtr[i];
    
    }

    peLength = 0;


    free(pePtr);

    free(operandIds);
    free(operatorIds);
    free(validityArray);

     
    for(int i=0; i<allNetsLength; i++) {
    
        delete netPtr[i];
    
    }
    allNetsLength = 0;



    free(netPtr);

    delete fastAccessModules;
}


PE& PE::operator=(PE& rhs) {


    peLength = (&rhs)->PeLength();
    allNetsLength = (&rhs)->AllNetsLength();
    parSaTempInit = (&rhs)->ParSaTempInit();
    parSaTemp = (&rhs)->ParSaTemp();
    parSaTempDelta = (&rhs)->ParSaTempDelta();
    totalHpwl = (&rhs)->TotalHpwl();
    //cout << "Anirudha:  " << peLength;
    
    memcpy(operandIds, (&rhs)->OperandIds(), ((peLength+1)/2)*sizeof(int));

    memcpy(operatorIds, (&rhs)->OperatorIds(), ((peLength-1)/2)*sizeof(int));

    memcpy(validityArray, (&rhs)->ValidityArray(), peLength*sizeof(int));


    for(int i=0; i<peLength; i++) {

        delete pePtr[i];
    
    }

    free(pePtr);


    pePtr = (Node**)malloc(peLength*sizeof(Node*));
    for(int i=0; i<peLength; i++) {

        //cout << "\niofcghod: " << i << endl;
        pePtr[i] = new Node((&rhs)->PePtr(i));
           

    }

    for(int i=0; i<peLength; i++) {

        if((&rhs)->PePtr(i)->Parent())
            if((&rhs)->PePtr(i)->Parent()->Operand())
                pePtr[i]->StoreParent(*(pePtr + operandIds[(&rhs)->PePtr(i)->Parent()->Id()]));
            else
                pePtr[i]->StoreParent(*(pePtr + operatorIds[(&rhs)->PePtr(i)->Parent()->Id()]));
        else
            pePtr[i]->StoreParent(NULL);



        if((&rhs)->PePtr(i)->Left())
            if((&rhs)->PePtr(i)->Left()->Operand())
                pePtr[i]->StoreLeft(*(pePtr + operandIds[(&rhs)->PePtr(i)->Left()->Id()]));
            else
                pePtr[i]->StoreLeft(*(pePtr + operatorIds[(&rhs)->PePtr(i)->Left()->Id()]));
        else
            pePtr[i]->StoreLeft(NULL);


        if((&rhs)->PePtr(i)->Right())
            if((&rhs)->PePtr(i)->Right()->Operand())
                pePtr[i]->StoreRight(*(pePtr + operandIds[(&rhs)->PePtr(i)->Right()->Id()]));
            else
                pePtr[i]->StoreRight(*(pePtr + operatorIds[(&rhs)->PePtr(i)->Right()->Id()]));
        else
            pePtr[i]->StoreRight(NULL);


    }
    

    for(int i=0; i<allNetsLength; i++) {
    
        delete netPtr[i];
    
    }

    free(netPtr);
    netPtr = (Net**)malloc(allNetsLength*sizeof(Net*));
    for(int i=0; i<allNetsLength; i++) {
    
        netPtr[i] = new Net((&rhs)->NetPtr(i));

    }
    
    for(int i=0; i<allNetsLength; i++) {

        for(int j=0; j<(&rhs)->NetPtr(i)->Size(); j++)

            netPtr[i]->StoreModule(pePtr[operandIds[(&rhs)->NetPtr(i)->Modules(j)->Id()]]);
    
    }

    
    for(int i=0; i<allNetsLength; i++) {

        for(int j=0; j<(&rhs)->NetPtr(i)->Size(); j++)

            netPtr[i]->Modules(j)->StoreNet(netPtr[i]);

    }
    
    
    //pePtr[0]->headPtr = *(pePtr +  operatorIds[(&rhs)->PePtr(0)->headPtr->Id()]);

    headPtr = pePtr[peLength-1];
    
    
    delete fastAccessModules;
    fastAccessModules = new unordered_map<string, size_t>();
    (*fastAccessModules) = *((&rhs)->FastAccessModules());

    return (*this);
}

// Access Functions

Node** PE::PePtr() {

    return(pePtr);

}

Node* PE::PePtr(size_t i) {

    return(pePtr[i]);

}


Net** PE::NetPtr() {

    return(netPtr);

}

Net* PE::NetPtr(size_t i) {

    return(netPtr[i]);

}

size_t PE::PeLength() {

    return(peLength);

}

size_t PE::AllNetsLength() {

    return(allNetsLength);

}

unordered_map<string, size_t>* PE::FastAccessModules() {

    return(fastAccessModules);

}

int* PE::OperandIds() {

    return(operandIds);

}

int* PE::OperatorIds() {

    return(operatorIds);

}

int* PE::ValidityArray()  {

    return(validityArray);

}



float PE::ParSaTempInit()  {

    return(parSaTempInit);

}

float PE::ParSaTemp()  {

    return(parSaTemp);

}

float PE::ParSaTempDelta()  {

    return(parSaTempDelta);

}

Node* PE::HeadPtr() {

    return(headPtr);

}

float PE::TotalHpwl() {

    return(totalHpwl);

}

float PE::TotalArea() {

    return(pePtr[0]->headPtr->Width() * pePtr[0]->headPtr->Height());

}

float PE::TotalAreaOccupied() {
    float totalareaoccupied = 0;
    for(int i = 0; i < peLength; i++)
    {
        if(NodeAt(i)->Operand())  // if it is an operand
        {
            totalareaoccupied += NodeAt(i)->Width() * NodeAt(i)->Height();        }
        else
        {
        
        }   
    }
    return (totalareaoccupied);

}



// Store Functions

void PE::StorePeLength(size_t n) {

    peLength = n;

}

void PE::StoreAllNetsLength(size_t n) {

    allNetsLength = n;

}

                                                                

// Return Function

Node* PE::NodeAt(size_t i) {
    
    return (pePtr[i]);

}

Net* PE::NetAt(size_t i) {
    
    return (netPtr[i]);

}

//Functions


void PE::CreateTree() {

    int i;
    Node* prevPtr;
    Node* indexPtr;
    
    pePtr[peLength-1]->headPtr = pePtr[peLength-1];
    operatorIds[pePtr[peLength-1]->Id()] = peLength-1;

    //cout << pePtr[peLength-1]->Value();

    //cout << pePtr[0]->headPtr->Value();
    prevPtr = pePtr[peLength-1]->headPtr;

    for (i=peLength-2; i>=0; i--) {
        
        indexPtr = prevPtr;

       
        while(indexPtr->Operand() || (indexPtr->Right() && indexPtr->Left())) {

            indexPtr = indexPtr->Parent();

        }


        //cout << "Anirudha: " << i << ": " << pePtr[i]->Operand() << endl;

        pePtr[i]->StoreParent(indexPtr);
        
        if(!indexPtr->Right()) 
            indexPtr->StoreRight(pePtr[i]);
        else 
            indexPtr->StoreLeft(pePtr[i]);

        if(pePtr[i]->Operand()) operandIds[pePtr[i]->Id()] = i;
        else operatorIds[pePtr[i]->Id()] = i;


        prevPtr = pePtr[i];
        

    }

    int sum = 0;

    for(i=0; i<peLength; i++) {

        if(!pePtr[i]->Operand()) 
            validityArray[i] = (++sum);
        else
            validityArray[i] = sum;


    }

    headPtr = pePtr[0]->headPtr;

}


//////////////////////////////////////////////////////////////////////////////
// PE::InitializeDimensions
// Initializes x, y, width, height, centroidX, centroidY
// Uses Tree paradigm. So CreateTree() needs to be run before this.
/////////////////////////////////////////////////////////////////////////////
void PE::InitializeDimensions(Node* index, Node* stopNode, int *flag) {

    if(index == NULL) return;
    if(index == stopNode) {
    
        //cout << "\nAnirudha: wopc " << stopNode->Value() << endl;
        *flag = 1;   
        return;

    }


    InitializeDimensions(index->Left(), stopNode, flag);
    InitializeDimensions(index->Right(), stopNode, flag);

    if(!(*flag)) return;

    if(index->Operand()) {

        index->StoreX(index->xRef);
        index->StoreY(index->yRef);     
        index->ComputeCentroid();

            //if(!strcmp(index->Value(),"4")) cout << "\nAnirudha: poklulu " << index->Parent()->Left()->Value() << endl; 
        if(index->Parent()->Left() == index) {


            if(index->Parent()->IsVertical()) {
                //if(!strcmp(index->Value(),"4")) cout << "\nAnirudha: ifgfpa\n" << endl; 
                index->xRef = index->xRef + index->Width();
            }

            if(index->Parent()->IsHorizontal()) {


             //   if(!strcmp(index->Value(),"4")) cout << "\nAnirudha: oopsfbg\n" << endl; 
                index->yRef = index->yRef + index->Height();

            }
        }

    } else {

        if(index->IsVertical()) {

            index->StoreX(index->Left()->X());
            //index->StoreY(min(index->Left()->Y(), index->Right()->Y()));
            index->StoreY(index->Left()->Y());
            index->StoreWidth(index->Left()->Width() + index->Right()->Width());
            index->StoreHeight(max(index->Left()->Height(), index->Right()->Height()));
            index->ComputeCentroid();


        } else {

            //index->StoreX(min())
            index->StoreX(index->Left()->X());
            index->StoreY(index->Left()->Y());
            index->StoreWidth(max(index->Left()->Width(), index->Right()->Width()));
            index->StoreHeight(index->Left()->Height() + index->Right()->Height());
            index->ComputeCentroid();

        }

        if(index->Parent()->IsHorizontal()) {
          
            index->xRef = index->X();
            index->yRef = index->Y() + index->Height();

        } else {

            index->xRef = index->X() + index->Width();
            index->yRef = index->Y();

        }
    }

 //   if(!strcmp(index->Value(),"4")) cout << "\nAnirudha: biap " << index->xRef << " " << index->yRef << endl;
}



void PE::InitializeDimensions() {

    int* flag;

    flag = (int*)malloc(sizeof(int));
    *flag = 1;
    
    InitializeDimensions(pePtr[0]->headPtr->Left(), NULL, flag);
    InitializeDimensions(pePtr[0]->headPtr->Right(), NULL, flag);


    if(pePtr[0]->headPtr->IsVertical()) {

        pePtr[0]->headPtr->StoreX(pePtr[0]->headPtr->Left()->X());
        pePtr[0]->headPtr->StoreY(min(pePtr[0]->headPtr->Left()->Y(), pePtr[0]->headPtr->Right()->Y()));
        pePtr[0]->headPtr->StoreWidth(pePtr[0]->headPtr->Left()->Width() + pePtr[0]->headPtr->Right()->Width());
        pePtr[0]->headPtr->StoreHeight(max(pePtr[0]->headPtr->Left()->Height(), pePtr[0]->headPtr->Right()->Height()));
        pePtr[0]->headPtr->ComputeCentroid();


    } else {

        pePtr[0]->headPtr->StoreX(pePtr[0]->headPtr->Left()->X());
        pePtr[0]->headPtr->StoreY(pePtr[0]->headPtr->Left()->Y());
        pePtr[0]->headPtr->StoreWidth(max(pePtr[0]->headPtr->Left()->Width(), pePtr[0]->headPtr->Right()->Width()));
        pePtr[0]->headPtr->StoreHeight(pePtr[0]->headPtr->Left()->Height() + pePtr[0]->headPtr->Right()->Height());
        pePtr[0]->headPtr->ComputeCentroid();

    }

    free(flag);
}


void PE::UpdateDimensions(Node* stopNode) {

    int* flag;

    flag = (int*)malloc(sizeof(int));
    *flag = 0;

    if(!(pePtr[0]->headPtr->Right() == stopNode)) {

        InitializeDimensions(pePtr[0]->headPtr->Left(), stopNode, flag);
        InitializeDimensions(pePtr[0]->headPtr->Right(), stopNode, flag);

    }

    if(pePtr[0]->headPtr->IsVertical()) {

        pePtr[0]->headPtr->StoreX(pePtr[0]->headPtr->Left()->X());
        pePtr[0]->headPtr->StoreY(min(pePtr[0]->headPtr->Left()->Y(), pePtr[0]->headPtr->Right()->Y()));
        pePtr[0]->headPtr->StoreWidth(pePtr[0]->headPtr->Left()->Width() + pePtr[0]->headPtr->Right()->Width());
        pePtr[0]->headPtr->StoreHeight(max(pePtr[0]->headPtr->Left()->Height(), pePtr[0]->headPtr->Right()->Height()));
        pePtr[0]->headPtr->ComputeCentroid();


    } else {

        pePtr[0]->headPtr->StoreX(pePtr[0]->headPtr->Left()->X());
        pePtr[0]->headPtr->StoreY(pePtr[0]->headPtr->Left()->Y());
        pePtr[0]->headPtr->StoreWidth(max(pePtr[0]->headPtr->Left()->Width(), pePtr[0]->headPtr->Right()->Width()));
        pePtr[0]->headPtr->StoreHeight(pePtr[0]->headPtr->Left()->Height() + pePtr[0]->headPtr->Right()->Height());
        pePtr[0]->headPtr->ComputeCentroid();

    }


    free(flag);

}


void PE::CreateFastAccessModule() {

    int i;

    for(i=0; i<peLength; i++) {

        if(pePtr[i]->Operand()) {
            (*fastAccessModules)[pePtr[i]->Value()] = i;

            //cout << "\nAnirudha " << i << "\t" << pePtr[i] << "\t"; 
            //cout << fastAccessModules->find(pePtr[i]->Value())->second << endl;
        }
    }

}

void PE::UpdateNetlist(size_t i, const char* st) {

    /*cout << "Anirudha:\n";
    cout << st;
    if (GetFastAccessModule()->find(st) != GetFastAccessModule()->end()) cout << "\tFound\n";
    cout << pePtr[GetFastAccessModule()->find(st)->second]->Value() << endl;
    */
    NetAt(i)->StoreModule(pePtr[GetFastAccessModule()->find(st)->second]);
    (pePtr[GetFastAccessModule()->find(st)->second])->StoreNet(NetAt(i));

}

void PE::ComputeHpwl() {

    int i, j;
    float xmin, xmax, ymin, ymax;

    for (i=0; i<allNetsLength; i++) {

        xmin = ymin = numeric_limits<float>::max();
        xmax = ymax = numeric_limits<float>::min();
        for (j=0; j<netPtr[i]->Size(); j++) {

            //cout << "\nAnirudha " << i << " "  << j << endl; 
            xmin = min(xmin, netPtr[i]->Modules(j)->CentroidX());
            ymin = min(ymin, netPtr[i]->Modules(j)->CentroidY());
            xmax = max(xmax, netPtr[i]->Modules(j)->CentroidX());
            ymax = max(ymax, netPtr[i]->Modules(j)->CentroidY());

        }

        //cout << "\n" << xmin << "\t" << xmax << "\t" << ymin << "\t" << ymax << endl;
        netPtr[i]->StoreOldHpwl(netPtr[i]->Hpwl());
        netPtr[i]->StoreHpwl(xmax-xmin+ymax-ymin);

    }

}

void PE::InitializeTotalHpwl() {

    int i;

    netPtr[i]->totalHpwl = 0;

    for(i=0; i<allNetsLength; i++) {

        netPtr[i]->totalHpwl += netPtr[i]->Hpwl();

    }

    totalHpwl = netPtr[0]->totalHpwl;

}

// Moves


// #define OLD_MOVE1 
// #define OLD_MOVE2 
// #define OLD_MOVE3
// #define OLD_MOVE4

#ifdef OLD_MOVE1

void PE::Move1(size_t n1, size_t n2) {

    int i;
    Node* temp;

    temp = pePtr[n1];

    pePtr[n1] = pePtr[n2];
    pePtr[n2] = temp;

    pePtr[0]->xRef = 0;
    pePtr[0]->yRef = 0;

    for(i=0; i<peLength; i++) {

        pePtr[i]->StoreRight(NULL);
        pePtr[i]->StoreLeft(NULL);
        pePtr[i]->StoreX(0);
        pePtr[i]->StoreY(0);

    }

    CreateTree();
    InitializeDimensions();
    ComputeHpwl();
    InitializeTotalHpwl();


}


#else

void PE::Move1(size_t n1, size_t n2) {

    int i;
    Node* temp;

    temp = pePtr[n1];

    //cout << "\nAnirudha: pasop " << pePtr[n1]->Value() << " " << pePtr[n2]->Value() << endl;

    if(!min(n1, n2)) {

        pePtr[0]->xRef = 0;
        pePtr[0]->yRef = 0;

    } else {

        if(!strcmp("+",pePtr[min(n1,n2)-1]->Parent()->Value())) {
            pePtr[0]->xRef = pePtr[min(n1, n2)-1]->X();    
            pePtr[0]->yRef = pePtr[min(n1, n2)-1]->Y() + pePtr[min(n1, n2)-1]->Height();
        } else {
            pePtr[0]->xRef = pePtr[min(n1, n2)-1]->X() + pePtr[min(n1, n2)-1]->Width();
            pePtr[0]->yRef = pePtr[min(n1, n2)-1]->Y();    
        }    
   
    }

    //cout << "\nAnirudha: opiq " << pePtr[0]->xRef << " " << pePtr[0]->yRef << endl;

    Node* tempNodePtr;
    
    if(pePtr[n1]->Parent() == pePtr[n2]->Parent()) {

        tempNodePtr = pePtr[n1]->Parent()->Left();
        pePtr[n1]->Parent()->StoreLeft(pePtr[n1]->Parent()->Right());
        pePtr[n1]->Parent()->StoreRight(tempNodePtr);


    } else {

    if(pePtr[n1]->Parent()->Left() == pePtr[n1])
        pePtr[n1]->Parent()->StoreLeft(pePtr[n2]);
    else
        pePtr[n1]->Parent()->StoreRight(pePtr[n2]);

    if(pePtr[n2]->Parent()->Left() == pePtr[n2])
        pePtr[n2]->Parent()->StoreLeft(pePtr[n1]);
    else
        pePtr[n2]->Parent()->StoreRight(pePtr[n1]);

    }

    tempNodePtr = pePtr[n2]->Parent();
    pePtr[n2]->StoreParent(pePtr[n1]->Parent());
    pePtr[n1]->StoreParent(tempNodePtr);

    operandIds[pePtr[n1]->Id()] = n2;
    operandIds[pePtr[n2]->Id()] = n1;
  

    pePtr[n1] = pePtr[n2];
    pePtr[n2] = temp;


    //cout << "\nAnirudha: posap " << pePtr[n1]->Parent()->Left()->Value() << " " << pePtr[n2]->Parent()->Right()->Value() << endl;
    

    //pePtr[0]->xRef = 0;
    //pePtr[0]->yRef = 0;

    
	  // for(i=0; i<peLength; i++) {

	  //     pePtr[i]->StoreRight(NULL);
	  //     pePtr[i]->StoreLeft(NULL);
	  //     //pePtr[i]->StoreX(0);
	  //     //pePtr[i]->StoreY(0);

	  // }
	  
	  // CreateTree();
 
   //cout << "Anirudha: In Move1 new"; 
    //InitializeDimensions();
    if (min(n1, n2) == 0) {

        //pePtr[0]->xRef = 0;
        //pePtr[0]->yRef = 0;
    /*    for(i=0; i<peLength; i++) {
     
            pePtr[i]->StoreRight(NULL);
            pePtr[i]->StoreLeft(NULL);
            pePtr[i]->StoreX(0);
            pePtr[i]->StoreY(0);
     
        }

        CreateTree();
	*/

        //cout << "\nInitializeDimensions()\n";
        InitializeDimensions();
    }
    else {
        //cout << "\nUpdateDimensions()\n";
        UpdateDimensions(pePtr[min(n1, n2)-1]);
    }
    ComputeHpwl();
    InitializeTotalHpwl();


}

#endif


#ifdef OLD_MOVE2

void PE::Move2(size_t n) { 

    int i;

    int ntemp = n;


    do {
        
        if(!strcmp(pePtr[ntemp]->Value(),"*")) 
            pePtr[ntemp]->StoreValue("+");
        else
            pePtr[ntemp]->StoreValue("*");

        
        ntemp = ntemp + 1;

        if(ntemp == peLength) break;

        //cout <<  "\nAnirudha hehehe: " << pePtr[ntemp]->Value() << endl;

    } while (!pePtr[ntemp]->Operand());

    ntemp = n - 1;

    while (!pePtr[ntemp]->Operand()) {

        if(!strcmp(pePtr[ntemp]->Value(),"*")) 
            pePtr[ntemp]->StoreValue("+");
        else
            pePtr[ntemp]->StoreValue("*");

        ntemp = ntemp - 1;

        if(ntemp < 0) break;

    }


    pePtr[0]->xRef = 0;
    pePtr[0]->yRef = 0;

    for(i=0; i<peLength; i++) {

        pePtr[i]->StoreRight(NULL);
        pePtr[i]->StoreLeft(NULL);
        pePtr[i]->StoreX(0);
        pePtr[i]->StoreY(0);

    }

    CreateTree();
    InitializeDimensions();
    ComputeHpwl();
    InitializeTotalHpwl();

    

}


#else



void PE::Move2(size_t n) { 

    int i;

    int ntemp = n;


    do {
        
        if(!strcmp(pePtr[ntemp]->Value(),"*")) 
            pePtr[ntemp]->StoreValue("+");
        else
            pePtr[ntemp]->StoreValue("*");

        
        ntemp = ntemp + 1;

        if(ntemp == peLength) break;

        //cout <<  "\nAnirudha hehehe: " << pePtr[ntemp]->Value() << endl;

    } while (!pePtr[ntemp]->Operand());

    ntemp--;

    Node* tempPtr = pePtr[ntemp];

    tempPtr = tempPtr->Left();



    ntemp = n - 1;

    while (!pePtr[ntemp]->Operand()) {

        if(!strcmp(pePtr[ntemp]->Value(),"*")) 
            pePtr[ntemp]->StoreValue("+");
        else
            pePtr[ntemp]->StoreValue("*");

        ntemp = ntemp - 1;

        if(ntemp < 0) break;

    }

    ntemp++;


    if(!strcmp("+",tempPtr->Parent()->Value())) {

        pePtr[0]->xRef = tempPtr->X();
        pePtr[0]->yRef = tempPtr->Y() + tempPtr->Height();

    } else {

        pePtr[0]->xRef = tempPtr->X() + tempPtr->Width();
        pePtr[0]->yRef = tempPtr->Y();

    }

    

    
   // for(i=0; i<peLength; i++) {

   //     pePtr[i]->StoreRight(NULL);
   //     pePtr[i]->StoreLeft(NULL);
   //     //pePtr[i]->StoreX(0);
   //     //pePtr[i]->StoreY(0);

   // }

   // CreateTree();

    
   // if(operandIds[tempPtr->Id()])
   //     UpdateDimensions(pePtr[operandIds[tempPtr->Id()]-1]);
   // else
   //     InitializeDimensions();
    

    UpdateDimensions(tempPtr);


    ComputeHpwl();
    InitializeTotalHpwl();

    

}

#endif


#ifdef OLD_MOVE3


void PE::Move3(size_t n1) { //IMP: check validity

    int i;
    Node* temp;

    temp = pePtr[n1];

    pePtr[0]->xRef = 0;
    pePtr[0]->yRef = 0;

    //cout << "\nAnirudha: " << n1 << endl;

    pePtr[n1] = pePtr[n1 + 1];
    pePtr[n1 + 1] = temp;


    for(i=0; i<peLength; i++) {

        pePtr[i]->StoreRight(NULL);
        pePtr[i]->StoreLeft(NULL);
        pePtr[i]->StoreX(0);
        pePtr[i]->StoreY(0);

    }

    CreateTree();
    InitializeDimensions();
    ComputeHpwl();
    InitializeTotalHpwl();


}

#else



void PE::Move3(size_t n1) { //IMP: check validity

    int i;
    Node* temp;
    Node* tempNodePtr;
    Node* stopNode;
    temp = pePtr[n1];


    size_t n2 = n1 + 1;

    if(!n1) {

        pePtr[0]->xRef = 0;
        pePtr[0]->yRef = 0;

    } 


    if(pePtr[n1]->Operand()) {

        if(pePtr[n2]->Parent()->Right() == pePtr[n2]) {

            pePtr[n2]->Parent()->StoreRight(pePtr[n1]);
            pePtr[n2]->StoreRight(pePtr[n2]->Left());
            pePtr[n2]->StoreLeft(pePtr[n2]->Parent()->Left());
            pePtr[n2]->Parent()->StoreLeft(pePtr[n2]);
            pePtr[n2]->Left()->StoreParent(pePtr[n2]);
            pePtr[n1]->StoreParent(pePtr[n2]->Parent());
            operandIds[pePtr[n1]->Id()] = n2;
            operatorIds[pePtr[n2]->Id()] = n1;
            validityArray[n1] = validityArray[n2];

            if(n1) {

                if(!strcmp("+",pePtr[n2]->Value())) {
                    pePtr[0]->xRef = pePtr[n2]->Left()->X();    
                    pePtr[0]->yRef = pePtr[n2]->Left()->Y() + pePtr[n2]->Left()->Height();
                } else {
                    pePtr[0]->xRef = pePtr[n2]->Left()->X() + pePtr[n2]->Left()->Width();
                    pePtr[0]->yRef = pePtr[n2]->Left()->Y();    
                }    

                stopNode = pePtr[n2]->Left();
            }

        } else {

            tempNodePtr = pePtr[n2]; 
            while(tempNodePtr == tempNodePtr->Parent()->Left()) tempNodePtr = tempNodePtr->Parent();
            pePtr[n2]->Parent()->StoreLeft(pePtr[n1]);
            pePtr[n2]->StoreRight(pePtr[n2]->Left());
            pePtr[n2]->StoreLeft(tempNodePtr->Parent()->Left());
            tempNodePtr->Parent()->StoreLeft(pePtr[n2]);
            pePtr[n2]->Left()->StoreParent(pePtr[n2]);
            pePtr[n1]->StoreParent(pePtr[n2]->Parent());
            pePtr[n2]->StoreParent(tempNodePtr->Parent());
            operandIds[pePtr[n1]->Id()] = n2;
            operatorIds[pePtr[n2]->Id()] = n1;
            validityArray[n1] = validityArray[n2];

            if(n1) {

                if(!strcmp("+",pePtr[n2]->Value())) {
                    pePtr[0]->xRef = pePtr[n2]->Left()->X();    
                    pePtr[0]->yRef = pePtr[n2]->Left()->Y() + pePtr[n2]->Left()->Height();
                } else {
                    pePtr[0]->xRef = pePtr[n2]->Left()->X() + pePtr[n2]->Left()->Width();
                    pePtr[0]->yRef = pePtr[n2]->Left()->Y();    
                }    

                stopNode = pePtr[n2]->Left();
            }
        }

    } else {

        if (pePtr[n1]->Parent()->Right()->Operand()) {

            if(n1) {

                if(!strcmp("+",pePtr[n1]->Parent()->Value())) {
                    pePtr[0]->xRef = pePtr[n1]->Left()->X();    
                    pePtr[0]->yRef = pePtr[n1]->Left()->Y() + pePtr[n1]->Left()->Height();
                } else {
                    pePtr[0]->xRef = pePtr[n1]->Left()->X() + pePtr[n1]->Left()->Width();
                    pePtr[0]->yRef = pePtr[n1]->Left()->Y();    
                    //cout << "\nAnirudha: tuipo " << pePtr[0]->xRef << " " << pePtr[0]->yRef << endl; 
                }    

                stopNode = pePtr[n1]->Left();
            }
            pePtr[n1]->Parent()->StoreRight(pePtr[n1]);
            pePtr[n1]->Parent()->StoreLeft(pePtr[n1]->Left());
            pePtr[n1]->StoreLeft(pePtr[n1]->Right());
            pePtr[n1]->StoreRight(pePtr[n2]);
            pePtr[n1]->Parent()->Left()->StoreParent(pePtr[n1]->Parent());
            pePtr[n2]->StoreParent(pePtr[n1]);
            operandIds[pePtr[n2]->Id()] = n1;
            operatorIds[pePtr[n1]->Id()] = n2;
            validityArray[n1]--;

        } else {
            
            if(n1) {

                if(!strcmp("+",pePtr[n1]->Parent()->Value())) {
                    pePtr[0]->xRef = pePtr[n1]->Left()->X();    
                    pePtr[0]->yRef = pePtr[n1]->Left()->Y() + pePtr[n1]->Left()->Height();
                } else {
                    pePtr[0]->xRef = pePtr[n1]->Left()->X() + pePtr[n1]->Left()->Width();
                    pePtr[0]->yRef = pePtr[n1]->Left()->Y();    
                }    

                stopNode = pePtr[n1]->Left();
            }
            pePtr[n2]->Parent()->StoreLeft(pePtr[n1]);
            pePtr[n1]->Parent()->StoreLeft(pePtr[n1]->Left());
            pePtr[n1]->StoreLeft(pePtr[n1]->Right());
            pePtr[n1]->StoreRight(pePtr[n2]);
            pePtr[n1]->Parent()->Left()->StoreParent(pePtr[n1]->Parent());
            pePtr[n1]->Left()->StoreParent(pePtr[n1]);
            pePtr[n1]->StoreParent(pePtr[n2]->Parent());
            pePtr[n2]->StoreParent(pePtr[n1]);
            operandIds[pePtr[n2]->Id()] = n1;
            operatorIds[pePtr[n1]->Id()] = n2;
            validityArray[n1]--;

        }
    }



    //cout << "\nAnirudha: " << n1 << endl;

    pePtr[n1] = pePtr[n1 + 1];
    pePtr[n1 + 1] = temp;


   /* for(i=0; i<peLength; i++) {

	//        pePtr[i]->StoreRight(NULL);
	//        pePtr[i]->StoreLeft(NULL);
        pePtr[i]->StoreX(0);
        pePtr[i]->StoreY(0);

    }

	//    CreateTree();
	*/
    if (n1 == 0)
        InitializeDimensions();
    else 
        UpdateDimensions(stopNode);
    ComputeHpwl();
    InitializeTotalHpwl();


}

#endif

#ifdef OLD_MOVE4


// Rotate block
void PE::Move4(size_t n) { // IMP: currently assuming it is only for operands        
    
    int i;
    float temp;

    if(pePtr[n]->Operand()) {

        //cout << "\nAnirudha in Move4: " << pePtr[n]->Value() << endl;

        pePtr[0]->xRef = 0;//pePtr[n]->X();
        pePtr[0]->yRef = 0;//pePtr[n]->Y();


        temp = pePtr[n]->Width();
        pePtr[n]->StoreWidth(pePtr[n]->Height());
        pePtr[n]->StoreHeight(temp);


        for(i=0; i<peLength; i++) {

            pePtr[i]->StoreRight(NULL);
            pePtr[i]->StoreLeft(NULL);
            pePtr[i]->StoreX(0);
            pePtr[i]->StoreY(0);

        }

        CreateTree();

	/*        
        if (n == 0)
            UpdateDimensions(NULL);
        else
            UpdateDimensions(pePtr[n-1]);
	*/  
        InitializeDimensions();
        ComputeHpwl();
        InitializeTotalHpwl();


    }

}




#else


// Rotate block
void PE::Move4(size_t n) { // IMP: currently assuming it is only for operands        
    
    int i;
    float temp;

    if(pePtr[n]->Operand()) {

        pePtr[0]->xRef = pePtr[n]->X();
        pePtr[0]->yRef = pePtr[n]->Y();


        temp = pePtr[n]->Width();
        pePtr[n]->StoreWidth(pePtr[n]->Height());
        pePtr[n]->StoreHeight(temp);

	/*
        for(i=0; i<peLength; i++) {

            pePtr[i]->StoreRight(NULL);
            pePtr[i]->StoreLeft(NULL);
            //pePtr[i]->StoreX(0);
            //pePtr[i]->StoreY(0);

        }

        CreateTree();
	*/
        if (n == 0)
            InitializeDimensions();
        else
            UpdateDimensions(pePtr[n-1]);
        ComputeHpwl();
        InitializeTotalHpwl();

    }

}


#endif

// Algorithms Appendix

int PE::SelectSaMove() {

    float sample;

    //srand(time(NULL));
    sample = rand() % ((int)(parSaTempDelta));

    //srand(time(NULL));
    if(sample < parSaTemp)
        return((int)(rand() % 3 + 1));
    else
        return(4);

}



bool PE::Move3Validity(int &m1) {

    if(m1 == (peLength - 1)) return(false);

    if(!pePtr[m1-1]->Operand() && !pePtr[m1+1]->Operand()) return(false);

    if(!pePtr[m1+1]->Operand())
    
        if(validityArray[m1] < (m1 - validityArray[m1])) {

            m1 = m1 - 1;
            return(true);

        } else 
            return(false);


    if(!pePtr[m1-1]->Operand())
        if(strcmp(pePtr[m1+2]->Value(), pePtr[m1]->Value())) 
            return(true);
        else 
            return(false);

    //srand(time(NULL));
    int prev = (int)(rand() % 2);

    if(prev) {

        if(validityArray[m1] < (m1 - validityArray[m1])) {

            m1 = m1 - 1;
            return(true);

        } else if(strcmp(pePtr[m1+2]->Value(), pePtr[m1]->Value())) 
            return(true);
        else
            return(false);

    } else {

        if(strcmp(pePtr[m1+2]->Value(), pePtr[m1]->Value()))
            return(true);
        else  if(validityArray[m1] < (m1 - validityArray[m1])) { 

            m1 = m1 - 1;
            return(true);

        } else 
            return(false);

    }
}

/*
bool PE::Move3Validity(int &m1) {

    if(m1 == (peLength-1)) if(pePtr[m1-1]->Value() == pePtr[m1]->Value()) return(false);

    if((pePtr[m1-1]->Value() == pePtr[m1]->Value()) || (pePtr[m1+1]->Value() == pePtr[m1]->Value())) return(false);

    return(true);

}
*/


void PE::SelectSaModules(int &move,int &m1,int &m2) {

    float sample;
    int i;

    //srand(time(NULL));
    sample = rand() % ((int)(parSaTempDelta));

    if(sample < parSaTemp) {

        //srand(time(NULL));

        move = (int)(rand() % 3 + 1);

        switch(move) {

            case 3:

                for(i=0; i<peLength; i++) {
                    
                    //srand(time(NULL));
                    m1 = operatorIds[(int) (rand() % (peLength - 1)/2)];
                    //cout << "\nAnirudha: uoperpav " << m1;
                    if(Move3Validity(m1)) break;

                }
            //cout << "\nheahpgiowe\n";
                //cout << "\nAnirudha 0.0.0\n";
                if(i<peLength) {

                    //cout << "\nAnirudha:\n" << "move: " << move << " m1: " << pePtr[m1]->Value() << " m1+1: " << pePtr[m1+1]->Value() << endl;
                    move = 3;
                    break;

                }

            case 2:
                //srand(time(NULL));
                m1 = operatorIds[(int) (rand() % (peLength - 1)/2)];
                //cout << "\nAnirudha:\n" << "move: " << move << " m1: " << pePtr[m1]->Value() << endl;
                move = 2;
                break;


            case 1:
                do {

                    //srand(time(NULL));
                    m1 = operandIds[(int)(rand() % (peLength + 1)/2)];
                    //srand(time(NULL));
                    m2 = operandIds[(int)(rand() % (peLength + 1)/2)];

                } while(m1 == m2);
                //cout << "\nAnirudha:\n" << "move: " << move << " m1: " << pePtr[m1]->Value() << " m2: " << pePtr[m2]->Value() << endl;
                move = 1;
                break;


        }

    } else {
       move = 4; 
       //srand(time(NULL));
       m1 = operandIds[(int)(rand() % (peLength + 1)/2)];
       //cout << "\nAnirudha:\n" << "move: " << move << " m1: " << pePtr[m1]->Value() << endl;
           
    }   


}



float PE::Cost(float tempParSaTempInit, float tempParSaTemp) {

	//cout<<"MD cost: area = "<<(tempParSaTempInit - tempParSaTemp) * pePtr[0]->headPtr->Width() *  pePtr[0]->headPtr->Height()<<" HPWL: "<<(tempParSaTemp)*netPtr[0]->totalHpwl<<" aspect: "<<abs(pePtr[0]->headPtr->Height() - pePtr[0]->headPtr->Width()) * (tempParSaTempInit - tempParSaTemp)<<endl;
    //return((tempParSaTempInit - tempParSaTemp) * pePtr[0]->headPtr->Width() * pePtr[0]->headPtr->Height() +  tempParSaTemp * netPtr[0]->totalHpwl /*+ abs(pePtr[0]->headPtr->Height() - pePtr[0]->headPtr->Width()) * (tempParSaTempInit - tempParSaTemp) * 300000*/);
    return(pePtr[0]->headPtr->Width() * pePtr[0]->headPtr->Height() + netPtr[0]->totalHpwl);
}

float PE::CustomizeStatic() {

    pePtr[0]->headPtr = headPtr;
    netPtr[0]->totalHpwl = totalHpwl;

}


// Algorithms

void PE::Sa() {

    ofstream myfile;
    myfile.open ("polish.txt", ios::app);
    if(myfile.is_open())
    {
        //myfile << "Writing this to a file.\n";
    }


    int move, m1, m2;

    PE* best = new PE(this);
    PE* NE = new PE(this);


    float deltaCost;
    float epsi = 1;
    float r = 0.9;  

    int MT, uphill, N, k, reject;

    float neCost, cost, bestCost;

    MT = uphill = 0;

    k = 10; // IMP parameter

    N = k * (peLength + 1)/2;

    parSaTemp = parSaTempInit = 500; // IMP parameter
 
    parSaTempDelta = parSaTempInit + 0.1 * parSaTempInit;
    float parSaTempDisp = 0;

    do { 

        MT = uphill = reject = 0;

        do {
       
            (*NE) = (*this);

            //cout << "\nAnirudha: goip\n" << this->pePtr[0]->Value();
            //cout << "\nAnirudha: goiperty\n" << NE->pePtr[0]->Value();


            //cout << "\nAnirudha: NE\n" << NE << endl; 
            //cout << "\nAnirudha: this\n" << this << endl;

//            cout << "\nAnirudha:\n";
//            PrintNetlist();//PrintNetsPerModule();//PrintNetlist();
//            NE->PrintNetlist();//PrintNetsPerModule();//PrintNetlist();
//            cout << "\n--------------------------\n";

            SelectSaModules(move, m1, m2);

            //cout << "\nAnirudha 0.0\n";
            NE->CustomizeStatic();
            //cout << "\nokgre: " << NE->Cost();
            switch(move) {

                case 1: NE->Move1(m1, m2);  break;
                case 2: NE->Move2(m1);  break;
                case 3: NE->Move3(m1);  break;
                case 4: NE->Move4(m1);  break;

            }

            MT++;

            neCost = NE->Cost(parSaTempInit, parSaTemp);
            //cout << "\nAnirudha XAX NEcost: " << NE->Cost() << endl;
            
            //best->CustomizeStatic();
            //cout << "\nAnirudha XAX bestcost: " << best->Cost() << endl;
            CustomizeStatic();

            cost = Cost(parSaTempInit, parSaTemp);
            //cout << "\nAnirudha XAX cost: " << cost << endl;
            deltaCost = neCost - cost;

            //NE->CustomizeStatic();
            //NE->PrintAllHpwl();
            CustomizeStatic();
            //PrintAllHpwl();

            //cout << "\nAnirudha: oivrpa limit probability = " << exp(-1*deltaCost/parSaTemp) << endl;  


            //srand(time(NULL));
            if((deltaCost < 0) || ((rand() % 1) < exp(-1*deltaCost/parSaTemp))) {

                if(deltaCost > 0){
                  uphill++;
                  //cout << "\nAnirudha: parvio uphill = " << uphill << endl;  

                } 
                (*this) = (*NE);
                
                // delete //////
                //(*best) = (*NE);
                //cout << "\nfgjhf\n";
                //best->CustomizeStatic();
                //best->PrintPostOrder();
                //cout << "\nfgjhf\n";

                
                /////////////////
                /////////////////////////////////////////////
               // CustomizeStatic();
               // PrintPostOrder();
               // NE->CustomizeStatic();
               // NE->PrintPostOrder();
               // //////////////////////////////////////////
                CustomizeStatic();
               // cout << "\n HJFS: " << this->Cost() << endl;
                cost = Cost(parSaTempInit, parSaTemp);
//                cout << "\nAnirudha 2:\n";
//                PrintNetlist();//PrintNetsPerModule();//PrintNetlist();
//                NE->PrintNetlist();//PrintNetsPerModule();//PrintNetlist();
//                cout << "\n--------------------------\n";


                best->CustomizeStatic();
                bestCost = best->Cost(parSaTempInit, parSaTemp);

                CustomizeStatic();

                //cout << "\nAnirudha cost: " << cost << " bestCost: " << bestCost << endl;
                
                if(cost < bestCost) {
                    //cout << "\nAnirudha Cost= " << Cost() << endl;
                    (*best) = (*this);
                    parSaTempDisp = parSaTemp;
                    best->CustomizeStatic();
                    //best->PrintPostOrder();
                    CustomizeStatic();  
                    
                    
                }
                
            } else
                reject++;


            // if(uphill > N)
            //     cout<<"MD: Uphill exit"<<uphill<<" "<<N<<endl;
            // else if (MT > (2*N))
            //     cout<<"MD: MT exit"<<MT<<endl;

        } while (!((uphill > N) || (MT > (2*N))));

        parSaTemp *= r;

    } while(!(((reject/MT) > 0.95) || (parSaTemp < epsi) || (false/*IMP put timeout condition here*/)) );

    (*this) = (*best);
    CustomizeStatic();
    //cout<<"Best temp: "<<parSaTempDisp<<endl;
}

void PE::Stockmeyer(vector<vector<vector<float>>>& sm, Node* myPtr) {

    if(myPtr->Operand()) return;

    Stockmeyer(sm, myPtr->Left());
    Stockmeyer(sm, myPtr->Right());

    int myId = operatorIds[myPtr->Id()];

    int li, ri;

    if(myPtr->Left()->Operand()) {

        li = operandIds[myPtr->Left()->Id()];

    } else {

        li = operatorIds[myPtr->Left()->Id()];

    }

    if(myPtr->Right()->Operand()) {

        ri = operandIds[myPtr->Right()->Id()];

    } else {

        ri = operatorIds[myPtr->Right()->Id()];

    }

    vector<vector<float>> lVector(sm[li]);
    vector<vector<float>> rVector(sm[ri]);
    vector<vector<float>>::iterator lIt = lVector.begin();
    vector<vector<float>>::iterator rIt = rVector.begin();


    vector<vector<float>> temp1;
    vector<float> temp2;

    temp2.push_back(0);

    int lIndex = 0;
    int rIndex = 0;

    float xl, yl, xr, yr;

    if(myPtr->IsHorizontal()) {

        while(lIt != lVector.end() && rIt != rVector.end()) {


            xl = lVector[lIndex][1];
            yl = lVector[lIndex][2];
            xr = rVector[rIndex][1];
            yr = rVector[rIndex][2];

            if(xl > xr) {

                temp2.push_back(xl);
                lIndex++;
                lIt++;

            } else if(xl < xr) {

                temp2.push_back(xr);
                rIndex++;
                rIt++;

            } else {

                temp2.push_back(xr);
                lIndex++;
                lIt++;
                rIndex++;
                rIt++;

            }


            temp2.push_back(yl+yr);
            temp2.push_back(xl);
            temp2.push_back(yl);
            temp2.push_back(xr);
            temp2.push_back(yr);
            

            temp1.push_back(temp2);

            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();



        }

        

    } else {



        while(lIt != lVector.end() && rIt != rVector.end()) {
            xl = lVector[lIndex][1];
            yl = lVector[lIndex][2];
            xr = rVector[rIndex][1];
            yr = rVector[rIndex][2];

            temp2.push_back(xl+xr);
            if(yl > yr) {

                temp2.push_back(yl);
                lIndex++;
                lIt++;

            } else if(yl < yr) {

                temp2.push_back(yr);
                rIndex++;
                rIt++;

            } else {

                temp2.push_back(yr);
                lIndex++;
                lIt++;
                rIndex++;
                rIt++;

            }

            temp2.push_back(xl);
            temp2.push_back(yl);
            temp2.push_back(xr);
            temp2.push_back(yr);
            

            temp1.push_back(temp2);

            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
        
        }


    }




    sm[myId] = temp1;


}

void PE::Stockmeyer(float x, float y, Node* myPtr, vector<vector<vector<float>>>& sm) {

    if(myPtr == NULL) return;

    int myId;

    if(myPtr->Operand()) 
        myId = operandIds[myPtr->Id()];
    else
        myId = operatorIds[myPtr->Id()];  


    vector<vector<float>> myVector(sm[myId]);     
    vector<vector<float>>::iterator myIt; 
    vector<vector<float>>::iterator bestIt;



    for(myIt = myVector.begin(); myIt != myVector.end(); ++myIt) {

        if(((*myIt)[1]) == x && (*myIt)[2] == y) {
            bestIt = myIt;
            break;


        }

    }

    (*bestIt)[0] = 1;
    myPtr->StoreWidth((*bestIt)[1]);
    myPtr->StoreHeight((*bestIt)[2]);

    if(!myPtr->Operand()) {
        Stockmeyer((*bestIt)[3], (*bestIt)[4], myPtr->Left(), sm);
        Stockmeyer((*bestIt)[5], (*bestIt)[6], myPtr->Right(), sm);
    }





}



void PE::Stockmeyer() {

    vector<vector<vector<float>>> sm;
    int i;


    sm.reserve(peLength);
    //&sm = (vector<vector<vector<float>>>)malloc(peLength * sizeof(vector<vector<vector<float>>>));

    vector<vector<float>> temp1;
    vector<float> temp2;
    for(i=0; i<peLength-1; i++) {
    
        temp2.push_back(0);
        if(pePtr[i]->Parent()->IsVertical()) {
            temp2.push_back(min(pePtr[i]->Width(), pePtr[i]->Height()));
            temp2.push_back(max(pePtr[i]->Height(), pePtr[i]->Width()));
        } else {
            temp2.push_back(max(pePtr[i]->Width(), pePtr[i]->Height()));
            temp2.push_back(min(pePtr[i]->Height(), pePtr[i]->Width()));
        }
        temp1.push_back(temp2);
        temp2.clear();
        temp2.push_back(0);
        if(pePtr[i]->Parent()->IsHorizontal()) {
            temp2.push_back(min(pePtr[i]->Width(), pePtr[i]->Height()));
            temp2.push_back(max(pePtr[i]->Height(), pePtr[i]->Width()));
        } else {
            temp2.push_back(max(pePtr[i]->Width(), pePtr[i]->Height()));
            temp2.push_back(min(pePtr[i]->Height(), pePtr[i]->Width()));
        }
        temp1.push_back(temp2);
        sm.push_back(temp1);

        temp2.clear();

        temp1.clear();

        if(!pePtr[i]->Operand()) sm[i].clear();


    }

    Stockmeyer(sm, headPtr->Left());
    Stockmeyer(sm, headPtr->Right());

    Node* myPtr;
    myPtr = headPtr;

    int myId = peLength - 1; // operatorIds[myPtr->Id()];

    int li, ri;

    if(myPtr->Left()->Operand()) {

        li = operandIds[myPtr->Left()->Id()];

    } else {

        li = operatorIds[myPtr->Left()->Id()];

    }

    if(myPtr->Right()->Operand()) {

        ri = operandIds[myPtr->Right()->Id()];

    } else {

        ri = operatorIds[myPtr->Right()->Id()];

    }

    vector<vector<float>> lVector(sm[li]);
    vector<vector<float>> rVector(sm[ri]);
    vector<vector<float>>::iterator lIt = lVector.begin();
    vector<vector<float>>::iterator rIt = rVector.begin();



    temp2.push_back(0);

    int lIndex = 0;
    int rIndex = 0;

    float xl, yl, xr, yr;

    if(myPtr->IsHorizontal()) {

        while(lIt != lVector.end() && rIt != rVector.end()) {


            xl = lVector[lIndex][1];
            yl = lVector[lIndex][2];
            xr = rVector[rIndex][1];
            yr = rVector[rIndex][2];

            if(xl > xr) {

                temp2.push_back(xl);
                lIndex++;
                lIt++;

            } else if(xl < xr) {

                temp2.push_back(xr);
                rIndex++;
                rIt++;

            } else {

                temp2.push_back(xr);
                lIndex++;
                lIt++;
                rIndex++;
                rIt++;

            }

            temp2.push_back(yl+yr);
            temp2.push_back(xl);
            temp2.push_back(yl);
            temp2.push_back(xr);
            temp2.push_back(yr);
            

            temp1.push_back(temp2);

            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();



        }

        

    } else {



        while(lIt != lVector.end() && rIt != rVector.end()) {
            xl = lVector[lIndex][1];
            yl = lVector[lIndex][2];
            xr = rVector[rIndex][1];
            yr = rVector[rIndex][2];

            temp2.push_back(xl+xr);
            if(yl > yr) {

                temp2.push_back(yl);
                lIndex++;
                lIt++;

            } else if(yl < yr) {

                temp2.push_back(yr);
                rIndex++;
                rIt++;

            } else {

                temp2.push_back(yr);
                lIndex++;
                lIt++;
                rIndex++;
                rIt++;

            }

            temp2.push_back(xl);
            temp2.push_back(yl);
            temp2.push_back(xr);
            temp2.push_back(yr);
            

            temp1.push_back(temp2);

            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
            temp2.pop_back();
        
        }


    }


    sm[myId] = temp1;


    //Preorder for going back and saving dimensions


    vector<vector<float>> headVector(sm[myId]);
    vector<vector<float>>::iterator headIt;
    vector<vector<float>>::iterator bestIt;

    float area = TotalArea();//numeric_limits<float>::max();

    

    for(headIt = headVector.begin(); headIt != headVector.end(); ++headIt) {

        cout << "\nAnirudha: papo "<< ((*headIt)[1])*((*headIt)[2]) << " " << area << endl;
        if((((*headIt)[1])*((*headIt)[2])) < area) {
            bestIt = headIt;
            area = ((*headIt)[1])*((*headIt)[2]);
            //cout << "\nAnirudha: papo "<< area << endl;
        
        }

    }

    if(TotalArea() > area) {

        (*bestIt)[0] = 1;
        headPtr->StoreWidth((*bestIt)[1]);
        headPtr->StoreHeight((*bestIt)[2]);


        Stockmeyer((*bestIt)[3], (*bestIt)[4], headPtr->Left(), sm);
        Stockmeyer((*bestIt)[5], (*bestIt)[6], headPtr->Right(), sm);

        pePtr[0]->xRef = 0;
        pePtr[0]->yRef = 0;


        for(i=0; i<peLength; i++) {

            pePtr[i]->StoreRight(NULL);
            pePtr[i]->StoreLeft(NULL);
            pePtr[i]->StoreX(0);
            pePtr[i]->StoreY(0);

        }

        CreateTree();

    /*        
        if (n == 0)
            UpdateDimensions(NULL);
        else
            UpdateDimensions(pePtr[n-1]);
    */  
        InitializeDimensions();
        ComputeHpwl();
        InitializeTotalHpwl();
 

    }

    vector<float>::iterator tempIt1;

 
}

// Debugging

void PE::PrintPostOrder(Node* index) {

    if(index == NULL) return;

    PrintPostOrder(index->Left());
    PrintPostOrder(index->Right());

    //cout << index->Value() << " ";
    cout << index->Value() << "\tx = " << index->X() << "\ty = " << index->Y() << "\tw = " << index->Width() << "\th = " << index->Height() <<  "\tcentroidX = " << index->CentroidX() << "\tcentroidY = " << index->CentroidY() << "\n";

}

void PE::PrintPostOrder() {

    cout << "\n\n\n";
    cout << "[PE.cc] PE::PrintPostOrder():" << endl;

    PrintPostOrder(pePtr[0]->headPtr->Left());
    PrintPostOrder(pePtr[0]->headPtr->Right());

    //cout << pePtr[0]->headPtr->Value() << endl;
    cout << pePtr[0]->headPtr->Value() << "\tx = " << pePtr[0]->headPtr->X() << "\ty = " << pePtr[0]->headPtr->Y() << "\tw = " << pePtr[0]->headPtr->Width() << "\th = " << pePtr[0]->headPtr->Height() << "\tcentroidX = " << pePtr[0]->headPtr->CentroidX() << "\tcentroidY = " << pePtr[0]->headPtr->CentroidY() << "\n";

    /*
    cout << "\n\nAnirudha:\n\n";
    for (int i=0; i<peLength; i++) {
        cout << validityArray[i] << "\n";
    }
    */
}

void PE::PrintNetlist() {

    int i, j, size;
    cout << "\n\n\n";
    cout << "[PE.cc] PE::PrintNetlist():" << endl;

    for(i=0; i<allNetsLength; i++) {
        size = NetAt(i)->Size();
        cout << NetAt(i)->Value() << "\t";
        for(j=0; j<size; j++) {

            cout << NetAt(i)->Modules(j)->Value() << "\t";

        }

        cout << endl;
    }

}

void PE::PrintNetsPerModule() {

    int i, j, size;
    cout << "\n\n\n";
    cout << "[PE.cc] PE::PrintNetsPerModule():" << endl;

    for(i=0; i<peLength; i++) {
        cout << NodeAt(i)->Value() << "\t";
        if(NodeAt(i)->Operand()) {
            size = NodeAt(i)->TotalNetsConnected();
            for(j=0; j<size; j++) 
                cout << NodeAt(i)->Nets(j)->Value() << "\t";
        }
        
        cout << endl;
    }


}

void PE::PrintAllHpwl() {

    int i;
    cout << "\n\n\n";
    cout << "[PE.cc] PE::PrintAllHpwl():" << endl;
    cout << "\nIndividual HPWLs:\n";
    for(i=0; i<allNetsLength; i++) {

        cout << NetAt(i)->Value() << "\t" << NetAt(i)->Hpwl() << endl;

    }

    cout << "\nTotal HPWL: " << NetAt(0)->totalHpwl << endl << endl;

}

//Retrieve Functions

unordered_map<string, size_t>* PE::GetFastAccessModule() {

    return(fastAccessModules);

}



#endif


