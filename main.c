#include <stdio.h>
#include "utils.h"

// would love to see a near ideal solution to compare to for learning.

// we use Length of word + 1 because we need to store 0 at end of all strings. Made it global as often reused.
const int wordStringLength = 6;
char userWord[6], randWord[6], correctLetters[6], correctString[6];


// function that copies a random word from the list of valid words to a string
void generateWord(char wordList[][wordStringLength]) {
        int random = rand() % 5756 + 1;
        for (int i = 0; i < wordStringLength + 1; i++)
            randWord[i] = wordList[random][i];
}

// function that checks user word against word list. Not an efficient implementation but works fast due to size of list.
// For larger sets of word would need to optimize but not necessary here.
int wordValidator(char wordList[][wordStringLength]) {
    int rightLetterCount;
    int i;
    // makes sure userword is alphabetical first
    for (i = 0; i < wordStringLength; i++)
        if (userWord[i] < 'a' && userWord[i] > 'z')
            return 0;

    // then checks if it is in the word list
    for (i = 0; i < 5767; i++) {
        rightLetterCount = 0;
        // if a letter is wrong then skip the word
        for (int j = 0; j < wordStringLength; j++) {
            if (userWord[j] != wordList[i][j])
                break;
            else {
                rightLetterCount += 1;
                if (rightLetterCount == wordStringLength - 1)
                    return 1;
            }
        }
    }
    return 0;
}

// function that changes user word to lower case for comparison with word list.
void toLower(char *word) {
    for (int i=0; word[i] != 0; i++) {
        if (word[i] >= 'A' && word[i] <= 'Z')
            word[i] += + 'z' - 'Z';
    }
}

// function that prompts user to input a word. Checks if chars are alphabetical only.
void userInput(char wordList[][wordStringLength], char incorrectLetters[], char hm) {
    // take in user string
    printf("Please enter a 5 letter word.\n");
    scanf("%5s", userWord);
    while ( getchar() != '\n');
    toLower(userWord); // change to lower case

    // ensure user input in word list
    if (!(wordValidator(wordList))) {
        printf("Please enter a valid word!\n");
        userInput(wordList, incorrectLetters, hm);
    }


    // extra checks for harm mode
    // loop through correctWord to ensure user input includes found letters
    for (int i = 0; correctString[i] != 0; i++) {
        if (!(userWord[i] == correctString[i] || correctString[i] == '-')) {
            userInput(wordList, incorrectLetters, hm);
        }
    }

    // make sure used letters are not included
    for (int i = 0; userWord[i] != 0; i++)
        for (int j = 0; j < 21; j++) {
            if (userWord[i] == incorrectLetters[j]) {
                userInput(wordList, incorrectLetters, hm);
            }
        }
    int correctLetterLength = 0;
    for (int i = 0; correctLetters[i] != 0; i++) {
        correctLetterLength++;
    }
    int found = 0;
    // make sure correctly guessed letters are used
    // if the user input word contains a letter from the correct letters list an array value is set to 1
    for (int j = 0; j < correctLetterLength; j++) {
        found = 0;
        for (int i = 0; userWord[i] != 0; i++)
            if (userWord[i] == correctLetters[j] || correctLetters[j] == ' ')
                found = 1;

        if (found == 0 && correctLetterLength != 0) {
            userInput(wordList, incorrectLetters, hm);
        }
    }

}

// checks user input against the randomly generated word from the word list
int wordCheck(char incorrectLetters[], int *incCount) {
    char tempstr[6];

    int i;
    // copying the random word to a temporary string so I can remove letters that are guessed right to avoid double counts
    for (i = 0; i < wordStringLength + 1; i++)
        tempstr[i] = randWord[i];


    int counter= 0, correctCounter = 0, found = 0;
    for (i = 0; i < wordStringLength; i++) {
        if (userWord[i] == tempstr[i]) {
            // checks letters correct and in the right position and stores them
            correctString[i] = userWord[i];
            tempstr[i] = '#'; // this is done to avoid counting double letters.
            correctCounter++;
            continue;
        }
        found = 0; // used to determine incorrect letters
        for (int j = 0; j < wordStringLength; j++) {
            if (userWord[i] == tempstr[j]) {
                // checks if the user input letters are in the randomly generated word and stores them
                correctLetters[counter] = userWord[i];
                tempstr[j] = '#'; // this is done to avoid counting double letters.
                counter++; // used to avoid blank spaces
                found = 1; // if user input letter correct we set to 1
                continue;
            }
        }
        if (found == 0) {
            // if user input letter not correct we add to incorrect string
            // we store across all guesses which is why we need to maintain an external index
            incorrectLetters[*incCount] = userWord[i];
            *incCount += 1;
        }
    }
    return correctCounter; // used to check if player won
}
// function that checks which letters were used per guess and stores them
void letterStatistics(char lettersUsed[], int *letterCountIndex) {
    //loops through user input and array of letters used
    int found; // used to determine if a letter needs to be added to letter bank
    for (int i = 0; userWord[i] != 0; i++) {
        found = 0; // reset found string for each letter
        for (int k = 0; k <= *letterCountIndex; k++) {
            if (userWord[i] == lettersUsed[*letterCountIndex]) {
                // if a letter in the input is already in the array we set to 1 to skip if statement
                found = 1;
            }
        }
        if (found == 0) {
            // if not in letter array we add to it
            lettersUsed[*letterCountIndex] = userWord[i];
            *letterCountIndex += 1;
        }
    }
}
// considered printing histogram from 1-6 and printing * based on games with that many guesses but that was more difficult
// to do than below printing of guesses per game and spec didn't specify which kind was wanted
void histStats(int guessCnt[], int guessIndex) {
    // printing histogram of guesses per game
    printf("Winning guess stats\n");
    printf("-----------------------\n");
        for (int i = 0; i < guessIndex; i++) {
            printf("Game %d :", i + 1);
            for (int j = 0; j < guessCnt[i]; j++)
                printf("*");
        printf("\n");
    }


}

void statistics(int gamesPlayed, int gamesWon, int letterStats[], int letterStatsIndex) {
    // calculating percentage of games won
    double percentageWon = (float)gamesWon / (float)gamesPlayed * 100;

    // print percentage of won games
    printf("%s%.2f%c\n","Percentage of games won: ", percentageWon, '%');

    // print average percentage of each letter across the games
    int totalLettersUsed = 0;
    for (int i = 0; i < letterStatsIndex; i++)
        totalLettersUsed += letterStats[i];


    // calculating average of letters used per game
    float averageLettersUsed = (((float)totalLettersUsed / ((float)gamesPlayed * 26.0)) * 100.0);

    // printing data to user
    printf("--------- STATISTICS ---------\n");
    printf("Games Won: %d Games Played: %d\n", gamesWon, gamesPlayed);
    printf("The average amount of letters used across the games was: %.2f%%\n", averageLettersUsed);

}

int main() {
   char incorrectLetters[21], wordList[5757][wordStringLength], letterCount[26];

    load_word_list(wordList);

    // made guess count and letter stats large enough for 100 games.
    // will run into buffer overflow errors after 100 games.
    int gamesPlayed = 0, gamesWon = 0, guessCountArray[100], letterStats[100],
    guessArrayIndex = 0, incorrectLetterCount = 0, win = 0, guessCounter,
    letterCountIndex = 0, letterStatsIndex = 0; // I feel like I could've used fewer variables. I use a ton.

    char gameLoop = 'y';
    char hardMode;

    int i;
    // setting memory of incorrect letters to spaces. Wanted to use memset be not allowed :(
    for (i = 0; i < 21; i++)
        incorrectLetters[i] = ' ';

    while (gameLoop == 'y') {
        guessCounter = 0;
        // checking if used wishes to play hard mode
        printf("Would you like to play Hard Mode? (y/n)");
        hardMode = getchar();
        while(getchar() != '\n'); // clears buffer

        //generate a random word from the list
        generateWord(wordList);


        // removing any inconsistency by setting the two strings to desired values
        correctLetters[wordStringLength] = 0;
        for (i = 0; correctLetters[i] != 0; i++)
            correctLetters[i] = ' ';

        for (i = 0; i < wordStringLength; i++)
            correctString[i] = '-';
        correctString[wordStringLength] = 0;

    while (guessCounter < 6) {
        //We fill the string to be displayed to user whenever they make a guess

            //get user input
            printf("HardMode: %c\n", hardMode);
            userInput(wordList, incorrectLetters, hardMode);

        guessCounter++;
        // check against random word
        win = wordCheck(incorrectLetters, &incorrectLetterCount);
        // store statistics of letters used
        letterStatistics(letterCount, &letterCountIndex);
        // print results
        printf("%s\n", correctString);
        printf("Correct Letter Bank: %s\n", correctLetters);

        if (win == 6) { // check if player won
            printf("Congratulations! You got the word\n");
            guessCountArray[guessArrayIndex] = guessCounter;
            guessArrayIndex += 1;
            gamesWon++;
            break;
        } else if (guessCounter == 6)
            printf("%s%s\n","You failed to guess the word. It was ", randWord);
    }

       // adding count of letters used in game to letter stats array
        letterStats[letterStatsIndex] = letterCountIndex;
        letterStatsIndex++;
        gamesPlayed++;
        printf("Would you like to play again? (y/n)\n");
         // I don't know why I need to put another one here. A \n is not being taken care of somewhere
        // but all of my getchars look good to me.
        gameLoop = getchar();
        while(getchar() != '\n');
    }
    // displaying game statistics
    statistics(gamesPlayed, gamesWon, letterStats, letterStatsIndex);
     //printing histogram
    histStats(guessCountArray, guessArrayIndex);


	return 0;
}