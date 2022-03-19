#ifndef _H
#define _H

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Node{
    public:
        char letter;
        Node* next;
};


class LinkedList{

    Node* head;
    Node* tail;

    public:
        void create();
        void addLetter(char);
        void reorderList();
        void removeDublicates();
        void reverseList();
        void deleteList();
        void printList();
};

#endif