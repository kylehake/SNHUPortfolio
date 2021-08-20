#include <iostream>
#include <conio.h> //needed for getch(); so user enabled pauses can be implemented
#include <stdlib.h> //needed to clear screen

using namespace std;

#include "displaymenu.h" //needed for menus and reports

int main () {
    //variables
    int yearInput;
    double initInvInput;
    double interestInput;
    double mDepInput;

    //prints first menu
    printInputMenu();

    //waits for any key to be pressed then clears screen
    getch();
    system("CLS");

    //asks for initial investment and input
    cout << "Enter initial investment amount." << endl;

    cin >> initInvInput;

    //checks for positive number, and asks to re-enter if not
    while (initInvInput < 0) {
        cout << "Error, enter positive value." <<endl;
        cin >> initInvInput;
    }

    //asks for monthly deposit amount and input
    cout << "Enter monthly deposit amount." << endl;

    cin >> mDepInput;

    //checks for positive number, and asks to re-enter if not
    while (mDepInput < 0) {
        cout << "Error, enter positive value." <<endl;
        cin >> mDepInput;
    }

    //asks for annual interest and input
    cout << "Enter annual interest." << endl;

    cin >> interestInput;

    //checks for positive number, and asks to re-enter if not
    while (interestInput < 0) {
        cout << "Error, enter positive value." <<endl;
        cin >> interestInput;
    }

    //asks for investment  length and input
    cout << "Enter length of investment in years." << endl;

    cin >> yearInput;

    //checks for positive number, and asks to re-enter if not
    while (yearInput < 0) {
        cout << "Error, enter positive value." <<endl;
        cin >> yearInput;
    }


    //clears screen and prints completed data table
    system("CLS");
    printInputMenuComp(initInvInput, mDepInput, interestInput, yearInput);

    //waits for any key to be pressed then clears screen
    getch();

    system("CLS");
    printReportWODep(initInvInput, mDepInput, interestInput, yearInput); //prints first static report

    //waits for any key to be pressed
    getch();

    printReportWithDep(initInvInput, mDepInput, interestInput, yearInput); //prints second static report
}
