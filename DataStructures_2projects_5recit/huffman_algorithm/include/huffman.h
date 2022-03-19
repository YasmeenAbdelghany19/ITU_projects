// PLEASE DO NOT REMOVE DECLARED VARIBLES AND FUNCTIONS OF CLASSES, IF YOU PREFER, YOU CAN ADD NEW ONES

/* @Author
Student Name: Student ID : <student id>
Student ID : 150190915
Date: 07.Dec.2021 */

#ifndef _H
#define _H

using namespace std;

#define ARRAY_SIZE 100 // This is max length of token array, Do not worry about longer case.
class Token
{
public:
    string symbol = ""; // To store character or symbol
    string code = "";   // To store encoded version of symbol
    int count = 0; // To store frequency of symbol
};

class Node
{
public:
    Token token {}; // To store token
    Node *left = NULL;  // To use in binary tree
    Node *right = NULL; // To use in binary tree
    Node *next = NULL;  // To use in priority queue
};

class PriorityQueue
{
public:
    Node *head = NULL;
    void push(Node *);
    int token_count = 0;
    PriorityQueue();
    Node *pop();
    void sort_queue(Node *);    //It sorts the SLL after merging and adding the two nodes with highest priority
};

class HuffmanBinaryTree
{
public:
    Node *root = NULL;
    HuffmanBinaryTree();
    Node *merge(Node *, Node *);             // To merge two nodes and place the new node conveniently
    void delete_postorder_traversal(Node *); // To release allocated memory
    ~HuffmanBinaryTree();
};

class Huffman
{
public:
    int index = 0;
    int token_count = 0;                   // To store number of token
    Token token_array[ARRAY_SIZE];         // To store tokens that includes symbol, corresponding code and frequency
    PriorityQueue priority_queue {};          // To store priority queue
    HuffmanBinaryTree huffman_binary_tree {}; // To store huffman binary tree
    void find_frequencies();               // It reads symbols from 'input.txt' file and count frequecy of each symbol
    void get_priority_queue();             // It constructs the priority queue
    void get_huffman_tree();               // It constructs the huffman tree
    void get_codes(Node *, string);        // It extract code for each symbol according to the huffman tree
    double calculate_compression_ratio();  // It calculates the compression ratio
    string return_code(string);            // It returns the code of given string
    void sort_array(Token *);              // It sorts the token array in ascending order of the frequecy of each symbol

private:
    int findindex(Token *, string); // It returns the index of given string parameter in Token array
};
#endif