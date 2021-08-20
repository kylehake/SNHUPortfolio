#include <iostream>
#include <iomanip> //this is used to format the menu and static reports

using namespace std;

#include "displaymenu.h" //header file for displaymenu

void printInputMenu () { //Prints a blank data input menu
    cout << setfill('_') << setw(30) << "" << endl;
    cout << setw(9) << "" << " Data Input " << setw(9) << "" << endl;
    cout << "Init. Investment Amount" << left << endl;
    cout << "Monthly Deposit" << left << endl;
    cout << "Annual Interest" << left << endl;
    cout << "Number Of Years" << left << endl;
    cout << setfill('_') << setw(30) << "" <<  endl;
    cout << "Press any key to continue..." << left << endl; //Pause program to prompt user to move forward, used with getch
}

void printInputMenuComp (double t_invest, double t_deposit, double t_interest, int t_years) { //Prints a complete data menu with user inputs added.
    cout << setfill('_') << fixed << setprecision(2) << setw(30) << "" << endl;
    cout << setw(9) << "" << " Data Input " << setw(9) << "" << endl;
    cout << setfill(' ') << "Init. Investment Amount" << left << setw(8) << right << t_invest << endl;
    cout << setw(15) << "Monthly Deposit" << left << setw(16) << right << t_deposit << endl;
    cout << setw(15) << "Annual Interest" << setprecision(0) << left << setw(16) << right << t_interest << "%" << endl;
    cout << setw(15) << "Number Of Years" << left << setw(16) << right << t_years << endl;
    cout << setfill('_') << setw(30) << "" <<  endl;
    cout << setw(15) << "Press any key to continue..." << left << endl; //Pause program to prompt user to move forward, used with getch
}

void printReportWODep(double t_invest, double t_deposit, double t_interest, int t_years) { //Prints the first static report, uses user defined "years"
    int i;
    double t_tempbalance = t_invest; //defined before the loop so that it can be iterated properly
    cout << setfill (' ') << fixed << setprecision(2); //Formatting of the header of the static report
    cout << setw(25) << "" << "Balance and Interest Without Additional Monthly Deposits" << setw(20) << "" << endl;
    cout << setfill('_') << setw(100) << "" << endl;
    cout << setfill(' ');
    cout << setw(5) << "" << "Year" << setw(20) << "" << "Year End Balance" << setw(30) << "" << "Year End Earned Interest" << endl;
    cout << setfill('_') << setw(100) << "" << endl;
    cout << setfill(' ') << fixed << setprecision(2);
    for (i = 1; i <= t_years; ++i) { //for loop is used to calculate year end balance and earned interest, and the it formats and prints it into the table.
            double t_tempinterest;
            t_tempinterest = (t_tempbalance * (t_interest / 100));
            t_tempbalance = t_tempbalance + t_tempinterest;
            cout << setw(10) << right << i << setw(36) << right << t_tempbalance << left << setw(50) << "" << t_tempinterest << endl;
            cout << setfill(' ') << setw(100) << "" << endl;
    }
    cout << setw(15) << "Press any key to continue..." << left << endl; //Pause program to prompt user to move forward, used with getch
}

void printReportWithDep(double t_invest, double t_deposit, double t_interest, int t_years) { //Uses calculated months from inputted years to be used in the for loop
    int i;
    double t_months = t_years * 12; //calculates months from years
    double t_openingAmt = t_invest; //opening balance at the beginning of the month
    double t_total; //total in account after monthly deposit is made
    double t_monthInt; //interest earned in current month
    double t_earnedInt; //interested earned over the year, used to calculate year end interest earned
    double t_closingBal; //monthly closing balance
    double t_intConst = ((t_interest/100)/12); // used to simplify math in loop, this is monthly interest constant
    cout << setfill (' ') << fixed << setprecision(2); //Formatting of the header of the static report
    cout << setw(25) << "" << "Balance and Interest With Additional Monthly Deposits" << setw(25) << "" << endl;
    cout << setfill('_') << setw(100) << "" << endl;
    cout << setfill(' ');
    cout << setw(5) << "" << "Year" << setw(20) << "" << "Year End Balance" << setw(30) << "" << "Year End Earned Interest" << endl;
    cout << setfill('_') << setw(100) << "" << endl;
    cout << setfill(' ') << fixed << setprecision(2);
    for (i = 1; i <= t_months; ++i) { //for loop is used to calculate year end balance and earned interest, and the it formats and prints it into the table, uses calculated months
        t_total = t_openingAmt + t_deposit; //calculates total before interest is added in
        t_monthInt = t_total * t_intConst; //calculates interest, compounded monthly
        t_closingBal = t_total + t_monthInt; //total balance including interest at end of the month
        t_earnedInt = t_earnedInt + t_monthInt; //adds monthly earned interest to total interest
        if ((i % 12) == 0) { //Only prints the year end totals for the table, mod 12 is used so that it will only print the 12th month or year end total.
                cout << setw(10) << right << (i / 12) << setw(36) << right << t_closingBal << left << setw(54) << right << t_earnedInt << endl;
            cout << setfill(' ') << setw(100) << "" << endl;
            t_earnedInt = 0; //Resets yearly earned interest to start over for the next year.
        }
        t_openingAmt = t_closingBal; //sets opening balance to closing balance to start the loop for the next month.
    }
    cout << setw(15) << "Press any key to continue..." << left << endl; //Pause program to prompt user to move forward, used with getch
}
