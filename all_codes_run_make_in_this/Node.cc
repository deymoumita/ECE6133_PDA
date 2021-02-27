// Created by Anirudha Kurhade [anirudha@gatech.edu]
#ifndef NODE_CC
#define NODE_CC

#include <iostream>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include "Node.h"

using namespace std;

// Constructor Definitions
// Node::Node() 
// Default Constructor
// All set to default values INCLUDING public variables
Node::Node() {
    value = NULL;
    operand = false;
    parent = NULL;
    left = NULL;
    right = NULL;
    width = 0;
    height = 0;
    id = 0;
    x = 0;
    y = 0;
    centroidX = 0;
    centroidY = 0;

}

Node::Node(Node* rhs) {

    operand = rhs->Operand();
    
    value = (char*)malloc(strlen(rhs->Value()) + 1);
    //if (value) cout << "\nAnirudha: Perfecto!\n";
    
    //cout << "\n>>" << rhs->Value() << endl;

    strcpy(value,(const char*) rhs->Value());

    id = rhs->Id();
    width = rhs->Width();
    height = rhs->Height();
    x = rhs->X();
    y = rhs->Y();
    centroidX = rhs->CentroidX();
    centroidY = rhs->CentroidY();



}


Node::Node(const char& st, bool op) {

//    value = (char*)malloc(strlen(st.c_str())+1);
//    strcpy(value, st.c_str());
    operand = op;
    parent = NULL;
    left = NULL;
    right = NULL;

}

// Destructor Definition

Node::~Node() {

    if (value) free(value);


}

// Return Functions

Node* Node::Parent() {

    return(parent);

}

Node* Node::Left() {

    return(left);

}

Node* Node::Right() {

    return(right);

}

bool Node::Operand() {

    return(operand);

}

char* Node::Value() {

    return(value);

}

float Node::Width() {

    return(width);

}

float Node::Height() {

    return(height);

}

float Node::X() {

    return(x);

}

float Node::Y() {

    return(y);

}

float Node::CentroidX() {

    return(centroidX);

}

float Node::CentroidY() {

    return(centroidY);

}

int Node::Id() {

    return(id);

}

size_t Node::TotalNetsConnected() {

    return(nets.size());

}

Net* Node::Nets(size_t i) {

    return(nets[i]);

}

vector<Net*> Node::Nets() {

    return(nets);

}


// Store Functions

void Node::StoreParent(Node* ptr) {

    parent = ptr;

}

void Node::StoreLeft(Node* ptr) {

    left = ptr;

}

void Node::StoreRight(Node* ptr) {

    right = ptr;

}

void Node::StoreOperand(bool op) {

    operand = op;

}

void Node::StoreValue(const char* st) {


    value = (char*)malloc(strlen(st)+1);
    strcpy(value, st);
//    cout << "In Node.cc: " << value << endl;
}

void Node::StoreId(size_t i) {

    id = i;

}


void Node::StoreWidth(float w) {

    width = w;

}

void Node::StoreHeight(float h) {

    height = h;

}

void Node::StoreX(float xtemp) {

    x = xtemp;

}

void Node::StoreY(float ytemp) {

    y = ytemp;

}

void Node::StoreCentroidX(float centroidXtemp) {

    centroidX = centroidXtemp;

}

void Node::StoreCentroidY(float centroidYtemp) {

    centroidY = centroidYtemp;

}

void Node::ComputeCentroid() {

    StoreCentroidX(x+(width/2));
    StoreCentroidY(y+(height/2));

}



void Node::StoreNet(Net* net) {

    nets.push_back(net);

}

//Functions to check operators

bool Node::IsHorizontal() {

    if(strcmp(Value(), "+") == 0) 
        return (true);
    else
        return (false);

}

bool Node::IsVertical() {

    if(strcmp(Value(), "*") == 0) 
        return (true);
    else
        return (false);

}



// FUNCTIONS OF NET CLASS //////////////////////////////////////

// Constructors

Net::Net() {

    value = NULL;
    hpwl = 0;
    oldHpwl = 0;

}

Net::Net(Net* rhs) {



    value = (char*)malloc(strlen(rhs->Value())+1);
    strcpy(value, rhs->Value());

    hpwl = rhs->Hpwl();
    oldHpwl = rhs->OldHpwl();


}



// Destructors


Net::~Net() {

    if (value) free(value);

}


// Store Functions
 
void Net::StoreValue(const char* st) {


    value = (char*)malloc(strlen(st)+1);
    strcpy(value, st);
    //    cout << "In Node.cc: " << value << endl;
}

void Net::StoreHpwl(float val) {

    hpwl = val;

}

void Net::StoreOldHpwl(float val) {

    oldHpwl = val;

}

void Net::StoreModule(Node* module) {

    modules.push_back(module);


}

// Retrieve Functions

float Net::Hpwl() {

    return(hpwl);

}

float Net::OldHpwl() {

    return(oldHpwl);

}

char* Net::Value() {

        return(value);

}

size_t Net::Size() {

    return(modules.size());

}

Node* Net::Modules(size_t i) {

    return(modules[i]);

}

vector<Node*> Net::Modules() {

    return(modules);

}


#endif
