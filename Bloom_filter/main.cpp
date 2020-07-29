#include"Cache_Memory.h"
#include"Bloom_filters.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>

static int maximum_URL = 7;
int i = 0;
using namespace std;
Doubly_Linked_List* get_static_start();

int get_count(Cache_Memory *root);

Cache_Memory* insert_URL_in_Cache(Cache_Memory*,string );

Cache_Memory* search_URL(Cache_Memory*,string);

Cache_Memory* NewNode(string);

Cache_Memory* Right_Rotate(Cache_Memory*);

Cache_Memory* Left_Rotate(Cache_Memory* );

Cache_Memory* splay(Cache_Memory*,string);

void preorder_traversal(Cache_Memory*);

void inorder_traversal(Cache_Memory*);

int to_find_height(Cache_Memory*,int);

string to_delete_last_URL(Cache_Memory*);

int Count_URL(Cache_Memory*);

void insert_list(Cache_Memory*);

Doubly_Linked_List* to_travel_URL(Cache_Memory*);

Doubly_Linked_List* bubble_sort(Doubly_Linked_List*);

void display_k_frequently_used_URL(Doubly_Linked_List*,int);

Cache_Memory* to_delete_URL(Cache_Memory *nroot,string URL);

int main()
{
    int choice;
    string URL;
    Bloom_filters Filter;
    Cache_Memory *root = NULL;
    Doubly_Linked_List *start_URL = NULL;
    int k,max_URL;
    while(1)
    {
        cout << "\n\n\n\n\t\t->Enter 1 to visit a new Website";
        cout << "\n\t\t->Enter 2 to open a existing Website";
        cout << "\n\t\t->Enter 3 to know the recently accessed Website";
        cout << "\n\t\t->Enter 4 to know the k-most accessed Website";
        cout << "\n\t\t->Enter 5 to see all the websites in the cache";
        cout << "\n\t\t->Enter 6 to remove a URL from cache";
        cout << "\n\t\t->Enter 7 to exit";
        cout << "\n\nEnter your choice : ";
        cin >> choice;
        switch(choice)
        {
            case 1:
                    cout << "\nEnter the URL : ";
                    cin >> URL;
                    if(i >= 1)
                    {
                        choice = Filter.query(URL);
                        if(choice == 1)
                        {
                            goto start;
                        }
                        else
                            cout << "URL " << URL << " should be inserted newly";
                    }
                    i++;
                    root = insert_URL_in_Cache(root,URL);
                    Filter.insert(URL);
                    max_URL = Count_URL(root);
                    if(max_URL  > maximum_URL)
                    {
                        URL = to_delete_last_URL(root);
                        Filter.delete_URL(URL);
                    }
                    start:
                    break;
            case 2:
                    cout << "\nURL's available in the cache are : " << endl;
                    preorder_traversal(root);
                    cout << "\nEnter the URL to be opened: ";
                    cin >> URL;
                    choice = Filter.query(URL);
                    if(choice == 1)
                        root = search_URL(root,URL);
                    break;
            case 3:
                    cout << "\nThe recently accessed URL is " << root->URL << " and it was accessed " << root->frequency << " time";
                    break;
            case 4:
                    max_URL = Count_URL(root);
                    cout << "\nThere are only " << max_URL << " websites available in the cache";
                    cout << "\nEnter the value of K: ";
                    cin >> k;
                    if(k > max_URL)
                        cout << "\nSorry you are demanding more than the available...";
                    else
                    {
                        to_travel_URL(root);
                        start_URL = get_static_start();
                        start_URL = bubble_sort(start_URL);
                        display_k_frequently_used_URL(start_URL,k);
                    }
                    break;
            case 5:
                    preorder_traversal(root);
                    break;
            case 6:
                    cout << "\nThe available URL's in the cache are : ";
                    preorder_traversal(root);
                     cout << "\nEnter the URL to be removed : ";
                    cin >>  URL;
                    if(Filter.delete_URL(URL))
                    {
                        cout << "\nThe URL has been removed";
                        root = to_delete_URL(root,URL);
                    }
                    break;
            case 7:
                    exit(0);
        }
    }
    return 0;
}
