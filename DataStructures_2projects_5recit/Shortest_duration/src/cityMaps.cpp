/* @Author
Student Name: Yasmeen Abdelghany
Student ID : 150190915
Date: 30.12.2021 */

#include <fstream>
#include <iostream>
#include <sstream>

#include "cityMaps.h"

using namespace std;

cityNode::cityNode(const string &city)
{
    this->city = city;
    left = NULL;
    right = NULL;
};

string cityNode::get_city(cityNode *node)
{
    return node->city;
};

int cityNode::get_cost(cityNode *node)
{
    return node->total_duration;
};

cityNode *cityNode::get_left(cityNode *node)
{
    return node->left;
};

cityNode *cityNode::get_right(cityNode *node)
{
    return node->right;
};

MapTree::MapTree(const string &file_name)
{
    ifstream map_file(file_name.c_str());
    string file_data;
    string source;
    string destination;
    int duration = 0;

    if (map_file.is_open())
    {
        //first word is the source, the second is the dest, the third is the duration
        for (int i = 1; map_file >> file_data; i++)
        {
            if (i == 1)
            {
                root = new cityNode(file_data); //first word in the file is the root of the tree (the city where friend1/2 lives)
            }
            if (i % 3 == 1)
            {
                source = file_data;
            }
            else if (i % 3 == 2)
            {
                destination = file_data;
            }
            else if (i % 3 == 0)
            {
                stringstream num(file_data); //string => int
                num >> duration;
                add_city(source, destination, duration);
            }
        }
        map_file.close();
    }
    else
    {
        cout << "File: " << file_name << " could NOT be opened!!";
        exit(1);
    }

    map_file.close();
};

cityNode *MapTree::get_root()
{
    return this->root;
};

void MapTree::add_city(const string &source, const string &destination, const int &duration)
{
    cityNode *parentNode = find_city(source, root);     //finds the node of the source city (it must exist in the tree)
    cityNode *childNode = find_city(destination, root); //finds the node of the dest city (if found, links will be pruned. Else add a new node of that city)

    if (childNode == NULL && parentNode->left == NULL)
    {
        parentNode->left = new cityNode(destination);
        parentNode->left->total_duration = duration + parentNode->total_duration; //total duration of a child node is the sum of the durations from the root to that node
    }
    else if (childNode == NULL && parentNode->right == NULL)
    {
        parentNode->right = new cityNode(destination);
        parentNode->right->total_duration = duration + parentNode->total_duration; //total duration of a child node is the sum of the durations from the root to that node
    }
    else
    {
        prune_links(childNode, parentNode, duration); //prune the links only if there are more than one path to the same city
    }
};

void MapTree::prune_links(cityNode *childNode, cityNode *parentNode, const int &duration)
{
    cityNode *parent2 = find_parent(childNode, root);
    //find which path is of a lower duration cost
    if (childNode->total_duration > duration + parentNode->total_duration)
    {
        if (parentNode->left == NULL)
        {
            parentNode->left = childNode; //update the link
        }
        else
        {
            parentNode->right = childNode; //update the link
        }
        //remove the links with the longer path, updates duration of the shorter path
        childNode->total_duration = duration + parentNode->total_duration;
        if (parent2->left == childNode)
        {
            parent2->left = NULL;
        }
        else if (parent2->right == childNode)
        {
            parent2->right = NULL;
        }
    }
};

cityNode *MapTree::find_parent(cityNode *childNode, cityNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    //if the childNode is on the left or right node of "root" then it is the parent
    if ((root->left != NULL && root->left == childNode) || (root->right != NULL && root->right == childNode))
    {
        return root;
    }
    //traverse through the tree til parent is found (preorder traverse)
    cityNode *parent = find_parent(childNode, root->left);
    if (parent != NULL)
    {
        return parent;
    }
    return find_parent(childNode, root->right);
};

cityNode *MapTree::find_city(const string &city, cityNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    if (node->city == city)
    {
        return node;
    }
    //traverse through the tree til parent is found (preorder traverse)
    cityNode *nodePtr = find_city(city, node->left); //check left nodes
    if (nodePtr != NULL)
    {
        return nodePtr;
    }
    return find_city(city, node->right); //go right if not found on the left
};

void MapTree::preorder_traverse(cityNode *node)
{
    if (node != NULL)
    {
        cout << node->city << " ";
        this->preorder_traverse(node->left);
        this->preorder_traverse(node->right);
    }
};

void MapTree::destruct_tree()
{
    clear_tree(root);
    root = NULL;
};

void MapTree::clear_tree(cityNode *root)    //preorder traverse to delete tree
{
    if (root != NULL)
    {
        clear_tree(root->left);
        clear_tree(root->right);
        delete root;
    }
};