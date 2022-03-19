/* @Author
Student Name: Student ID : <student id>
Student ID : 150190915
Date: 07.Dec.2021 */

#include <fstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <cmath>
#include "huffman.h"

using namespace std;

int Huffman::findindex(Token *arr, string elem)
{
    // YOU HAVE TO COMPLETE THIS PART
    // THIS FUNCTION RETURNS THE INDEX OF STRING ON TOKEN ARRAY, IF STRING IS NOT IN TOKEN ARRAY, RETURNS -1
    int index = -1;
    for (int i = 0; i < this->token_count; i++)
    {
        if (arr[i].symbol == elem)          //if the array of index i has the symbol = target symbol break and retrun that index
        {
            index = i;
            break;
        }
    }

    return index;
}

void Huffman::find_frequencies()
{
    int index = -1;
    ifstream file_obj; //object to read file
    file_obj.open("../input.txt", ifstream::in);
    char symbol = 0;

    if (!file_obj.is_open())
    {
        file_obj.open("input.txt", ifstream::in);
        if (!file_obj.is_open())
        {
            cerr << "Could not open the file" << endl;
            exit(1);
        }
    }
    while (file_obj.get(symbol))
    {
        string s;
        s.push_back(symbol);
        bool char_found = false;

        for (int i = 0; i <= index; i++)    //if the symbol already exists in the token array, incremenet count
        {
            if (token_array[i].symbol == s.substr(s.length() - 1))    
            {
                token_array[i].count += 1;
                char_found = true;
                break;
            }
        }
        if (char_found == false)    //if it doesn't, increment the indext and add a new symbol in the array
        {
            token_array[++index].symbol = symbol;
            token_array[index].count += 1;
            this->token_count = index + 1;
        }
        // YOU HAVE TO COMPLETE THIS PART
    }
    file_obj.close();
}

void Huffman::sort_array(Token *token_array)
{
    //sort the token array according to their frequency from least to greatest or according to ASCII from least to greatest ... _ comes before D in ASCII code
    for (int i = 0; i < token_count; i++)
    {
        for (int j = i + 1; j < token_count; j++)
        {
            if (token_array[j].count < token_array[i].count)
            {
                swap(token_array[j].symbol, token_array[i].symbol);
                swap(token_array[j].count, token_array[i].count);
            }
            else if (token_array[j].count == token_array[i].count && token_array[j].symbol > token_array[i].symbol)
            {
                swap(token_array[j].symbol, token_array[i].symbol);
                swap(token_array[j].count, token_array[i].count);
            }
        }
    }
}

PriorityQueue::PriorityQueue()
{
    head = NULL;
    token_count = 0;
}

void PriorityQueue::push(Node *newnode)
{
    // YOU HAVE TO COMPLETE THIS PART
    this->token_count++;    //increment every time a newnode is added to the SLL
    Node *tempNode = head;

    if (head == NULL)
    {
        head = newnode;
    }
    else
    {
        while (tempNode->next != NULL)
        {
            tempNode = tempNode->next;
        }
        tempNode->next = newnode; //push every newnode to the end of the SLL
    }
}

Node *PriorityQueue::pop()
{
    // YOU HAVE TO COMPLETE THIS PART
    this->token_count--;  //decrement every time a newnode is removed from the SLL

    if (head == NULL)
    {
        return NULL;
    }

    Node *highest_priority_node = head;     
    head = head->next;          //make the head be the second node in the list and return the first node(highest priority node)
    highest_priority_node->next = NULL;
    return highest_priority_node;
}

void PriorityQueue::sort_queue(Node *last_added_node)
{
    Node *tempNode = head;
    Node *prevNode = NULL;      //the node before the last node

    //gets the node before the last node
    for (int i = 0; i < this->token_count; i++)
    {
        if (i == this->token_count - 2)
        {
            prevNode = tempNode;    
            break;
        }
        tempNode = tempNode->next;
    }

    tempNode = head;
    //sorts the SLL in ascending order
    if (last_added_node->token.count < head->token.count)
    {
        last_added_node->next = head;
        head = last_added_node;
        prevNode->next = NULL;
    }
    while (tempNode->next != NULL && tempNode->next != last_added_node)
    {
        if (last_added_node->token.count < tempNode->next->token.count)
        {
            last_added_node->next = tempNode->next;
            tempNode->next = last_added_node;
            prevNode->next = NULL;
            break;
        }
        else
        {
            tempNode = tempNode->next;
        }
    }
}

void Huffman::get_priority_queue()
{
    // YOU HAVE TO COMPLETE THIS PART
    sort_array(token_array); //sort the token array before constructing the SLL

    for (int i = 0; i < this->token_count; i++)
    {
        Node *newLeafNode = new Node;   //make a new leaf node for each symbol
        newLeafNode->token = token_array[i];
        newLeafNode->left = NULL;
        newLeafNode->right = NULL;
        newLeafNode->next = NULL;
        this->priority_queue.push(newLeafNode); //add the new leaf node to the SLL
    }
}

HuffmanBinaryTree::HuffmanBinaryTree()
{
    root = NULL;
}

Node *HuffmanBinaryTree::merge(Node *node1, Node *node2)
{
    // YOU HAVE TO COMPLETE THIS PART
    //Create a new internal node with these two nodes and with probability equal to the sum of the two nodes’ probabilities
    Node *parentNode = new Node;
    parentNode->token.symbol = node2->token.symbol + node1->token.symbol;   
    parentNode->token.count = node2->token.count + node1->token.count;     
    parentNode->left = node2;
    parentNode->right = node1;
    return parentNode;
}

void HuffmanBinaryTree::delete_postorder_traversal(Node *traversal)
{
    if (traversal != NULL)
    {
        delete_postorder_traversal(traversal->left);
        delete_postorder_traversal(traversal->right);
        delete traversal;
    }
}

HuffmanBinaryTree::~HuffmanBinaryTree()
{
    delete_postorder_traversal(root);
    cout << "Code with no memory leak ;)" << endl;
    root = NULL;
}

void Huffman::get_huffman_tree()
{
    // YOU HAVE TO COMPLETE THIS PART
    while (this->priority_queue.token_count > 1)
    {
        Node *parentNode = this->huffman_binary_tree.merge(this->priority_queue.pop(), this->priority_queue.pop()); //get the parent node of the two nodes with highest priority
        this->priority_queue.push(parentNode);      //add the parent node to the SLL
        this->priority_queue.sort_queue(parentNode);  //sort the SLL after addding each parent node
    }
    this->huffman_binary_tree.root = this->priority_queue.head; //The remaining node is the root node
}

void Huffman::get_codes(Node *traversal, string codepart)
{
    // YOU HAVE TO COMPLETE THIS PART
    // A RECURSIVE FUNCTION APPENDS CODEPART STRING 1 OR 0 ACCORDING TO THE HUFFMAN BINARY TREE

    //if the symbol is in the left(right) node concatenate 0(1) to the codepart and call the function recursively till it finds the leaf node with the target symbol.
    if (index < this->token_count)
    {
        if (traversal->left && traversal->left->token.symbol.find(token_array[index].symbol) != string::npos)
        {
            codepart += "0";
            get_codes(traversal->left, codepart);
        }
        if (traversal->right && traversal->right->token.symbol.find(token_array[index].symbol) != string::npos)
        {
            codepart += "1";
            get_codes(traversal->right, codepart);
        }
        else        //leaf node reached
        {       
            token_array[index].code = codepart; //set its code to be = to the codepart
            index++;
            codepart = ""; //reset codepart
            get_codes(this->huffman_binary_tree.root, codepart);
        }
    }
}

string Huffman::return_code(string target)
{
    int index = findindex(token_array, target);
    return token_array[index].code;
}

double Huffman::calculate_compression_ratio()
{
    double bit_required_before = 0;
    for (int i = 0; i < token_count; i++)
        bit_required_before += token_array[i].count * 8;
    double bit_required_after = 0;
    for (int i = 0; i < token_count; i++)
        bit_required_after += token_array[i].count * token_array[i].code.length();
    return bit_required_before / bit_required_after;
}
