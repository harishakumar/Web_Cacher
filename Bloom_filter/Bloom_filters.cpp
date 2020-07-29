#include "Bloom_filters.h"
static bool insert_flag = true;

Bloom_filters::Bloom_filters()
{
}

void Bloom_filters::set_URL(int size , int k)
{
    size_of_filter = size;
    filter_array = new int[size]();
    collision_zone = new int[size]();
    for(int i = 0;i<size;i++)
        collision_zone[i] = 0;
    Hash_number = k;
}

int calculate_Hash_Number(double p)
{
    double k1 = log10(1/p);
    int k = k1/log10(2);

    if(k < 2)
        k = 2;
    return k;
}

int calculate_size(double p,int n)
{
    int m = n*abs(log(p)) / pow(log(2),2);

    return m;
}
uint32_t Bloom_filters::FNV_hash(string element)
{
    uint32_t fnv_prime = 16777619;
    uint32_t fnv_offset = 21661362;
    int element_length = element.length();
    uint32_t hash = 0;

    hash = fnv_offset;
    for(int i=0;i<element_length;i++)
    {
        hash = hash^element[i];
        hash = hash*fnv_prime;
    }
    return hash % size_of_filter;
}
uint32_t Bloom_filters::Murmur_hash(string element)
{
    uint32_t seed = 0;
    uint32_t hash;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    int r1 = 15;
    int r2 = 13;
    int m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t k;

    hash = seed;
    int element_length = element.length();
    for(int i=0;i<element_length;i++)
    {
        k = element[i];
        k = k * c1;
        k = (k << r1) | (k >> (32 - r1));
        k = k * c2;

        hash = hash^k;
        hash = (hash << r2) | (hash >> r2);
        hash = hash*m + n;
    }

    hash = hash * 0x85ebca6b;
    hash = hash ^ (hash >> 13);
    hash = hash * 0xc2b2ae35;
    hash = hash ^ (hash >> 16);
    return (hash % size_of_filter);
}
bool Bloom_filters::add_URL(string new_element)
{
    uint32_t fnv_index = FNV_hash(new_element);
    uint32_t murmur_index = Murmur_hash(new_element);
    filter_array[fnv_index] = 1;
    filter_array[murmur_index] = 1;
    collision_zone[fnv_index] = collision_zone[fnv_index] + 1;
    collision_zone[murmur_index] = collision_zone[murmur_index] + 1;

    if(Hash_number > 2)
    {
        for(int i=3;i<=Hash_number;i++)
        {
            uint32_t hash_index = fnv_index + (i * murmur_index);
            hash_index = hash_index % size_of_filter;
            filter_array[hash_index] = 1;
        }
    }
    return true;
}

bool Bloom_filters::query_URL(string element)
{
    bool found = false;
    uint32_t fnv_index = FNV_hash(element);
    uint32_t murmur_index = Murmur_hash(element);
    if(filter_array[fnv_index] == 1 && filter_array[murmur_index] == 1)
    {
        found = true;
    }

    if(Hash_number > 2)
    {
        for(int i=3;i<=Hash_number;i++)
        {
            uint32_t hash_index = fnv_index + (i * murmur_index);
            hash_index = hash_index % size_of_filter;
            if(filter_array[hash_index] != 1)
                found = false;
        }
    }
    return found;
}

bool Bloom_filters::delete_URL(string element)
{
    bool found = false;
    uint32_t fnv_index = FNV_hash(element);
    uint32_t murmur_index = Murmur_hash(element);
    if(filter_array[fnv_index] == 1 && filter_array[murmur_index] == 1)
    {
        found = true;
    }
    if(Hash_number > 2)
    {
        for(int i=3;i<=Hash_number;i++)
        {
            uint32_t hash_index = fnv_index + (i * murmur_index);
            hash_index = hash_index % size_of_filter;
            if(filter_array[hash_index] != 1)
                found = false;
        }
    }
    if(found)
    {
        collision_zone[fnv_index] = collision_zone[fnv_index] - 1;
        collision_zone[murmur_index] = collision_zone[murmur_index] - 1;
        if(collision_zone[fnv_index] == 0)
            filter_array[fnv_index] = 0;
        if(collision_zone[murmur_index] == 0)
            filter_array[murmur_index] = 0;
    }
    else
        cout << "\nURL is not in Bloom Filter " << endl;
    return found;
}

void Bloom_filters::insert(string element)
{
    if(insert_flag == true)
    {
        double p = 2;
        if(element[0] == '?')
        {
            int p1 = element[1] - '0';
            int p2 = element[2] - '0';
            if(p2 > 0)
            {
                p = (p1 * 10) + p2;
                p = p / 100;
            }
            else
                p = p1 / 100.;
        }
        if((p < 0) || (p > 1))
        {
            p = probability;
        }
        int m_array = calculate_size(p,20);
        //cout << m_array << " m_array";
        int k_hash = calculate_Hash_Number(p);
        set_URL(m_array,k_hash);
        insert_flag = false;
        if(element[0] != '?')
        {
            add_URL(element);
            cout << "\nNew URL is added in the Bloom Filter.....";
        }
    }
    else if(add_URL(element))
        cout << "\nNew URL is added in the Bloom Filter.....";
}

int Bloom_filters::query(string element)
{
    int flag = 0;
    clock_t startTime = clock();
    if(query_URL(element))
    {
        cout << "\nURL " << element << " is present in cache\n";
        flag = 1;
    }
    else
        cout << "\nURL " << element << " is not there in the cache\n";

    clock_t endTime = clock();
    clock_t clockTicksTaken = endTime - startTime;
    double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
    cout << "\nTime taken for query: " << timeInSeconds << endl << endl;
    return flag;
}
