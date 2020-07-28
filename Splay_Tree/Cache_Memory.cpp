#include "Cache_Memory.h"
static int count = 0;                             // to count the no of URL in Cache_Memory
static Doubly_Linked_List *static_start = NULL;     // to insert the URL in Cache_Memory into the doubly linked list as it is inserted recursively


/*      To insert a new node in cache memory        */
Cache_Memory* NewNode(string New_URL)
{
    Cache_Memory *N = new Cache_Memory();
    N->URL = New_URL;
    N->frequency = 1;
    N->left_child = NULL;
    N->right_child = NULL;
    return N;
}

/*      Function to right rotate the given URL in the Cache Memory     */
Cache_Memory* Right_Rotate(Cache_Memory *x)
{
    Cache_Memory *y = x->left_child;
    Cache_Memory *z = y->right_child;
    y->right_child = x;
    x->left_child = z;
    return y;
}

/*      Function to left rotate the given URL in the Cache Memory       */
Cache_Memory* Left_Rotate(Cache_Memory *x)
{
    Cache_Memory *y = x->right_child;
    Cache_Memory *z = y->left_child;
    y->left_child = x;
    x->right_child = z;
    return y;
}

/*      Function to splay the recently used URL in the Cache Memory to the root     */
Cache_Memory* splay(Cache_Memory *root,string New_URL)
{
    if(root == NULL || root->URL == New_URL)
        return root;

    if(root->URL > New_URL)
    {

        if(root->left_child->URL > New_URL)
        {
            root->left_child->left_child = splay(root->left_child->left_child,New_URL);
            root = Right_Rotate(root);
        }

        else if(root->left_child->URL < New_URL)
        {
            root->left_child->right_child = splay(root->left_child->right_child,New_URL);

            root->left_child = Left_Rotate(root->left_child);
        }
        return Right_Rotate(root);
    }
    else
    {

        if(root->right_child->URL > New_URL)
        {
            root->right_child->left_child = splay(root->right_child->left_child,New_URL);

            root->right_child = Right_Rotate(root->right_child);

        }

        else if(root->right_child->URL < New_URL)
        {
            root->right_child->right_child = splay(root->right_child->right_child,New_URL);
            root = Left_Rotate(root);
        }
        return Left_Rotate(root);
    }
}

/*      To insert the newly used URL in the Cache Memory        */
Cache_Memory* insert_URL_in_Cache(Cache_Memory *root,string New_URL)
{
    Cache_Memory *temp = root;
    while(temp != NULL)
    {
        if(temp->URL > New_URL)
        {
            if(temp->left_child != NULL)
                temp = temp->left_child;

            else
            {
                temp->left_child = NewNode(New_URL);
                break;
            }
        }
        else
        {
            if(temp->right_child != NULL)
                temp = temp->right_child;
            else
            {
                temp->right_child = NewNode(New_URL);
                break;
            }
        }
    }
    root = splay(root,New_URL);
    return root;
}

/*      Function to display the URL in the Cache Memory in pre-order traversal      */
void preorder_traversal(Cache_Memory *root)
{
    if(root != NULL)
    {
        cout << root->URL << " " << root->frequency << endl;
        preorder_traversal(root->left_child);
        preorder_traversal(root->right_child);
    }
}

/*      Function to display the URL in the Cache Memory in in-order traversal       */
void inorder_traversal(Cache_Memory *root)
{
    if(root != NULL)
    {
        inorder_traversal(root->left_child);
        cout << root->URL << " ";
        inorder_traversal(root->right_child);
    }
}

/*      Function to find the height of the node that has URL in it      */
int to_find_height(Cache_Memory *root,int *f)
{
    if(root == NULL)
        return 0;

    else
    {
        int &x = *f;
        int left_height = to_find_height(root->left_child,&x);
        int right_height = to_find_height(root->right_child,&x);

        if(left_height > right_height)
        {
            *f = 1;
            return (left_height + 1);
        }
        else
        {
            *f = 0;
            return (right_height + 1);
        }
    }
}

/*      Function to delete the last URL in the cache memory when the amount of URL in cache memory exceeds the limit        */
void to_delete_last_URL(Cache_Memory *root)
{
    Cache_Memory *nroot = root;
    int f = 0;
    do
    {
        to_find_height(nroot,&f);
        if(f)
            nroot = nroot->left_child;
        else
            nroot = nroot->right_child;
    }while(nroot->left_child != NULL || nroot->right_child != NULL);
    string New_URL = nroot->URL;
    while(root != NULL)
    {
        if(root->left_child != NULL)
        {
            if(root->left_child->URL == New_URL)
            {
                f = 1;
                break;
            }
        }
        if(root->right_child != NULL)
        {
            if(root->right_child->URL == New_URL)
            {
                f = 0;
                break;
            }
        }
        if(New_URL > root->URL)
        {
            root = root->right_child;
        }
        else
        {
            root = root->left_child;
        }
    }
    if(f)
    {
        root->left_child = NULL;
        delete nroot;
    }
    else
    {
        root->right_child = NULL;
        delete nroot;
    }
}

/*      Function to the URL in the Cache Memory     */
int Count_URL(Cache_Memory *root)
{
    if(root != NULL)
    {
        Count_URL(root->left_child);
        count++;
        Count_URL(root->right_child);
    }
    return count;
}

/*      Function to insert the URL in cache memory into Doubly Linked list to find k most frequently used URL     */
void insert_list(Cache_Memory *root)
{
    if(static_start == NULL)
    {
        Doubly_Linked_List *temp = new Doubly_Linked_List();
        temp->node = root;
        temp->prev = NULL;
        temp->next = NULL;
        static_start = temp;
    }
    else
    {
        Doubly_Linked_List *temp = new Doubly_Linked_List();
        temp->node = root;
        temp->prev = NULL;
        temp->next = static_start;
        static_start->prev = temp;
        static_start = temp;
    }
}

/*      Function that initiates inserting URL into Doubly Linked List       */
Doubly_Linked_List* to_travel_URL(Cache_Memory *root)
{
    if(root != NULL)
    {
        to_travel_URL(root->left_child);
        insert_list(root);
        to_travel_URL(root->right_child);
    }
    return static_start;
}

/*      Function that searches the URL in Cache Memory      */
Cache_Memory* search_URL(Cache_Memory *root,string New_URL)
{
    Cache_Memory *temp = root;
    int flag = 0;
    while(temp != NULL)
    {
        if(temp->URL > New_URL)
            temp = temp->left_child;
        else if(temp->URL < New_URL)
            temp = temp->right_child;
        else
        {
            flag = 1;
            break;
        }
    }

    if(flag == 1)
    {
        root = splay(root,New_URL);
        root->frequency = root->frequency + 1;
    }
    return root;
}

/*      Function that displays the nodes in the doubly linked list      */
void display_doubly_linked_list(Doubly_Linked_List *start)
{
    Doubly_Linked_List *ptr = start;
    if(start == NULL)
    {
        cout << "\nThe list is Empty" << endl;
        return;
    }
    while(ptr != NULL)
    {
        cout << ptr->node->URL << " " << ptr->node->frequency << endl;
        ptr = ptr->next;
    }
}

/*      Function to sorts the Doubly linked list by bubble sort to find the k most frequently used URL      */
Doubly_Linked_List* bubble_sort(Doubly_Linked_List *start)
{

    Doubly_Linked_List *current = NULL;
    Doubly_Linked_List *index = NULL;

    Cache_Memory *temp;

    if(start == NULL)
        return start;

    else
    {
        for(current = start;current->next != NULL;current = current->next)
        {
            for(index = current->next;index != NULL;index = index->next)
            {
                if(current->node->frequency < index->node->frequency)
                {
                    temp = current->node;
                    current->node = index->node;
                    index->node = temp;
                }
            }
        }
    }
    return start;
}

/*      Function to display the k most frequently used URL's        */
void display_k_frequently_used_URL(Doubly_Linked_List *start,int k)
{
    int i =1;
    cout << "\n";
    while(i <= k)
    {
        cout << start->node->URL << " " << start->node->frequency << endl;
        start = start->next;
        i++;
    }
}
/*
int main()
{
    Cache_Memory *root = NewNode("sadham");
    Doubly_Linked_List *start_URL = NULL;
    root = insert_URL_in_Cache(root,"abdul");
    root = insert_URL_in_Cache(root,"aslam");
    root = insert_URL_in_Cache(root,"karur");
    root = insert_URL_in_Cache(root,"trichy");
    root = insert_URL_in_Cache(root,"coimbatore");
    root = insert_URL_in_Cache(root,"chennai");
    cout << "\nPre_order " << endl;
    preorder_traversal(root);
    cout << "\nIn_order " << endl;
    inorder_traversal(root);
    int count;
    count  =  Count_URL(root);
    cout << count;
    int max = 5;
    while(count>max)
    {
        count = count - 1;
        to_delete_last_URL(root);
    }
    cout << endl;
    preorder_traversal(root);
    cout << endl;
    root = search_URL(root,"sadham");
    root = search_URL(root,"coimbatore");
    preorder_traversal(root);
    to_travel_URL(root);
    start_URL = static_start;

    //display_doubly_linked_list(start_URL);

   start_URL = bubble_sort(start_URL);
    display_doubly_linked_list(start_URL);
    int k =2;
   display_k_frequently_used_URL(start_URL,k);
//    cout << start_URL->node->frequency;
//    cout << start_URL->node->URL;
//    cout << "\nAfter deletion : " << endl;
//    to_delete_last_URL(root);
//    cout << "\nPre_order " << endl;
//    preorder_traversal(root);
//    cout << "\nIn_order " << endl;
//    inorder_traversal(root);

    return 0;
}
*/
