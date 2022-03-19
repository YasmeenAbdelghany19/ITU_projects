/* @Author
Student Name: <Yasmeen Abdelghany>
Student ID : <150190915>
Date: <10.11.2021> */

#ifndef _H
#define _H

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class materialNode{
    public:
        char id;
        int weight;
        materialNode* next;
};

class wagonNode{
    public:
        int wagonId;
        materialNode* material;
        wagonNode* next;
};

class Train{  

    wagonNode* head;

    public:
        void create();
        ~Train(); //delete every wagon and every material in each wagon
        void addMaterial(char,int); 
        void deleteFromWagon(char, int);
        void addFirstWagonMat(wagonNode*, char, int); //adds first wagon in train
        void addNewMaterialNode(wagonNode*, materialNode*, char, int); //adds a new material node if doesn't exist
        void addWagon(int, char); //adds a new wagon in the train
        void deleteFromPrevWagon(wagonNode*, materialNode*, char, int, bool); //deletes weight from the material in the previous wagon if weight >20k
        void removeWagon(wagonNode*); //deleted wagon if it is empty
        void printWagon(); //Prints wagon info
        void orderList(); //orders materials in each wagon A>B>C>D
        bool wagonCapacity(wagonNode*); //determines if there's no material in wagon
        wagonNode *getPrev_wagon(wagonNode*); //gets the node of the previous wagon
        materialNode *getPrev_mat(materialNode*, wagonNode*); //gets the node of the previous material
};

#endif