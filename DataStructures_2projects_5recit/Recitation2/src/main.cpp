#include <iostream> 
#include <stdlib.h>
#include <fstream>


#include "linkedList.h"

using namespace std;

int main(int argc, char* argv[]) {
	system("clear");// make this line as comment if you are compiling on Windows
	//system("cls"); // make this line as comment if you are compiling on Linux or Mac

    LinkedList newLinkedList;
    newLinkedList.create();    //Link list initializing

    ifstream file(argv[1], ios::in);

    if(file.is_open()){ //Read from file
        char letter;

        while(file.peek() != EOF){
            file>>letter;
            newLinkedList.addLetter(letter);
        }
    }
    else{
        cout<<"File not found!"<<endl;
    }
    file.close();

    cout<<"Readedd letters in Circular Linked List: ";
    newLinkedList.printList();

    newLinkedList.reorderList();
    cout<<"After reordering: ";
    newLinkedList.printList();

    newLinkedList.removeDublicates();
    cout<<"After removing dublicates: ";
    newLinkedList.printList();
    
    newLinkedList.reverseList();
    cout<<"Reversed Circular list: ";
    newLinkedList.printList();

    newLinkedList.deleteList();
    return EXIT_SUCCESS;
}