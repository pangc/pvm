#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include<cstddef>
#include<stdlib.h>

template<class t>
class ll_link  {
public:
   t      data;
   ll_link *next;
};

// The stack class
template<class t>
class Stack  {
public:
   Stack()   {llstart = NULL; n = 0;}
   ~Stack()  {Empty();}
   void Empty();           // Make stack empty
   void Push (t node);     // Push item on stack
   t Pop ();               // Pop item from stack
   inline t GetTop ()         {return llstart->data;}
   t GetNo (int e);        // Get item no. n from stack
   inline int  Len ()         {return n;}
   inline bool IsEmpty ()     {return n==0;}
private:
   ll_link<t> *llstart;    // Start link (last in)
   int n;                  // Number of links
};

template<class t>
void Stack<t>::Empty ()  {
   ll_link<t> *search, *del;

   search = llstart;
   while (search != NULL)    {
      del = search;
      search = search->next;
      delete del;
   }
   llstart = NULL; n = 0;
}


template<class t>
t Stack<t>::Pop ()   {
   t rv; ll_link<t> *del;
   rv = llstart->data;
   del = llstart;
   llstart = llstart->next; n--;
   delete del;
   return rv;
}

template<class t>
t Stack<t>::GetNo (int e)  {
   int i;
   ll_link<t> *link;

   link = llstart;
   i = n - e - 1;
   while (i > 0)   {link = link->next; i--;}
   return link->data;
}

template<class t>
void Stack<t>::Push (t node)   {
   ll_link<t> *newlink;

   newlink = (ll_link<t> *)malloc (sizeof (ll_link<t>));
   newlink->data  = node;
   newlink->next  = llstart;
   llstart        = newlink;
   n++;
}

#endif // STACK_H_INCLUDED
