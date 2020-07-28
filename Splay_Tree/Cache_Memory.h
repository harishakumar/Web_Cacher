#include<iostream>
using namespace std;

class Cache_Memory
{
    public:
        string URL;
        int frequency;
        Cache_Memory *left_child;
        Cache_Memory *right_child;
};

class Doubly_Linked_List
{
    public:
        Cache_Memory *node;
        Doubly_Linked_List *next;
        Doubly_Linked_List *prev;
};
