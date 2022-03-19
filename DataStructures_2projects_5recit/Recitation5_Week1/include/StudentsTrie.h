// PLEASE DO NOT REMOVE DECLARED VARIBLES AND FUNCTIONS OF CLASSES, 
// IF YOU PREFER, YOU CAN ADD NEW ONES

/* @Author
Student Name: Student ID : Yasmeen Abdelghany
Student ID : 150190915
Date: 22.Dec.2021 */

#ifndef _H
#define _H

#include <string>

using namespace std;

#define MAX_CHILDREN 10 // Each student ID consists of a sequence of digits: 0-9 

class TrieNode{
        char digit;   // Current Digit
        TrieNode* children[MAX_CHILDREN];   // Next Digit(s) 
        friend class StudentsTrie;

    public:
        TrieNode(const char& digit);
};

class StudentsTrie{
        TrieNode *root;

    public:
        int index = -1;

        StudentsTrie     ( const string& file_name  ); // Construct a StudentsTrie using the records in 'file_name' 
        void insert_id   ( const string& student_id ); // Insert a student ID into the StudentsTrie 
        bool in_the_trie ( const string& student_id ); // Check existence of a student ID in the StudentsTrie
        ~StudentsTrie    (                          ); // StudentsTrie Destructor
};

#endif