#include "item.h"

item::item(std::string itemName, int texture)
{
    this->itemname = itemName;
    this->texture = texture;

    this->owner = nullptr;

    this->health = 0;
    this->mana = 0;
    this->attack = 0;
    this->defense = 0;
}

item::~item()
{}
