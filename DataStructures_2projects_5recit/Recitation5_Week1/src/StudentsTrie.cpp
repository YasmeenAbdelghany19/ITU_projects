#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "StudentsTrie.h"

using namespace std;

TrieNode::TrieNode(const char &digit)
{
   this->digit = digit;
   for (int i = 0; i < MAX_CHILDREN; children[i++] = NULL)
      ;
};

// Construct a StudentsTrie using the records in 'file_name'
StudentsTrie::StudentsTrie(const string &file_name)
{
   ifstream student_ids_file(file_name.c_str()); // ifstream object for input file
   string current_student_id;

   root = new TrieNode('r');

   if (student_ids_file.is_open())
   {
      for (int i = 1; student_ids_file >> current_student_id; i++)
      {
         insert_id(current_student_id); // YOU HAVE TO IMPLEMENT insert_id() METHOD BELOW
      }
      student_ids_file.close();
   }
   else
   {
      cout << "File: " << file_name << " could NOT be opened!!";
      exit(1);
   }

   student_ids_file.close();
};

// Insert a student ID into the StudentsTrie
void StudentsTrie::insert_id(const string &student_id)
{
   // IMPLEMENT THIS METHOD
   TrieNode *tmpNode = root;
   for (char d : student_id)
   {
      if (tmpNode->children[0] == NULL)
      {
         tmpNode->children[0] = new TrieNode(d);
         tmpNode = tmpNode->children[0];
      }
      else
      {
         while (++index < MAX_CHILDREN)
         {
            //move tmpNode to the next child, if that next cild has the required digit
            if (tmpNode->children[index] != NULL && tmpNode->children[index]->digit == d)
            {
               tmpNode = tmpNode->children[index];
               index = -1;
               break;
            }
            //add a new TrieNode if there's no child with the current digit
            else if(tmpNode->children[index] == NULL)
            {
               tmpNode->children[index] = new TrieNode(d);
               tmpNode = tmpNode->children[index];
               index = -1;
               break;
            }
         }
         
      }
   }
};

// Check existence of a student ID in the StudentsTrie
bool StudentsTrie::in_the_trie(const string &student_id)
{
   // IMPLEMENT THIS METHOD

   // IN ORDER NOT TO GET COMPILER ERROR, I RETURN A DUMMY VALUE

   TrieNode *tmpNode = root;
   int level = 0;
   for (char d : student_id)
   {
      while (++index < MAX_CHILDREN)
      {
         //if the next number is not in all children of the  current tmpNode, then the next child has NULL value so return false
         if (tmpNode->children[index] == NULL)
         {
            return false;
         }
         else if (tmpNode->children[index]->digit == d)
         {
            tmpNode = tmpNode->children[index];
            index = -1;
            level++;
            break;
         }
         
      }
   }
   return true;
};

// StudentsTrie Destructor
StudentsTrie::~StudentsTrie()
{
   for (int i = 0; i < MAX_CHILDREN; i++)
   {
      if (root->children[i])
         delete root->children[i];
   };
};
