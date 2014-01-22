#include <iostream>
#include <sstream>


class Elem
{
public:
    Elem(int k) : key(k), next(NULL), prev(NULL) { }
    
    int key;
    Elem *next, *prev;
    Elem *new_next, *new_prev;
};


class List
{
public:
    Elem *head, *tail;
    int length;

    List() : head(NULL), tail(NULL), length(0) { }
    ~List();
    
    void insert_back(int n);
    Elem *submax(Elem *e);
    void flip(Elem *e);
    void print();
    void sort();
};


List::~List()
{
    Elem *e = head;
    Elem *f;
    
    while(e != NULL)
    {
        f = e->next;
        delete e;
        e = f;
    }
}


void List::insert_back(int n)
{
    Elem *e = new Elem(n);
    
    if(tail == NULL)
        head = tail = e;
    else
    {
        tail->next = e;
        e->prev = tail;
        tail = e;
    }
    
    length++;
}


Elem *List::submax(Elem *e)
{
    Elem *f = head;
    int count = 0;
    
    Elem *f_max = head;
    
    /*while(e != NULL && count <= length - i)
    {
        if(e->key > emax->key) emax = e;
        e = e->next;
    }*/
    
    while(f != NULL && f != e->next)
    {
        if(f->key > f_max->key) f_max = f;
        f = f->next;
    }
    
    return f_max;
} 


void List::print()
{ 
    Elem *e = head;
    
    while(e != NULL)
    {
        std::cout << e->key;
        if(e != tail) std::cout << " ";
        e = e->next;
    }
    
    //e = tail;
    
    std::cout << std::endl;
    
/*    while(e != NULL)
    {
        std::cout << e->key << " ";
        e = e->prev;
    }*/
}


void List::flip(Elem *new_top)
{
    Elem *new_bottom = head;
    Elem *next_bottom = new_top->next;
    
    Elem *e = new_top;
    
    int count = length + 1;
    
    while(e != NULL)
    {
        e->new_next = e->prev;        
        e = e->prev;
        
        count--;
    }
    
    std::cout << count << " ";

    e = head;
    
    while(e != next_bottom)
    {
        e->new_prev = e->next;
        e = e->next;
    }

    head = new_top;
    new_top->new_prev = NULL;
    new_bottom->new_next = next_bottom;
    if(next_bottom) next_bottom->prev = new_bottom;
    else tail = new_bottom;
    
    e = head;
    
    while(e != next_bottom)
    {
        e->prev = e->new_prev;
        e = e->new_next;
    }
    
    e = new_bottom;
    
    while(e != NULL)
    {
        e->next = e->new_next;
        e = e->new_prev;
    }
}


void List::sort()
{
    Elem *e = tail;
    Elem *max;
    
    while(e != head)
    {
        max = submax(e);
        
        if(max != e)
        {
            if(max != head) flip(max);
            flip(e);
        }
        
        e = max->prev;
    }
    
    std::cout << "0" << std::endl;
}



int main()
{
    using namespace std;
    
    while(!cin.eof())
    {
        List l;
        
        string line;
        getline(cin, line);
        if(line.length() == 0) break;
        
        stringstream ss(line);
        
        while(!ss.eof())
        {
            int i;
            ss >> i;
            l.insert_back(i);
        }
        
        l.print();
        l.sort();        
    }
    
    return 0;
}
