// File:   01set_and_multiset.cpp for CS319-Week 11
// Author: Niall Madden
// Date:   25-03-2015
// What:   Compare the use of set and multiset
// For more information see
//    http://www.maths.nuigalway.ie/~niall/CS319

#include <iostream>
#include <iomanip>
#include <set>       // set and multiset
#include <list>

using namespace std;

int main(void )
{
  list <int> list_int;
  list <int>::iterator list_int_i;
  multiset <int> multi_int;
  multiset <int>::iterator multi_int_i;

  for (int i=1; i<=20; i++) // (0,3,6,9,12,15,18)
  {

     list_int.push_back(i);
  }

    list_int_i = list_int.begin();
    advance(list_int_i,5);
    cout << *list_int_i;
//  cout << "The multiset has " << multi_int.size() << " items." << endl;
//  cout << "\t They are: ";
//  for (multi_int_i = multi_int.begin();
//       multi_int_i != multi_int.end();
//       multi_int_i++)
//     cout <<  setw(3) << *multi_int_i;
//  cout << endl;
//  cout << "\t 6 occurs " << multi_int.count(6) << " times." << endl;
//
//  cout << "The set has " << set_int.size() << " items." << endl;
//  cout << "\t They are: ";
  for (list_int_i = list_int.begin();
       list_int_i != list_int.end();
       list_int_i++)
     cout <<  setw(3) << *list_int_i;
//  cout << endl << "\t 6 occurs " << set_int.count(6) << " time." << endl;
  return(0);
}
