/* @Author
Student Name: Yasmeen Abdelghany
Student ID : 150190915
Date: 30.12.2021 */

#ifndef _H
#define _H

#include <string>

using namespace std;

class cityNode
{
private:
    int total_duration = 0;
    string city;
    cityNode *left;
    cityNode *right;
    friend class MapTree;

public:
    cityNode(const string &city);
    string get_city(cityNode *node);
    int get_cost(cityNode *node);
    cityNode *get_left(cityNode *node);
    cityNode *get_right(cityNode *node);
};

class MapTree
{

    cityNode *root;

public:
    MapTree(const string &file_for_friend_x);
    cityNode *get_root();
    void add_city(const string &s, const string &d, const int &duration);  //constructs the tree-like map
    void prune_links(cityNode *s, cityNode *d, const int &duration);       //prunes the excessive links 
    cityNode *find_city(const string &c, cityNode *node);                  //searches for the city node in the tree 
    cityNode *find_parent(cityNode *n, cityNode *r);                       //searches for the parent node 
    void preorder_traverse(cityNode *n);                                   //print the tree in preorder traverse 
    void destruct_tree();                                                  //delete all nodes of the tree (destructor-like) 
    void clear_tree(cityNode *root);
};

#endif