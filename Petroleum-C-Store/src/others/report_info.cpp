// Author:   Herbert Schildt
// Modified: Qiang Hu
// File name: ~ftp/pub/class/cplusplus/Array/Array4.cpp
// Purpose:  A simple employee database program.

#include <iostream>
using namespace std;

// function prototyping
int menu();    // funciton to display the menu
void enter();  // function to enter info
void report(); // function to print report

// Global variables:
char name[2][80];  // this array holds employee names
char phone[2][20]; // their phone numbers
float hours[2];    // hours worked per week
float wage[2];     // wage
int choice;

int main()
{
  do {
    choice = menu(); // get selection
    switch(choice) {
      case 0: break;
      case 1: enter();
        break;
      case 2: report();
        break;
      default: cout << "Try again.\n\n";
    }
  } while(choice != 0);

  return 0;
}

// Return a user's selection.
int menu()
{
  int choice;

  cout << "0. Quit\n";
  cout << "1. Enter information\n";
  cout << "2. Report information\n";
  cout << "\nChoose one: ";
  cin >> choice;

  return choice;
}

// Enter information.
void enter()
{
  int i;
  for(i=0; i<2; i++) {
    cout << "Enter last name: ";
    cin >> name[i];
    cout << "Enter phone number: ";
    cin >> phone[i];
    cout << "Enter number of hours worked: ";
    cin >> hours[i];
    cout << "Enter wage: ";
    cin >> wage[i];
  }
}

// Display report.
void report()
{
  int i;

  for(i=0; i<2; i++) {
    cout << name[i] << ' ' << phone[i] << '\n';
    cout << "Pay for the week: " << wage[i] * hours[i];
    cout << '\n';
  }
}
