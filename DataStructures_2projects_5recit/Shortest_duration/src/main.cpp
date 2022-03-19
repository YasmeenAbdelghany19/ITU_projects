/* @Author
Student Name: Yasmeen Abdelghany
Student ID : 150190915
Date: 30.12.2021 */

//g++ -Wall src/main.cpp src/cityMaps.cpp -I include -o blg223e_hw2
//./blg223e_hw2  map1_friend1.txt  map1_friend2.txt

#include <iostream>
#include <stdio.h>
#include <map>

#include "cityMaps.h"

using namespace std;

void print_preorder(MapTree friend_1, MapTree friend_2);
void print_meeting_point(MapTree friend_1, MapTree friend_2);
void find_meeting_point(cityNode *node, multimap<int, string> &meetPoint, MapTree friend_1, MapTree friend_2);

int main(int argc, char *argv[])
{
    system("cls");

    MapTree friend_1(argv[1]);  //constructs the tree of friend1 map
    MapTree friend_2(argv[2]);  //constructs the tree of friend2 map

    print_preorder(friend_1, friend_2);         //printing the tree using preorder traverse
    print_meeting_point(friend_1, friend_2);    //finds the meeting point with the lowest duration and prints it

    friend_1.destruct_tree();   //deletes tree
    friend_2.destruct_tree();   //deletes tree

    return EXIT_SUCCESS;
}

void print_preorder(MapTree friend_1, MapTree friend_2)
{
    cout << "FRIEND-1: ";
    friend_1.preorder_traverse(friend_1.get_root());
    cout << endl;
    cout << "FRIEND-2: ";
    friend_2.preorder_traverse(friend_2.get_root());
    cout << endl;
}

void find_meeting_point(cityNode *friend1_city, multimap<int, string> &meetPoint, MapTree friend_1, MapTree friend_2)
{
    //check if the city in map1 tree exists in map2 tree
    cityNode *friend2_city = friend_2.find_city(friend1_city->get_city(friend1_city), friend_2.get_root());
    if (friend2_city != NULL)
    {
        //if the city exists in both trees, insert it into the multimap (ordered)
        meetPoint.insert(pair<int, string>(friend2_city->get_cost(friend2_city) + friend1_city->get_cost(friend1_city), friend2_city->get_city(friend2_city)));
    }
    //traverse through all the tree
    if (friend1_city->get_left(friend1_city))
    {
        find_meeting_point(friend1_city->get_left(friend1_city), meetPoint, friend_1, friend_2);
    }
    if (friend1_city->get_right(friend1_city))
    {
        find_meeting_point(friend1_city->get_right(friend1_city), meetPoint, friend_1, friend_2);
    }
}

void print_meeting_point(MapTree friend_1, MapTree friend_2)
{
    multimap<int, string> meetingPoint;

    cityNode *rootOfFriend1 = friend_1.get_root();
    find_meeting_point(rootOfFriend1, meetingPoint, friend_1, friend_2);  //makes the multimap

    auto it = meetingPoint.begin();
    cout << "MEETING POINT: " << it->second << endl;
    cout << "TOTAL DURATION COST: " << it->first << endl;
}