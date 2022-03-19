currentWagon = head;
materialNode *current_material = currentWagon->material;
int empty_space = matCapacity_checker(material, currentWagon);
if (empty_space > 1)
{
    while (current_material->id != material)
    {
        current_material = current_material->next;
    }
    if (weight > empty_space)
    {
        extraWeight = weight - empty_space;
        current_material->weight = empty_space;
        addWagon(id_num++);
        addMaterial(material, extraWeight);
    }
    else
    {
        current_material->weight = current_material->weight + weight;
    }
}
else
{
    while (empty_space == 1)
    {
        currentWagon = currentWagon->next;
        empty_space = matCapacity_checker(material, currentWagon);
    }
    if (empty_space == 0)
    {
        currentWagon->material = new materialNode;
        current_material = currentWagon->material;
        if (weight > MAX_CAPACITY)
        {
            extraWeight = weight - MAX_CAPACITY;
            current_material->weight = MAX_CAPACITY;
            addMaterial(material, extraWeight);
        }
        else
        {
            current_material->weight = weight;
        }
        current_material->id = material;
        current_material->next = NULL;
    }
    else if (empty_space > 1)
    {
        current_material = currentWagon->material;
        while (current_material->id != material)
        {
            current_material = current_material->next;
        }
        if (weight > MAX_CAPACITY - empty_space)
        {
            extraWeight = weight - (MAX_CAPACITY - empty_space);
            current_material->weight = MAX_CAPACITY;
            addWagon(id_num++);
            addMaterial(material, extraWeight);
        }
        else
        {
            current_material->weight = current_material->weight + weight;
        }
    }
}