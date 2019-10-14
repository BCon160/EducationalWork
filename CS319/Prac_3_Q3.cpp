#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class stack {               //For method descriptions please go to where the methods are defined.
private:
      char *contents;
      int max_stacksize;    //This variable will be used in the isFull method to check if the stack is full yet.
      bool isFull(void);
      bool isEmpty(void);
public:
      stack (void);
      stack (unsigned int StackSize);
      ~stack (void);
      void push(char c);
      char pop(void);
      int top;              //This variable will keep track of how many items are currently in the stack.
};

stack::stack(unsigned int StackSize)    //This constructor takes a user enteres value an creates an array(stack) of thet size.
{
    top = 0;
    max_stacksize = StackSize;
    contents = new char[StackSize];
}

stack::~stack(void )    //This destructor deletes the stack array to free up memory
{
    delete [] contents;
}

void stack::push(char c)    //This method puts the char handed to it onto the top of the stack if there is space, otherwise the user is warned.
{
    if (isFull()==false)
    {
        contents[top]=c;
        top++;
    }
    else
    {
        cerr << "The stack is full.  Stack needs to be popped before it can be pushed again!" << endl;
    }
}

char stack::pop(void)       //This method returns the item on top of the stack if one exists, otherwise the user is warned.
{
    if (isEmpty()==false)
    {
        top--;
        return(contents[top]);
    }
    else
    {
       cerr << "The stack is empty. It needs to be pushed before it can be popped again!" << endl;
       return('\0');
    }
}

bool stack::isEmpty(void)       //This method checks if the stack is currently empty and returns true in this case. This method is called in stack::pop.
{
    if (top==0)                 //Recall, top is the current number of items in the stack
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

bool stack::isFull(void)        //This method checks if the stack is currently full and returns true in this case. This method is called in stack::push.
{
    if (top==max_stacksize)     //Again, top is the current number of items in the stack
    {
        return(true);
    }
    else
    {
        return(false);
    }
}


int main(void )
{
    double maxSize;         //maxSize will store the size of stack we will use
    unsigned subj, i = 1, binNumSize, digit;    //Subj will be the decimal number, i is a placeholder(2nd while loop), binNumSize stores the size of the binary number,
                                                //digit is a temporary variable(1st while loop)

    cout << "Please enter a positive integer so it can be converted into decimal form: " << endl;
    cin >> subj;

    maxSize = log2(subj) + 1;       //These two lines make sure that we create a stack big enough handle the number of binary digits that represent the
    maxSize = ceil(maxSize);        //decimal number.  The code uses the fact that a binary number will have no more than log2(decimal form) + 1 digits.

    stack st(maxSize);

    while (subj >= 1.0)     //This while loop calculates the binary digits that correspond to "subj" and puts them in the stack
    {
        digit = subj % 2;
        if (digit==1)       //Digit is of type unsigned int.  Our stack can only store chars so I have to include this if block.
            st.push('1');
        else
            st.push('0');
        subj/=2;
    }

    cout << "The binary representation of this number is: ";
    binNumSize = st.top;    //binNumSize accesses the last position in the stack.  This tells us how many binary digits are in the stack.

    while (i<=binNumSize)   //This while loop pops all of the binary digits out of the stack
    {
        cout << st.pop();
        i++;
    }

    return (0);
}

