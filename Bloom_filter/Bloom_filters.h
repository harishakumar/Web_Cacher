#include<string>
#include<stdlib.h>
#include<stdint.h>
#include<cmath>
#include<ctime>
#include<iostream>
//#include "Cache_Memory.h"
#define probability 0.2
#define exp_num 1000
#define arraysize 200
using namespace std;

class Bloom_filters
{
    private:
        uint32_t FNV_hash(string);
        uint32_t Murmur_hash(string);
    public:
        Bloom_filters();
        int size_of_filter;
        int *filter_array;
        int Hash_number;
        int *collision_zone;

        void set_URL(int,int);
        void set_size(int);
        void insert(string);
        int query(string);
        bool delete_URL(string);

        bool add_URL(string);
        bool query_URL(string);
};
