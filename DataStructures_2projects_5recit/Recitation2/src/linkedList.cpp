#include <iostream>
#include <stdio.h>
#include <string.h>

#include "linkedList.h"

using namespace std;


void LinkedList::create(){
    head = NULL;
    tail = NULL;
};

void LinkedList::addLetter(char tempChar){    //Node create and assign function
    Node* newNode = new Node;
    newNode->letter = tempChar;
    newNode->next = NULL;

    if(!head){      //If head is null assign node to head
        head = newNode;
        tail = newNode;
    }
    else{           //Assign node to end of the linked list
        tail->next = newNode;
        tail = newNode;   //Update tail
        tail->next = head;
    }
};

void LinkedList::reorderList(){     //Function to reorder nodes
    Node* tempPrev = NULL;
    Node* tempCurrent = head;
    Node* tempNext = head->next;
    Node* tempCurrentTail = tail;

    while(true){        //Traverse in linked list
        if(tempCurrent == head && (tempCurrent->letter > tempNext->letter)){   //Change head node order
            tempCurrent->next = tempNext->next;
            head = tempNext;
            head->next = tempCurrent;

            tempCurrent = head;             //Restart traversing
            tempNext = head->next;
            tail->next = head;
        }   
        else if(tempCurrent->letter > tempNext->letter){        //Change location of nodes other then head
            tempPrev->next = tempNext;
            tempCurrent->next = tempNext->next;
            tempNext->next = tempCurrent;

            if(tempCurrentTail == tempNext){
                tail = tempCurrent;
                tail->next = head;
                tempCurrentTail = tail;
            }
            tempCurrent = head;                 //Restart traversing
            tempNext = head->next;
            tempPrev = NULL; 
        }
        else{               //If no change needed go to next node
            tempPrev = tempCurrent;
            tempCurrent = tempCurrent->next;
            tempNext = tempNext->next;          
        }   
        if(tempNext == head)  //Break loop if traversal ended
            break;
    }
};

void LinkedList::removeDublicates(){   //Function to remove dublicate letters
    Node* tempPrev = NULL;
    Node* tempCurrent = head;
    Node* tempNext = head->next;
    Node* deleteNode;

    while(true){     //Traverse in linked list
        if(tempCurrent == head && (tempCurrent->letter == tempNext->letter)){    //Remove the head node
            deleteNode = head;
            head = tempNext;

            tempCurrent = head;
            tempNext = head->next;
            tail->next = head;
            delete deleteNode;              //Free the memory
        }
        else if(tempCurrent->letter == tempNext->letter){      //Remove node inside the linked list
            deleteNode = tempCurrent;
            tempPrev->next = tempNext;

            tempCurrent = head;
            tempNext = head->next;
            tempPrev = NULL;
            delete deleteNode;       //Free the memory
        }
        else{                 //If no removal needed go to next node
            tempPrev = tempCurrent;
            tempCurrent = tempCurrent->next;
            tempNext = tempNext->next;         
        }   
        if(tempNext == head)     //Break loop if traversal ended
            break;
    }
};

void LinkedList::reverseList(){    //Function to reverse links of the linked list
    Node* tempCurrent = head;
    Node* tempPrev = NULL;
    Node* tempNext = head->next;

    while (true){     //Traverse in linked list
        tempNext = tempCurrent->next;
        tempCurrent->next = tempPrev;
        tempPrev = tempCurrent;
        tempCurrent = tempNext;
        if(tempNext == head)  //Break loop if traversal ended
            break;
    }
    head = tempPrev;
    tail = tempCurrent;    //Update tail
    tail->next = head;     
};

void LinkedList::deleteList(){    //Function for deleting nodes and freeing memory
    Node* deleteHead;
    while(true){        //Traverse in linked list
        if(head == tail)  //Break loop if traversal ended
            break;
        deleteHead = head;
        head = head->next;
        tail->next = head;
        delete deleteHead; //Free the memory
    } 
    deleteHead = head;  
    head = NULL;
    tail = NULL;
    delete deleteHead;    //Delete the last node
}; 

void LinkedList::printList(){
    Node* temp = head;
    while(true){
        cout<<temp->letter<<" ";
        temp = temp->next;
        if(temp == head)
            break;
    }
    cout<<endl;
};