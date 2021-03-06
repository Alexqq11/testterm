// Win32 builds require Windows.h for Sleep(milliseconds).
// They also require curses.h for the PDCurses library.
#ifdef _WIN32
#   include <Windows.h>
#   include <curses.h>
#   define SLEEP(delay) Sleep(delay/1000)
// Unix builds require ncurses.h for the Ncurses library.
// Unix also requires unistd.h for usleep(microseconds).
// usleep/1000 = Sleep
#else
#   include <ncurses.h>
#   include <unistd.h>
#   define SLEEP(delay) usleep(delay)
#endif
// Thanks to /u/aftli_work for the file consolidation suggestion!

#include <stdlib.h>
#include <time.h>
#include <string.h>
#define OFFSET_LEFT 0
#define OFFSET_RIGHT 20

//#define _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
///
#include <iostream>
using namespace std;
int kbhit(){
    /* Get the current char. */
    int ch = getch();

    /* Returns true if a key has been hit. False if it hasn't. */ 
    if (ch != ERR) {
        return 1;
    } else {
        return 0;
    }
}
void pass();

void slowPrint(char arr[], int size, int line){
    int i = 0;
    
    for(i=0; i<size; i++){  
        /* Print the current character in the current position. */
        mvprintw(line,i,"%c",arr[i]);
        /* Move the cursor to the next position */
        move(line, i+1);
        refresh();
        /* If any keyboard input was recieved, go directly to pass(), otherwise continue */
        if(kbhit()){
            pass();
        }
        SLEEP(20000);
    }
}
int main();
void slowType(char arr[], int size, int line){
    int i = 0;
    for(i=0; i<size; i++){  
        mvprintw(line,i+1,"%c",arr[i]);
        move(line, i+2);
        refresh();
        if(kbhit()){
            pass();
        }
        SLEEP(70000);
    }
}

void passPrint(char arr[], int size, int line){
    int i;
    for(i=0; i<size; i++){  
        mvprintw(line,i,"%c",arr[i]);
        move(line, i+1);
        refresh();
        SLEEP(20000);
    }
}


void printChoices(int hex, char arr[], int line, int offset){
    mvprintw(line,offset,"0x%X", hex);
    int i = 0;
    for(i=0; i<12; i++)
        mvprintw(line,7+offset+i,"%c",arr[i]);
    move(line, 20+offset);
    refresh();
    SLEEP(30000);
}

void file_write_module(char *arr[],int len){
   FILE *fr;  
   fr = fopen ("passwords5.txt", "w");
   int i = 0;
   for (i = 0; i < len; i++){
      fputs(arr[i], fr);
      fputs("\n",fr); 
   } 

}

int getCharLoc(int y, int x){
    /* Left side */
    if(x<19)
        return 12*(y-5)+(x-7);
    /* Right side */
    else
        return 12*(y-5)+(x-27+204);
}



/// Plays the introduction sequence. 
///
void intro(){
    clear();
    SLEEP(250000);
    char arr[] = "WELCOME TO ROBCO INDUSTRIES (TM) TERMLINK";
    
    slowPrint(arr,sizeof(arr), 0);

    move(1, 0);
    refresh();
    SLEEP(30000);
    mvprintw(2,0,"%c", '>');
    move(2,1);
    refresh();
    SLEEP(1500000);

    char arr2[] = "SET TERMINAL/INQUIRE";
    slowType(arr2,sizeof(arr2), 2);
    

    char arr3[] = "RIT-V300";
    slowPrint(arr3,sizeof(arr3), 4);

    mvprintw(6,0,"%c", '>');
    refresh();
    SLEEP(1500000);
    char arr4[] = "SET FILE/PROTECTION=OWNER:RWED ACCOUNTS.F";
    slowType(arr4,sizeof(arr4),6);
    
    mvprintw(7,0,"%c", '>');
    refresh();
    SLEEP(1500000);
    char arr5[] = "SET HALT RESTART/MAINT";
    slowType(arr5,sizeof(arr5),7);
    
    char arr6[] = "Initializing Robco Industries(TM) Boot Agent v2.3.0";
    slowPrint(arr6,sizeof(arr6),9);

    char arr7[] = "RBIOS-4.02.08.00 53EE5.E7.E8";
    slowPrint(arr7,sizeof(arr7),10);

    char arr8[] = "Copyright 2201-22-3 Robco Ind.";
    slowPrint(arr8,sizeof(arr8),11);

    char arr9[] = "Uppermem: 64 KB";
    slowPrint(arr9,sizeof(arr9),12);

    char arr10[] = "Root (5A8)";
    slowPrint(arr10,sizeof(arr10),13);

    char arr11[] = "Maintenance Mode";
    slowPrint(arr11,sizeof(arr11),14);

    mvprintw(16,0,"%c",'>');
    refresh();
    SLEEP(1500000);
    char arr12[] = "RUN DEBUG/ACCOUNTS.F";
    slowType(arr12,sizeof(arr12),16);
    move(16,0);
    refresh();
    SLEEP(50000);
}


int currentCharContains(char arr[],char c){
    int i;
    for(i=0; i<12; i++)
        if(arr[i]==c)
            return 1;
    return 0;
}

long LinesAmount(char * buffer , long length){
        long linesAmount = 0;
        long i; 
        for (i = 0; i < length; i++) {
            if (buffer[i] == '\n') {
                linesAmount++;
            }
        }
        return linesAmount;
    }

char **  ReadPasswordsFromFile(char * filename, int * len){
    FILE* f;
    f = fopen(filename, "r");   /*if (!f){  printf("failed to open file"); exit(42); }*/
    fseek(f, 0, SEEK_END);// length of file
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = 0;
    buffer = (char *)malloc(length);
    if (buffer)
        fread(buffer,sizeof(char), length, f);  /*if (!buffer) { printf("read nothing"); exit(42); }*/
    fclose(f);
    int linesAmount = LinesAmount(buffer,length);
    char** lines = 0;
    lines = (char **)malloc(linesAmount * sizeof(char*));
    int currentLine = 0;
    int previos = 0;
    int i;
    for (i = 0; i < length; i++) {
        if (buffer[i] == '\n') {//  (|| i == length - 1) to read all but we add \n to end off data
            lines[currentLine] = (char *)malloc(i - previos + 1);
            memcpy(lines[currentLine], &buffer[previos], i - previos);
            lines[currentLine][i - previos] = '\0'; // Последний символ в строке 0
            previos = i+1; // +1 для windows (там где \r\n)
            currentLine++;
        }
    }
    //*wordSize = sizeof(lines[0]);
    *len = currentLine;
    free(buffer);
    return lines;
    }

void printPasswordScreenIntro(){
    char prompt[] = "ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL";
    passPrint(prompt,sizeof(prompt),0);
    char prompt2[] = "ENTER PASSWORD NOW";
    passPrint(prompt2, sizeof(prompt2), 1);
    char prompt3[] = "4 ATTEMPT(S) LEFT: * * * *";
    passPrint(prompt3, sizeof(prompt3), 3);
    }

void StartNewScreenNodelayFalse(){
    erase();
    endwin();
    initscr();
    noecho();
    refresh();
    attron(A_BOLD);
    nodelay(stdscr, 0);
    if(has_colors() == 1){
        // Colors
        start_color();
        init_pair(1,COLOR_GREEN,COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }
}
char * GenBinTrash(int BIGSTRING_SIZE){ // rename it GenBinTrash 
    char * bigString = (char *)malloc(BIGSTRING_SIZE * sizeof(char) + 1); // keep attention with +1;
    char randValues[] = "!@#$^*()_-=+\\|/[]{}?\"\':;,.<>";
    int i;
    for(i=0; i<BIGSTRING_SIZE; i++){
        /* Fill bigString with random values */
        bigString[i] = randValues[rand()%29];
    }
    bigString[BIGSTRING_SIZE] = '\0'; 
    return bigString;
}
void GenPasswods(char ** passwordList, char * correctWord, char * bigString, int BIGSTRING_SIZE, int WORD_SIZE, int WORD_POOL_SIZE, int WORDS_CHOSEN){
    int place;                      /* Current place for checking and word insertion*/
    int takenWords[WORDS_CHOSEN];   /* Words already placed in bigString */
    int valid;                      /* 1 if selected word is not already used and 
                                       does not conflict with other words, 0 otherwise */
    int pickedWord = 0;             /* Indicate whether or not we've chosen the correct word */
    int left = WORDS_CHOSEN;        /* # of words that still need to be chosen */
    int i;
    while(left>0){ // pointer to: passwordList , correctWord, bigString ; BIGSTRSIZE,WRDSIZE,WORD_POOL SIZE, WORD_CHOSEN,  
        valid = 1; 
        
        /* Choose a random place in bigString */
        place = rand()%(BIGSTRING_SIZE-WORD_SIZE);
        
        /* Check of any characters there or around it are A-Z */
        for(i=place-1; i<place+WORD_SIZE+1; i++){
            if(bigString[i] > 64 && bigString[i] < 91){
                valid = 0;
            }
        }


        if(valid){
            int wordLoc = rand()%WORD_POOL_SIZE;
            
            /* Check if the word selected has already been selected */
            for(i=0;i<=WORDS_CHOSEN-left;i++)
            {
                if(wordLoc == takenWords[i])
                    valid = 0;
            }

            if(valid){

                /* Add the word to bigString */
                for(i=place; i<place+WORD_SIZE; i++){
                    bigString[i] = passwordList[wordLoc][i-place];
                    /* If this is the first word chosen, it is the correct word. */ 
                    if(!pickedWord)
                        correctWord[i-place] = passwordList[wordLoc][i-place];
                }
                pickedWord = 1;
                left--;
            }
        }
    }
}
void PrintGeneratedScreen(char * bigString){
        /* Generate the hex values on the left sides */
    int arbHex;
    arbHex = (rand() % 200) + 63744;

        /* Print the hex and the filled bigString */
    char temp[12];  
    int current = 0;
    int j = 0;
    int i = 0;
    for(i=5; i<22; i++){
        /* Print left side */
        for(j=0; j<12; j++){
            temp[j] = bigString[j+current];
        }
        printChoices(arbHex,temp,i, OFFSET_LEFT);
        current = current + 12;
        arbHex = arbHex + 12;
    }
    j = 0;
    
    for(i=5; i<22; i++){
        /* Print right side */
        for(j=0; j<12; j++){
            temp[j] = bigString[j+current];
        }
        printChoices(arbHex,temp,i, OFFSET_RIGHT);
        current = current + 12;
        arbHex = arbHex + 12;
    }

} 
void CheckAttempts( int attemptsAmount){
    char a0,a1,a2,a3;

    mvprintw(1,0,"                                 ");
    mvprintw(3,0,"                              "); // NEED TOO ANDERSTUD WHY WE NEED THIS SPACES
    switch(attemptsAmount){
        case 1: 
            mvprintw(3,0,"1 ATTEMPT(S) LEFT: *");
            attron(A_BLINK);
            mvprintw(1,0,"!!! WARNING: LOCKOUT IMNINENT !!!");
            attroff(A_BLINK);
            attron(A_BOLD);
            break;
        case 2: 
            mvprintw(3,0,"2 ATTEMPT(S) LEFT: * *");
            mvprintw(1,0,"ENTER PASSWORD NOW");
            break;
        case 3: 
            mvprintw(3,0,"3 ATTEMPT(S) LEFT: * * *");
            mvprintw(1,0,"ENTER PASSWORD NOW");
            break;
        case 4: 
            mvprintw(3,0,"4 ATTEMPT(S) LEFT: * * * *");
            mvprintw(1,0,"ENTER PASSWORD NOW");
            break;
        case 0: 
            clear();
            /*mvprintw(getmaxx(stdscr)/2,getmaxy(stdscr)/2,"TERMINAL LOCKED");
            mvprintw(getmaxx(stdscr)/2 + 2,getmaxy(stdscr)/2 + 2,"PLEASE CONTACT AN ADMINISTRATOR");*/
            mvprintw(10,20,"TERMINAL LOCKED");
            mvprintw(12,12,"PLEASE CONTACT AN ADMINISTRATOR");
            refresh();
            while(true){ 
                a0=getch();
                a1=getch();
                a2=getch();
                a3=getch();
                if ((a0 == '8') && (a1 == '8') && a2 == '4' && a3 == '1' ){
                   /* endwin();
                    srand ( (unsigned)time(NULL) );
                    initscr();
                    noecho();
                    refresh();
                    attron(A_BOLD);
                    nodelay(stdscr, 1);
                    if(has_colors() == 1){
                    
                        start_color();
                        init_pair(1,COLOR_GREEN,COLOR_BLACK);
                        attron(COLOR_PAIR(1));
                    }
                    intro();
                    pass();
                    */
                    clear();
                    mvprintw(12,12,"POWER OFF");
                    refresh();
                    getch();
                    main();

                }
                else if (a0 == '5' && a1 == '3' && a2 == '4' && a3 == '2'){ //AUTHOR ERR  PATH AT THE NEXT LINE
                endwin(); // 
                exit(0);
            }
        }
    }
    refresh();
}

int  checkSelectedPassword(char * currentChar,char * correctWord, char * output, int WORD_SIZE , int attemptsAmount){
    int i = 0;
    int rightLetters = WORD_SIZE;
                for(i=0;i<WORD_SIZE; i++){
                    if(currentChar[i]!=correctWord[i])
                        rightLetters--;
                }
                if(rightLetters==WORD_SIZE){
                    mvprintw(15,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(15,41+i,"%c",currentChar[i]);
                    }
                    sprintf(output,"Exact match!");
                    mvprintw(16,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(16,41+i,"%c",output[i]);
                    }
                    sprintf(output,"Please wait ");
                    mvprintw(17,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(17,41+i,"%c",output[i]);
                    }
                    sprintf(output,"while system");
                    mvprintw(18,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(18,41+i,"%c",output[i]);
                    }
                    sprintf(output,"is accessed.");
                    mvprintw(19,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(19,41+i,"%c",output[i]);
                    }
                    refresh();
                    SLEEP(3000000);
                    clear();
                    /*mvprintw(getmaxx(stdscr)/2,getmaxy(stdscr)/2,"TERMINAL LOCKED");
                     mvprintw(getmaxx(stdscr)/2 + 2,getmaxy(stdscr)/2 + 2,"PLEASE CONTACT AN ADMINISTRATOR");*/
                    mvprintw(12,12,"FLAG IS: RUCTF_HAVE_A_FUN");
                    refresh();
                    getch();
                    clear();
                    mvprintw(12,12,"POWER OFF");
                    refresh();
                    getch();
                    main();
                    endwin();
                    exit(0);
                    
                }
                else{
                    mvprintw(17,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(17,41+i,"%c",currentChar[i]);
                    }
                    sprintf(output,"Entry denied");
                    mvprintw(18,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(18,41+i,"%c",output[i]);
                    }
                    sprintf(output,"%d/%d correct.",rightLetters,WORD_SIZE);
                    mvprintw(19,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(19,41+i,"%c",output[i]);
                    }
                    attemptsAmount--;
                }
    return attemptsAmount;
}

void pass(){
    static const int BIGSTRING_SIZE = 408;
    int WORD_POOL_SIZE = 0;
    int WORD_SIZE = 5;
    static const int WORDS_CHOSEN = 15; 
    /* Start a new screen where nodelay is false */
    StartNewScreenNodelayFalse();
    
    /* Intro text */
    printPasswordScreenIntro();
    
    /* Generate the string to hold the bracket tricks and words */
    char * bigString  = GenBinTrash(BIGSTRING_SIZE); 

    /* Words from http://www-01.sil.org/linguistics/wordlists/english/wordlist/wordsEn.txt */
    char filename[] = "passwords5.txt"; 

    char** passwordList = ReadPasswordsFromFile(filename, &WORD_POOL_SIZE) ;



    char correctWord[WORD_SIZE];    /* the correct word */
    GenPasswods(passwordList, correctWord, bigString, BIGSTRING_SIZE, WORD_SIZE, WORD_POOL_SIZE, WORDS_CHOSEN);

    /* Create and fill an array to keep track of which brackets were used */
    int i;
    int usedBrackets[BIGSTRING_SIZE];
    for(i=0; i<BIGSTRING_SIZE; i++){
        usedBrackets[i] = 1;
    }
    
    PrintGeneratedScreen(bigString);

    // attention//
    mvprintw(21,40,"%c",'>');
    move(5,7);
    char currentChar[12]; /* Max length chrrentChar could be (total possible length of a bracket trick) */
    currentChar[0] = mvinch(5,7);
    
    // TODO Clear any key presses that may have occurred during this loading sequence 

    int y,x,origy,origx,starty,startx;  /* values that keep track of current yx locations, and original ones */
    int wordLength;                     /* How long a word is */
    int charStart;                      /* where character counting starts for brackets */
    char keyPress;                      /* key pressed by user */
    int charCounter;                    /* counts currentChar - used for incrementing currentChar to print or change it */
    int bracketLength;                  /* length of a bracket trick */
    char endBracket;                    /* the end bracket that corresponds to currentChar[0]; */
    int bracketTricks=0;                /* Total number of bracket tricks used */
    int needsClearing = 0;              /* Whether or not highlights need to be purged */
    int needsClearingMultiLine = 0;     /* Whether or not a multi line highlight needs to be purged */
    char output[12];                    /* Used for side terminal output */
    int attemptsAmount = 4;
    
    while(1){
        getyx(stdscr,y,x);
        
        /* Get attemptsAmount left */
        CheckAttempts(attemptsAmount);
        move(y,x);
        /* Check if highlights need to be purged */
        if(needsClearing){
            charCounter = 0;
            while(charCounter!=bracketLength+1){
                currentChar[charCounter] = mvinch(origy,charStart+charCounter);
                mvprintw(origy,charStart+charCounter,"%c",(int)currentChar[charCounter]);
                charCounter++;
            }
            mvprintw(21,41,"            ",currentChar[0]);
            needsClearing = 0;
            move(y,origx);
        }
        //CleanHighLights(&needsClearing , &charCounter, &bracketLength, currentChar, &origx, &y, &x ,&origy, &charStart);
        if(needsClearingMultiLine){
            charCounter = 0;
            //int timesDown = 0;
            while(charCounter!=wordLength){
                currentChar[charCounter] = mvinch(starty,startx);
                mvprintw(starty,startx,"%c",currentChar[charCounter]);
                charCounter++;
                startx++;
                if(startx==19 || startx==39){
                    startx-=12;
                    starty++;
                }
            }
            mvprintw(21,41,"            ",currentChar[0]);
            needsClearingMultiLine = 0;
            move(y,x);
        }
        /* Clear the char array */
        for(i=0;i<12;i++)
            currentChar[i]=' ';
        currentChar[0] = mvinch(y,x);
        /* Set the new y and x to origy and origx */
        origy = y;
        origx = x;



        /* Check for bracket tricks */
        if((currentChar[0]=='(' || currentChar[0]=='<' || currentChar[0]=='[' || currentChar[0]=='{') && usedBrackets[getCharLoc(y,x)] && bracketTricks<WORDS_CHOSEN){
            charStart = x;
            bracketLength=0;
            while(x!=18 && x!=38){
                x++;
                endBracket = mvinch(y,x);
                bracketLength++;
            if((endBracket == ')' && currentChar[0]=='(') || 
                (endBracket == '>' && currentChar[0]=='<') || 
                (endBracket == ']' && currentChar[0]=='[') || 
                (endBracket == '}' && currentChar[0]=='{')){
                    /* Reprint the bracket trick with highlight */
                    attron(A_STANDOUT);
                    charCounter = 0;
                    while(1){
                        currentChar[charCounter] = mvinch(y,charStart+charCounter);
                        mvprintw(y,charStart+charCounter,"%c",currentChar[charCounter]);
                        if(currentChar[charCounter] == endBracket)
                            break;
                        charCounter++;
                    }
                    attroff(A_STANDOUT);
                    /* Print the bracket trick to output */
                    attron(A_BOLD);
                    for(i=0;i<=charCounter;i++)
                        mvprintw(21,41+i,"%c",(int)currentChar[i]);
                    /* Notify that highlighting will need to be cleared next move */
                    needsClearing = 1;
                }
            }
            if(!((endBracket == ')' && currentChar[0]=='(') || 
                (endBracket == '>' && currentChar[0]=='<') || 
                (endBracket == ']' && currentChar[0]=='[') || 
                (endBracket == '}' && currentChar[0]=='{'))){
                mvprintw(21,41,"%c",currentChar[0]);
            }
        }


        /* Check for letters */
        else if(currentChar[0]>64 && currentChar[0]<91){
            /* Check for letter behind the current location */
            int tempx = x;
            int tempy = y;
            while(bigString[getCharLoc(tempy,tempx)-1]>64 && bigString[getCharLoc(tempy,tempx)-1]<91){
                currentChar[0] = bigString[getCharLoc(tempy,tempx)];
                tempx--;
                if(tempx==6 || tempx==26){
                    tempx+=12;
                    tempy--;
                }
            }
            startx = tempx;
            starty = tempy; /* We'll need the location of the first char for cleaning */
            /* And start there */
            charCounter = 0;
            while(bigString[getCharLoc(tempy,tempx)+1]>64 && bigString[getCharLoc(tempy,tempx)+1]<91){
                currentChar[charCounter] = bigString[getCharLoc(tempy,tempx)];
                charCounter++;
                tempx++;
                if(tempx==19 || tempx==39){
                    tempx-=12;
                    tempy++;
                }
            }
            /* Now currentChar is the String, and charCounter+1 is the length */
            wordLength = charCounter+1;
            /* Reprint the word with highlight */
            tempx = startx;
            tempy = starty;
            attron(A_STANDOUT);
            charCounter = 0;
            while(charCounter!=wordLength){
                currentChar[charCounter] = mvinch(tempy,tempx);
                mvprintw(tempy,tempx,"%c",currentChar[charCounter]);
                charCounter++;
                tempx++;
                if(tempx==19 || tempx==39){
                    tempx-=12;
                    tempy++;
                }
            }
                    attroff(A_STANDOUT);
                    /* Print the word to output */
                    attron(A_BOLD);
                    for(i=0;i<charCounter;i++)
                        mvprintw(21,41+i,"%c",(int)currentChar[i]);
                    /* Notify that highlighting will need to be cleared next move */
                    needsClearingMultiLine = 1;
        }
        /* Nothing was found, print current char */
        else
            mvprintw(21,41,"%c",currentChar[0]);
        
        move(origy,origx);
        refresh();
        
        keyPress = getch();
        getyx(stdscr,y,x);
        if (keyPress == '\033') { //красота кода? не-не, не слышал
            getch(); // skip the [
            switch(getch()) { // the real value
                case 'A':
                    if(y>5)
                        move(y-1,x);
                    break;
                case 'B':
                    if(y<21)
                        move(y+1,x);
                    break;
                case 'C':
                    if(x<38){
                        if(x==18)
                            move(y,27);
                        else
                            move(y,x+1);
                    }
                    break;
                case 'D':
                    if(x>7){
                        if(x==27)
                            move(y,18);
                        else
                            move(y,x-1);
                    }
                   break;
                }
            }

        if(keyPress=='w'){
            if(y>5)
                move(y-1,x);
        }
        if(keyPress=='s'){
            if(y<21)
                move(y+1,x);
        }
        if(keyPress=='a'){
            if(x>7){
                if(x==27)
                    move(y,18);
                else
                    move(y,x-1);
            }
                
        }
        if(keyPress=='d'){
            if(x<38){
                if(x==18)
                    move(y,27);
                else
                    move(y,x+1);
            }
        }
        if(keyPress==3)     /* Ctrl-C */
            exit(0);
        if(keyPress=='\n'){ /* Enter */
            mvprintw(17,40,"              ");
            mvprintw(18,40,"              ");
            mvprintw(19,40,"              ");
            /* If the char is a left bracket */
            if(((currentChar[0]=='(') && currentCharContains(currentChar,')')) || 
               (currentChar[0]=='<' && currentCharContains(currentChar,'>')) || 
               (currentChar[0]=='[' && currentCharContains(currentChar,']')) || 
               (currentChar[0]=='{' && currentCharContains(currentChar,'}'))){
                /* Set the selected bracket as used */
                usedBrackets[getCharLoc(y,x)] = 0;
                /* Increment total bracket tricks used */
                bracketTricks++;
                if(rand()%2==0){ // somethink  interesting it was 5
                    /* 20% chance of allowance replenish */
                    sprintf(output,"Allowance   ");
                    mvprintw(18,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(18,41+i,"%c",output[i]);
                    }
                    sprintf(output,"replenished.");
                    mvprintw(19,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(19,41+i,"%c",output[i]);
                    }
                    attemptsAmount = 4;
                }
                else{
                    /* Remove a dud */
                    int tempx,tempy;
                    pickagain:do{
                        if(rand()%2==0)
                        tempx = (rand()%12)+7;
                    else
                        tempx = (rand()%12)+27;
                    tempy = (rand()%17)+5;
                    } while(!(bigString[getCharLoc(tempy,tempx)]>64 && bigString[getCharLoc(tempy,tempx)]<91));
                    while(bigString[getCharLoc(tempy,tempx)-1]>64 && bigString[getCharLoc(tempy,tempx)-1]<91){
                        tempx--;
                        if(tempx==6 || tempx==26){
                            tempx+=12;
                            tempy--;
                        }
                    }

                    startx = tempx;
                    starty = tempy;
                    
                    charCounter = 0;
                    while(bigString[getCharLoc(tempy,tempx)+1]>64 && bigString[getCharLoc(tempy,tempx)+1]<91){
                        currentChar[charCounter] = bigString[getCharLoc(tempy,tempx)];
                        charCounter++;
                        tempx++;
                        if(tempx==19 || tempx==39){
                            tempx-=12;
                            tempy++;
                        }
                    }
                    /* Check if currentChar = correctWord */
                    int allCorrect=1;
                    for(i=0;i<WORD_SIZE;i++){
                        if(currentChar[i]!=correctWord[i])
                            allCorrect = 0;
                    }
                    if(allCorrect)
                        goto pickagain;
                    
                    tempx = startx;
                    tempy = starty;
                    while(bigString[getCharLoc(tempy,tempx)]>64 && bigString[getCharLoc(tempy,tempx)]<91){
                        mvprintw(tempy,tempx,"%c",'.');
                        bigString[getCharLoc(tempy,tempx)] = '.';
                        tempx++;
                        if(tempx==19 || tempx==39){
                            tempx-=12;
                            tempy++;
                        }
                    }
                    
                    sprintf(output,"Dud removed.");
                    mvprintw(19,40,">");
                    for(i=0;i<12;i++){
                        mvprintw(19,41+i,"%c",output[i]);
                    }
            
                }
            }
            /* Else compare it to the correct word */
            else{
               attemptsAmount = checkSelectedPassword( currentChar, correctWord, output, WORD_SIZE , attemptsAmount);

            }
            move(y,x);
        }   
        refresh();
        
    }

    endwin();
    exit(0);
}




int main(){

    // TODO  get input from args here. Pass it to pass

    srand ( (unsigned)time(NULL) );
    initscr();
    noecho();
    refresh();
    attron(A_BOLD);
    nodelay(stdscr, 1);
    if(has_colors() == 1){
        /* Colors */
        start_color();
        init_pair(1,COLOR_GREEN,COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }

    intro();
    pass();

    
    return 0;
}
