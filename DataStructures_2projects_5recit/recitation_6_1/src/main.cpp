/* @Author
Student Name: Yasmeen Abdelghany
Student ID : 150190915
Date: 04.01.2022 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <iomanip>

using namespace std;

string file_path;

bool perform_operation(char);
void print_menu();
void listProducts();
void listDay();

int main(int argc, char *argv[])
{
    bool end = false;
    char choice;
    ifstream inFile;

    file_path = argv[1];

    inFile.open(file_path, ios::in);
    if (!inFile.is_open())
    {
        cerr << "File cannot be opened";
        return EXIT_FAILURE;
    }

    while (!end)
    {
        print_menu();
        cin >> choice;
        end = perform_operation(choice);
    }

    inFile.close();
    return EXIT_SUCCESS;
}

void print_menu()
{
    cout << "Choose an operation" << endl;
    cout << "L: List 5 products with the highest revenue for total" << endl;
    cout << "E: Exit" << endl;
    cout << "Enter a choice {L, E}: ";
}

bool perform_operation(char choice)
{
    bool terminate = false;
    string strday;

    switch (choice)
    {
    case 'L':
    case 'l':
        listProducts();
        break;
    case 'E':
    case 'e':
        terminate = true;
        break;
    default:
        cout << "Error: You have entered an invalid choice" << endl;
        cin >> choice;
        terminate = perform_operation(choice);
        break;
    }
    return terminate;
}

void listProducts()
{
    ifstream orders_file(file_path.c_str());
    string file_data = "";
    string product = "";
    string price = "";
    string quantity = "";
    string day = "";
    double price_value = 0.0;
    double quantity_value = 0.0;
    int count = 0;
    map<string, double> orders_map;

    if (orders_file.is_open())
    {
        while (!orders_file.eof())  //reads file line by line and copies each word to its own string then changes price and quantity to double data type
        {
            orders_file >> product >> price >> quantity >> day;
            if (count != 0)
            {
                stringstream num(price);
                num >> price_value;
                stringstream num1(quantity);
                num1 >> quantity_value;
                orders_map[product] += price_value * quantity_value;
            }
            ++count;
        }
        orders_file.close();
    }
    else
    {
        cerr << "File cannot be opened";
        exit(1);
    }

    orders_file.close();

    multimap<double, string> ordered_orders;
    map<string, double>::iterator it;

    for (it = orders_map.begin(); it != orders_map.end(); it++)
    {
        ordered_orders.insert(make_pair(it->second, it->first));    //copies orders_map into multimap<double, string> ordered_orders
    }

    multimap<double, string>::reverse_iterator inv_it;

    cout << "5 products with the highest revenue for total are:" << endl;
    for (inv_it = ordered_orders.rbegin(), count = 0; count < 5; inv_it++, count++)
    {
        cout << inv_it->second << " " << fixed << setprecision(2) << inv_it->first << endl; //prints the last 5 elements in the multimap
    }
}