//
//  main.c
//  Craps
//
//  Created by Shiuan Lin on 10/4/15.
//

#include <stdio.h>
#include <stdbool.h> // For many flags
#include <stdlib.h>  // atoi();
#include <math.h>    // Srand(); rand();
#include <ctype.h>   // isdigital();
#include <time.h>    // For Srand() to create new seed

#define RULETEXT "Game rule: \n Set your bet, if the first roll is 7 or 11, you win.\n If the first roll is 2,3,12, then you lose. \n If the first roll is anything else,\n it will be set as the new winning point.\n If 7 is rolled before the winning point, then you lose.\n Betting 0 means you don't wanna play\n Have Fun!\n\n"
#define NOTDICENUMBER 0
//#define DEBUGMODE // Toggle to enable DEBUGMODE


typedef struct  bank    Bank;           // Holds the bank and bets.
typedef struct  point   Points;         // Holds the winning/losing points
typedef struct  dice    Dice;           // Holds dice's sum and numbers
typedef enum    states  CrapsStates;    // The state machine.

enum states{
    
    //Greeting, showing rules, initializing data, and go to BetGetting state.
    Initializing,
    
    //Showing bank balence, asking user to bet (setBet checks input logic and other good stuff), and go to Rolling or Ending state.
    BetGetting,
    
    //Rolling numbers, check if win, lose, or set a new point. Go to Win/Lose/PatternShowing state.
    Rolling,
    
    //Showing the number rolled, and the type of the number(hard & easy); Go back to BetGetting state.
    PatternShowing,
    
    //Add money or take money from bank. Go to BetGetting state. If broke, ends the game.
    Winning,
    Losing,

    //Showing a greeting and toggle the playing flag.
    Ending
};

struct point{
    int  winPoint[2];        // Holds 7 and 11 as inital, else [0] holds winning, [1] holds 0.
    int  losePoint[3];       // Hold 2, 3, 12. then hold 7 once the point is set.
    bool establishedPoint;  // A flag that helps logic to know which data to compare.
    bool (*checkWin)(Points *, Dice *); // Function pointer that check against winning point.
    bool (*checkLose)(Points *, Dice *); // Function pointer that check against losing point.

};

struct bank{
    int myMoney, myBet;         // Holds the total and betting number.
    void (*setBet)(Bank *);     // Function pointer that sets the bet.
    void (*winMoney)(Bank *);   // Function pointer that adds money.
    void (*loseMoney)(Bank *);  // Function pointer that takes money.
    
};

struct dice{
    int firstRoll, secondRoll, sum;         // Holds what it says.
    void (* rollNumbers)(Dice *);           // Function pointer that gets numbers base on time.
    bool (* checkPointAndUpdate)(Points *, Dice *); // Updates the new winning points.
    char* (* numbertoEnglish)(Dice *);
};

// Initializing the structures.
void initDataTypes(Bank * refBank, Dice *refDice);
void initPoints(Points * refPoints);

// The sturct will take this function. They are doing the right thing, don't worry.
bool checkWin(Points * refPoints, Dice * refDice);
bool checkLose(Points * refPoints, Dice * refDice);
void setBet(Bank * refBank);
void winMoney(Bank * refBank);
void loseMoney(Bank * refBank);
void rollNumbers(Dice * refDice);
bool checkPointAndUpdate(Points * refPoints, Dice * refDice);
char* numbertoEnglish(Dice * refDice);




int main(int argc, const char * argv[]) {

    Points myPoints;
    Bank myBank;
    Dice myDice;
    CrapsStates currentState = Initializing;
    bool playing = true;
    
    while(playing){
        switch(currentState){
                
            case Initializing:
                printf("Welcome to Eric's Craps game\n");
                printf(RULETEXT);
                initDataTypes(&myBank, &myDice);
                initPoints(&myPoints);
                currentState = BetGetting;
                break; // case Initializing
            
            case BetGetting:
                printf("Your bank has $ %d, how much do you want to bet?\n", myBank.myMoney);
                myBank.setBet(&myBank); // Scans the input, and check if that's legal.
                currentState = (myBank.myBet == 0)? Ending : Rolling; // No bet? End.
                break; // case BetGetting
            
            case Rolling:
                myDice.rollNumbers(&myDice);
                
                if (myPoints.checkWin(&myPoints, &myDice)){
                    currentState = Winning;
                    break;
                }
                if (myPoints.checkLose(&myPoints, &myDice)) {
                    currentState = Losing;
                    break;
                }
                if(myDice.checkPointAndUpdate(&myPoints, &myDice)){
                    currentState = BetGetting;
                } else{
                    currentState = PatternShowing;

                }
                break; // case Rolling

            case PatternShowing:
                // Logic that show the pattern. Easy/ hard and speical name.
                if (myDice.firstRoll == myDice.secondRoll) {
                    printf("\nIt is a Hard %s (%d, %d)\n", myDice.numbertoEnglish(&myDice), myDice.firstRoll, myDice.secondRoll);
                } else if (myDice.sum == 2 || myDice.sum == 3 || myDice.sum == 5 || myDice.sum == 11 || myDice.sum == 12){
                    printf("\nIt is a %s (%d, %d)\n", myDice.numbertoEnglish(&myDice), myDice.firstRoll, myDice.secondRoll);
                } else {
                    printf("\nIt is a Easy %s (%d, %d)\n", myDice.numbertoEnglish(&myDice), myDice.firstRoll, myDice.secondRoll);
                }
                currentState = BetGetting;
                break; // case PatternShowing
                
            case Winning:
                printf("%s (%d, %d)!! ",myDice.numbertoEnglish(&myDice) ,myDice.firstRoll, myDice.secondRoll);
                printf("You won this around!\nYou won $%d \n", myBank.myBet);
                myBank.winMoney(&myBank);
                initPoints(&myPoints);
                currentState = BetGetting;
                break; // case Winning
                
            case Losing:
                printf("%s (%d, %d)!! ",myDice.numbertoEnglish(&myDice) ,myDice.firstRoll, myDice.secondRoll);
                printf("You lost this around...\nYou lost $%d \n", myBank.myBet);
                myBank.loseMoney(&myBank);
                if (myBank.myMoney == 0) {
                    printf("\nSorry, you are broke.");
                    currentState = Ending;
                    break;
                }
                initPoints(&myPoints);
                currentState = BetGetting;
                break; // case Losing
            
            case Ending:
                printf("\nThank you for playing\n");
                playing = false;
                break; // case Ending
                
        } // State Machine
        
    } // while(playing)

}

void initDataTypes(Bank * refBank, Dice *refDice){
    
    refBank->myMoney    = 1000;
    refBank->myBet      = 0;
    refBank->setBet     = &setBet;
    refBank->loseMoney  = &loseMoney;
    refBank->winMoney   = &winMoney;
    
    refDice->firstRoll   = 0;
    refDice->secondRoll  = 0;
    refDice->sum         = 0;
    refDice->rollNumbers = &rollNumbers;
    refDice->checkPointAndUpdate = &checkPointAndUpdate;
    refDice->numbertoEnglish = &numbertoEnglish;
    
}

void initPoints(Points * refPoints){
    
    refPoints->establishedPoint = false;
    refPoints->losePoint[0] = 2;
    refPoints->losePoint[1] = 3;
    refPoints->losePoint[2] = 12;
    refPoints->winPoint[0]  = 7;
    refPoints->winPoint[1]  = 11;
    refPoints->checkWin     = &checkWin;
    refPoints->checkLose    = &checkLose;
}

bool checkWin(Points * refPoints, Dice * refDice){
    
    if (refDice->sum == refPoints->winPoint[0] ||
        refDice->sum == refPoints->winPoint[1]) {
        return true;
    }
    return false;
}

bool checkLose(Points * refPoints, Dice * refDice){
    
    if (refDice->sum == refPoints->losePoint[0] ||
        refDice->sum == refPoints->losePoint[1] ||
        refDice->sum == refPoints->losePoint[2]) {
        return true;
    }
    return false;
}

void setBet(Bank * refBank){
    char inputNumber[20];
    int i = 0;
    bool flag = false;
    scanf("%s", inputNumber);
    for (i = 0; (i< 20) && (inputNumber[i] != '\0'); i++) {
        flag = isdigit(inputNumber[i]);
        if (!flag) {
            printf("You did not enter a valid input! Please enter your bet again:");
            setBet(refBank);
            break;
        }
    }
    if (flag) {
        refBank->myBet = atoi(inputNumber);
        if (refBank->myBet > refBank->myMoney) {
            printf("You don't have that much money!");
            printf("Your bank only has $%d!\n", refBank->myMoney);
            printf("Enter your bet again:");
            setBet(refBank);
        }
    }
}

void winMoney(Bank * refBank){
    refBank->myMoney += refBank->myBet;
    refBank->myBet = 0;
    
}
void loseMoney(Bank * refBank){
    refBank->myMoney -= refBank->myBet;
    refBank->myBet = 0;
    
}
void rollNumbers(Dice * refDice){
    time_t t;
    srand((unsigned)time(&t));
    refDice->firstRoll = (rand()%6) +1;
    refDice->secondRoll = (rand()%6) +1;
    refDice->sum = refDice->firstRoll + refDice->secondRoll;
}

bool checkPointAndUpdate(Points * refPoints, Dice * refDice){
    if (refPoints->establishedPoint == false) {
        refPoints->establishedPoint = true;
        refPoints->winPoint[0] = refDice->firstRoll + refDice->secondRoll;
        refPoints->winPoint[1] = NOTDICENUMBER;
        refPoints->losePoint[0] = 7;
        refPoints->losePoint[1] = NOTDICENUMBER;
        refPoints->losePoint[2] = NOTDICENUMBER;
        printf("\n%s (%d, %d). New winning point is set\n", refDice->numbertoEnglish(refDice),refDice->firstRoll, refDice->secondRoll);
        return true;
    }
    return false;
}

char* numbertoEnglish(Dice * refDice){
    switch (refDice->sum) {
        case 2:
            return "Snake Eyes";
            break;
            
        case 3:
            return "Ace Deuce";
            break;
            
        case 4:
            return "Four";
            break;
            
        case 5:
            return "Fever Five";
            break;
            
        case 6:
            return "Six";
            break;
            
        case 7:
            return "Steven Out";
            break;
            
        case 8:
            return "Eight";
            break;
            
        case 9:
            return "Nine";
            break;
            
        case 10:
            return "Ten";
            break;
            
        case 11:
            return "Yo-leven";
            break;
            
        case 12:
            return "Boxcars";
            break;
            
        default:
            return "Invalid";
            break;
    }
}


