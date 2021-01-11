#Web-Catcher

Web-Catcher developed with C++ maintains the Cache of a browser.The Cache Memory is implemented using Splay trees to handle Cache efficiently. Loading and Searching of URL is implemented with Bloom Filters to make membership check-in constant space and time.

#What is Bloom Filters?

Bloom filter is a probabilistic data structure invented by Burton Howard Bloom in 1970. It allows for membership check-in constant space and time. Bloom filter trades exactness for efficiency and has a large number of applications in software engineering.

#Why Bloom Filters?

It allows for membership lookup in constant space & time. Bloom filter can very quickly answer YES/NO questions, like “is this item in the set?”.
Very infrequently it will give a false positive answer, implies it will say YES if the answer is NO (Probably in the set).
It will never give false negative answer implies it will never say NO if the answer is YES (Definitely not in the set).
The basic bloom filter supports two operations test and add.
It has constant time complexity for both adding items and asking whether a key is present or not.
You can’t remove an item from a bloom filter.
It also requires very less space compared to the number of items you need to store and check.
Bloom filters use hash functions and before discussing bloom filter in detail, let’s have a look at hashing and hash functions.

#Bloom filter implementation

A basic bloom filter will have two operations test and add. The Base data structure for the bloom filter is bit vector or bit-array. It uses a bit array of size m and k hash functions. Initially, all the bits in bit vector will be set to 0.

To add an element to the bloom filter, we hash the element k times using hash functions and set bits at indexes of those hash values.

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
To test for membership, we simply hash the element with hash functions and then check if those indices are set in the bit vector. If the bit at all those indices is not set, you know that the element is not in the set. If they are set, it might be because the same element or combination of other elements could have set the same bits. Later is the reason why a bloom filter can sometimes give a false positive answer.

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
