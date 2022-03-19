/* @Author
Student Name: <Yasmeen Abdelghany>
Student ID : <150190915>
Date: <10.11.2021> */

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "linkedList.h"

using namespace std;

#define MAX_CAPACITY 2000
int id_num = 1;

void Train::create()
{
    head = NULL;
};

Train::~Train()
{
    wagonNode *tempWagon1 = head;
    materialNode *tempMat1 = tempWagon1->material;
    wagonNode *tempWagon2 = head;
    materialNode *tempMat2 = tempWagon1->material;

    while (tempWagon1 != NULL)
    {
        tempWagon2 = tempWagon1->next;
        tempMat1 = tempWagon1->material;
        while (tempMat1 != NULL)
        {
            tempMat2 = tempMat1->next;
            delete tempMat1;
            tempMat1 = tempMat2;
        }
        tempWagon1->material = NULL;
        delete tempWagon1;
        tempWagon1 = tempWagon2;
    }

    head = NULL;
};

/*adds a wagon and the material if the train is empty 
  adds weight to current material if thre's room for it
  adds the extra weight in a new wagon or in the next wagon if there's empty room for it
  adds a new material if the material doesn't exist in the wagon.*/
void Train::addMaterial(char material, int weight)
{
    wagonNode *currentWagon = head;
    int extraWeight = 0;

    if (currentWagon == NULL)
    {
        addFirstWagonMat(currentWagon, material, weight);
    }
    else
    {
        materialNode *current_material = currentWagon->material;

        while (current_material->weight == 0 ||
               current_material->next != NULL ||
               current_material->id == material)
        {
            if (current_material->id == material)
            {
                if (current_material->weight == MAX_CAPACITY)
                {
                    currentWagon = currentWagon->next;
                    current_material = currentWagon->material;
                }
                else if (weight > MAX_CAPACITY - current_material->weight)
                {
                    extraWeight = weight - (MAX_CAPACITY - current_material->weight);
                    current_material->weight = MAX_CAPACITY;
                    if (!currentWagon->next)
                    {
                        addWagon(id_num++, material);
                    }
                    currentWagon = currentWagon->next;
                    current_material = currentWagon->material;
                    weight = extraWeight;
                }
                else
                {
                    current_material->weight += weight;
                    break;
                }
            }
            else
            {
                current_material = current_material->next;
            }
        }
        if (current_material->next == NULL && current_material->id != material)
        {
            addNewMaterialNode(currentWagon, current_material, material, weight);
        }
    }

    orderList();       // orders materials in each wagon A>B>C>D
};

/*deletes weight from lastwagon's material x
  deletes weight from the previous wagon if >2000KG is to be deleted
  deletes material if its weight is 0
  deletes wagon if all materials are deleted
  */
void Train::deleteFromWagon(char material, int weight)
{
    wagonNode *lastWagon = head;
    materialNode *current_material = lastWagon->material;
    bool is_inside = true;

    while (lastWagon->next != NULL)
    {
        lastWagon = lastWagon->next;
    }
    current_material = lastWagon->material;

    while (current_material != NULL)
    {
        if (current_material->id == material)
        {
            int extraWeight = 0;
            if (weight >= current_material->weight)
            {
                extraWeight = weight - current_material->weight;
                materialNode *tobedeleted = current_material;
                if (lastWagon->material != current_material)
                {
                    materialNode *prevMat = getPrev_mat(current_material, lastWagon);
                    prevMat->next = tobedeleted->next;
                    delete tobedeleted;
                    tobedeleted = NULL;
                }
                else
                {
                    lastWagon->material = tobedeleted->next;
                    delete tobedeleted;
                    tobedeleted = NULL;
                }
                removeWagon(lastWagon);
                if (extraWeight != 0)
                {
                    deleteFromWagon(material, extraWeight);
                }
                break;
            }
            else
            {
                current_material->weight -= weight;
                break;
            }
        }
        else
        {
            current_material = current_material->next;
            if (current_material == NULL)
            {
                is_inside = false;
            }
        }
    }
    if (is_inside == false)
    {
        deleteFromPrevWagon(lastWagon, current_material,
                            material, weight, is_inside);
    }
};

//if the train is empty, add a wagon and the material
void Train::addFirstWagonMat(wagonNode *currentWagon, char material,
                             int weight)
{
    int extraWeight = 0;
    addWagon(id_num++, material);
    currentWagon = head;

    //add extraweight in a new wagon 
    if (weight > MAX_CAPACITY)
    {
        extraWeight = weight - MAX_CAPACITY;
        currentWagon->material->weight = MAX_CAPACITY;
        addWagon(id_num++, material);                   //add a wagon to put the extraweight on   
        addMaterial(material, extraWeight);
    }
    else
    {
        currentWagon->material->weight = weight;
    }
}

//if the material doesn't exist in the wagon, adds a new material node and sets its weight
void Train::addNewMaterialNode(wagonNode *currentWagon,
                               materialNode *current_material, char material, int weight)
{
    materialNode *new_material = new materialNode;
    new_material->id = material;
    new_material->weight = 0;
    new_material->next = NULL;
    int extraWeight = 0;

    current_material->next = new_material;
    current_material = current_material->next;

    //add extraweight in a new wagon
    if (weight > MAX_CAPACITY - current_material->weight)
    {
        extraWeight = weight - (MAX_CAPACITY - current_material->weight);
        current_material->weight = MAX_CAPACITY;
        if (!currentWagon->next)
        {
            addWagon(id_num++, material);      //add anotheer wagon if the next wagon is null
        }
        addMaterial(material, extraWeight);
    }
    else
    {
        current_material->weight = weight;
    }
}

//adds a new wagon to the train with the specified material and weight
void Train::addWagon(int id_num, char material)
{
    wagonNode *new_wagon = new wagonNode;
    materialNode *new_material = new materialNode;
    new_material->id = material;
    new_material->weight = 0;
    new_material->next = NULL;
    new_wagon->wagonId = id_num;
    new_wagon->material = new_material;
    new_wagon->next = NULL;

    if (head == NULL)
    {
        head = new_wagon;
    }
    else
    {
        wagonNode *nextWagon = head;
        while (nextWagon->next != NULL)
        {
            nextWagon = nextWagon->next;
        }
        nextWagon->next = new_wagon;
    }
};

/*if all weight is deleted from the current wagon's material x, 
  delete the extraweight from the previous wagon's material x
  until all the specified weight is unloaded from the train*/
void Train::deleteFromPrevWagon(wagonNode *lastWagon,
                                materialNode *current_material, char material,
                                int weight, bool is_inside)
{
    while (is_inside == false)
    {
        wagonNode *prevWagon = getPrev_wagon(lastWagon);
        current_material = prevWagon->material;

        while (current_material != NULL)
        {
            if (current_material->id == material)
            {
                materialNode *tobedeleted = current_material;
                if (weight >= current_material->weight)
                {
                    weight -= current_material->weight;
                    if (weight == 0)
                    {
                        is_inside = true;
                    }
                    if (prevWagon->material != tobedeleted)
                    {
                        materialNode *prevMat = getPrev_mat(current_material, prevWagon);
                        prevMat->next = tobedeleted->next;
                        delete tobedeleted;
                        tobedeleted = NULL;
                        lastWagon = prevWagon;
                    }
                    else
                    {
                        prevWagon->material = tobedeleted->next;
                        delete tobedeleted;
                        tobedeleted = NULL;
                    }
                    removeWagon(prevWagon);
                }
                else
                {
                    current_material->weight -= weight;
                    is_inside = true;
                }
            }
            else
            {
                current_material = current_material->next;
                if (current_material == NULL)
                {
                    is_inside = false;
                    lastWagon = prevWagon;
                }
            }
        }
    }
}

//deletes wagon if there's no material in it >>> all emptied
void Train::removeWagon(wagonNode *lastWagon)
{
    bool delete_it = wagonCapacity(lastWagon);
    wagonNode *tobedeleted = lastWagon;
    wagonNode *prev = NULL;
    if (delete_it)
    {
        prev = getPrev_wagon(lastWagon);
        if (prev == lastWagon)
        {
            head = NULL;
        }

        prev->next = NULL;
        delete tobedeleted;
        tobedeleted = NULL;
    }
};

//checks capacity of the wagon so that it gets deleted if capacity is = 0
bool Train::wagonCapacity(wagonNode *wagon)
{
    wagonNode *current_wagon = head;
    materialNode *current_material = current_wagon->material;

    while (current_wagon != wagon)
    {
        current_wagon = current_wagon->next;
    }
    current_material = current_wagon->material;

    if (current_material == NULL)
    {
        return 1;
    }
    return 0;
};

//get the node of the previous wagon
wagonNode *Train::getPrev_wagon(wagonNode *lastWagon)
{
    wagonNode *prev = head;
    if (lastWagon == head)
    {
        return lastWagon;
    }

    while (prev->next != lastWagon)
    {
        prev = prev->next;
    }
    return prev;
};

//get the node of the previous material
materialNode *Train::getPrev_mat(materialNode *lastMat, wagonNode *wagon)
{
    wagonNode *cur = head;
    materialNode *prevMat = cur->material;
    while (cur != wagon)
    {
        cur = cur->next;
    }

    prevMat = cur->material;

    if (prevMat == lastMat)
    {
        return prevMat;
    }

    while (prevMat->next != lastMat)
    {
        prevMat = prevMat->next;
    }
    return prevMat;
};

//orders materials in each wagon A>B>C>D at the end of the addMaterial function
void Train::orderList()
{
    wagonNode *tempWagon = head;

    while (tempWagon != NULL)
    {
        for (materialNode *i = tempWagon->material; i; i = i->next)
        {
            for (materialNode *j = i->next; j; j = j->next)
            {
                if (i->id > j->id)
                {
                    swap(i->id, j->id);
                    swap(i->weight, j->weight);
                }
            }
        }
        tempWagon = tempWagon->next;
    }
};

void Train::printWagon()
{
    wagonNode *tempWagon = head;

    if (tempWagon == NULL)
    {
        cout << "Train is empty!!!" << endl;
        return;
    }

    while (tempWagon != NULL)
    {
        materialNode *tempMat = tempWagon->material;
        cout << tempWagon->wagonId << ". Wagon:" << endl;
        while (tempMat != NULL)
        {
            cout << tempMat->id << ": " << tempMat->weight << "KG, ";
            tempMat = tempMat->next;
        }
        cout << endl;
        tempWagon = tempWagon->next;
    }
    cout << endl;
};
