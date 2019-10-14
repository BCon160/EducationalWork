//Author: Brendan Connolly
//ID: 12370346
//Email: b.connolly16@nuigalway.ie
//Date: 17/2/15
//Description: This program is a parser that checks that all brackets in a given file.  It can also handle such things as comments and quotation marks.
//Note: Small parts of this program are based on code written by Dr Niall Madden

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>


using namespace std;

class stack {               //For method descriptions please go to where the methods are defined.
private:
      char *contents;
      int max_stacksize;    //This variable will be used in the isFull method to check if the stack is full yet.
      int top;              //This variable will keep track of how many items are currently in the stack.
public:
      stack (void);
      stack (unsigned int StackSize);
      ~stack (void);
      void push(char c);
      char pop(void);
      bool isEmpty(bool endFile);
      bool isFull(void);
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
    if (isEmpty(false)==false)      //"false" parameter means that we have not reached the end of the file.
    {
        top--;
        return(contents[top]);
    }
    else
    {
       return('\0');
    }
}

bool stack::isEmpty(bool endFile)       //This method checks if the stack is currently empty and returns true in this case.
                                        //This method is called in stack:pop()
{
    if (top==0)                 //Recall, top is the current number of items in the stack
    {
        if (endFile == false)
        {
            return(true);
        }
        else
        {
            return(true);
        }
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

void isMLComment(string currLine);

int main(void)
{
    bool noError = true, isComment = false;
    unsigned lineCount = 1, stackSize = 0;
    char comp;                      //comp is a temporary variable to be used when comparing things from the stack and file.
    ifstream InFile;
    string InFileName;
    char c;

    cout << "Please enter the name of the file you want to check for errors in: " << endl;
    cin >> InFileName;
    InFile.open(InFileName.c_str());

    while ( InFile.fail() )     // This whlie loop provides a warning to the user that the file couldn't be opened.
    {
        cout << "Could not open: " << InFileName << ". Please enter a new filename" << endl;
        cin >> InFileName;
        InFile.open(InFileName.c_str());
    }

    cout << "Successfully opened: " << InFileName << endl;
    InFile.get(c);              //This code reads the next character from the open file. It is used a lot below.

    while( InFile.eof() == false)
    {
        if (c == '(' || c == '{' || c == '[')   //We count the total number of opening brackets in the file to obtain
            stackSize++;                        //the maximum possible required stacksize
        else if (c == '\n')
            lineCount++;
        else{}
        InFile.get(c);
    }

    cout << "Total number of lines in file are: " << lineCount << endl;

    stack st(stackSize);
    InFile.clear();             //Lines 140-141 reset the file as if it was never touched
    InFile.seekg(ios::beg);
    lineCount = 1;              //lineCount must be reset to 1 to account for the first line

    InFile.get(c);

    while( InFile.eof() == false)   //While the end of file(eof) has not been reached do:
    {
        if (c == '/')       //This if block checks for comments
        {
            if (InFile.peek() == '/')   //This branch of the if block checks for single line comments
            {
                while (c != '\n')   //The end of the line will signify the end of the comment.
                {
                    InFile.get(c);
                }
                lineCount++;
            }
            else if (InFile.peek() == '*')      //This branch checks for multi-line comments
            {
                isComment = true;
                while (isComment == true)
                {
                    InFile.get(c);
                    if (c == '*')
                    {
                        InFile.get(c);
                        if (c == '/')
                            isComment = false;
                        else{}
                    }
                    else if (c == '\n')
                        lineCount++;
                    else{}
                }
            }
            else{}
        }
        else if (c == '"')      //This branch makes sure the program ignores brackets inside double quote marks
        {
            InFile.get(c);
            while (c != '"')
                InFile.get(c);
        }
        else if (c == '\'')     //This branch makes sure the program ignores brackets inside single quote marks
        {
            InFile.get(c);
            while (c != '\'')
                InFile.get(c);
        }
        else if (c == '\n')     //This branch keeps track of the number of lines in the open file
        {
            lineCount++;
        }
        else if (c == '(' || c == '{' || c == '[')
        {
            st.push(c);     //If the current character is an opening bracket of some type, it goes on the stack.
        }
        else if (c == ')')
        {
            if (st.isEmpty(false) == true)  //If the stack is empty it means that the current ')' has no matching opening '('
            {
                noError = false;
                cerr << "The closing bracket ) in line " << lineCount << " is unmatched." << endl;
            }
            else
            {
                comp = st.pop();    //Here, we check the last item on the stack to see if the brackets match up. This is the same in the next 2 branches
                if (comp != '(')
                {
                    noError = false;
                    cerr << "Your Bracketing is incorrect.  Round brackets () don't match up in line " << lineCount << endl;
                }
                else{}
            }
        }
        else if(c == '}')       //Same logic as lines 199-216
        {
            if (st.isEmpty(false) == true)
            {
                noError = false;
                cerr << "The closing brace } in line " << lineCount << " is unmatched." << endl;
            }
            else
            {
                comp = st.pop();
                if (comp != '{')
                {
                    noError = false;
                    cerr << "Your Bracketing is incorrect.  Braces {} don't macth up in line " << lineCount << endl;
                }
                else{}
            }
        }
        else if(c == ']' )      //Same logic as lines 199-216
        {
            if (st.isEmpty(false) == true)
            {
                noError = false;
                cerr << "The closing bracket ] in line " << lineCount << " is unmatched." << endl;
            }
            else
            {
                comp = st.pop();
                if (comp != '[')
                {
                    noError = false;
                    cerr << "Your Bracketing is incorrect.  Square brackets [] don't macth up in line " << lineCount << endl;
                }
                else{}
            }
        }
        else if(c == '&' || c == '|')       //This branch warns the user that they may be using syntax that they don't realise is wrong
        {
            InFile.get(c);
            if (c != '&' || c != '|')
                cout << "Be careful, possible misprint in line: " << lineCount << ". To get a logical and type &&. To get logical or type ||." << endl;
            else{}
        }

        else{}
        InFile.get(c);
    }
    if (st.isEmpty(true) == true)        //In st.isEmpty(true), the "true" parameter tells us that we have reached the end of the file
    {
        if (noError == true)
        {
            cout << "This file has great (bracket) syntax. Well done!" << endl;
            cout << lineCount << " lines were checked" << endl;
        }
        else
        {
            cout << "There is a syntax error in your file(see above for details)" << endl;
            cout << lineCount << " lines were checked" << endl;
        }
    }
    else
    {
        cout << "The syntax is incorrect, you have too many opening brackets." << endl;
        cout << lineCount << " lines were checked" << endl;
        cout << "These are the brackets that didn't match anything:" << endl;
        while (st.isEmpty(true) == false)
        {
        cout << st.pop() << endl;
        }
    }
    InFile.close();
    return(0);
}
