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

#define _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
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
/*char**  PasswordFileReader(char passwords[]){
    FILE *fr;
    char line[6];
    char** strs = malloc(sizeof(char*)*7000);
    fr = fopen("passwords5.txt", "rt");      //TODO OPENING CUSTOM FILE WITH PASSWORD READ SPECIFICATION 
    int i = 0;
    while(fgets(line, 6, fr) != NULL)
   { // some problems to test
        if (i < 7 ){ 
        printw("%s",line);
        }
        strs[i] = malloc(strlen(line) + 1);
        strcpy(strs[i], line);
        i++;
        if (i < 10) {
        printw("%s\n", strs[i]);
        }
   }
   /*strs[i] = malloc(strlen("foo") + 1);
    strcpy(strs[i], "foo");*/
   /*fclose(fr);
   return strs;  
} */

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

char **  ReadPasswords(){
    FILE* f;
    f = fopen("passwords5.txt", "r");   /*if (!f){  printf("failed to open file"); exit(42); }*/
    fseek(f, 0, SEEK_END);// length of file
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = 0;
    buffer = (char *)malloc(length);
    if (buffer)
        fread(buffer,sizeof(char), length, f);  /*if (!buffer) { printf("read nothing"); exit(42); }*/
    fclose(f);
    long linesAmount = LinesAmount(buffer,length);
    char** lines = 0;
    lines = (char **)malloc(linesAmount * sizeof(char*));
    long currentLine = 0;
    long previos = 0;
    long i;
    for (i = 0; i < length; i++) {
        if (buffer[i] == '\n') {//  (|| i == length - 1) to read all but we add \n to end off data
            lines[currentLine] = (char *)malloc(i - previos + 1);
            memcpy(lines[currentLine], &buffer[previos], i - previos);
            lines[currentLine][i - previos] = '\0'; // Последний символ в строке 0
            previos = i+1; // +1 для windows (там где \r\n)
            currentLine++;
        }
    }
    free(buffer);
    return lines;
}

void pass(){
    static const int BIGSTRING_SIZE = 408;


    /* Start a new screen where nodelay is false */
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
    
    /* Intro text */
    char prompt[] = "ROBCO INDUSTRIES (TM) TERMLINK PROTOCOL";
    passPrint(prompt,sizeof(prompt),0);
    
    char prompt2[] = "ENTER PASSWORD NOW";
    passPrint(prompt2, sizeof(prompt2), 1);
    char prompt3[] = "4 ATTEMPT(S) LEFT: * * * *";
    passPrint(prompt3, sizeof(prompt3), 3);
    
    /* Generate the hex values on the left sides */
    int arbHex;
    arbHex = (rand() % 200) + 63744;
    
    /* Generate the string to hold the bracket tricks and words */
    char bigString [BIGSTRING_SIZE];
    char randValues[] = "!@#$%^*()_-=+\\|/[]{}?\"\':;,.<>";
    int i;
    for(i=0; i<BIGSTRING_SIZE; i++){
        /* Fill bigString with random values */
        bigString[i] = randValues[rand()%29];
    }
    
    /* Words from http://www-01.sil.org/linguistics/wordlists/english/wordlist/wordsEn.txt */
        char** wordArr = ReadPasswords();/*{
            "AAHED", "ABACI", "ABACK", "ABAFT", "ABASE", "ABASH", "ABATE", 
            "ABBES", "ABBEY", "ABBOT", "ABEAM", "ABETS", "ABHOR", "ABIDE", 
            "ABLER", "ABLES", "ABNER", "ABODE", "ABOIL", "ABORT", "ABOUT", 
            "ABOVE", "ABUSE", "ABUTS", "ABUZZ", "ABYES", "ABYSM", "ABYSS", 
            "ACCTS", "ACERB", "ACHED", "ACHES", "ACHOO", "ACIDS", "ACIDY", 
            "ACING", "ACMES", "ACNED", "ACNES", "ACOIN", "ACORN", "ACRED", 
            "ACRES", "ACRID", "ACTED", "ACTIN", "ACTOR", "ACUTE", "ADAGE", 
            "ADAMS", "ADAPT", "ADDAX", "ADDED", "ADDER", "ADDLE", "ADEPT", 
            "ADIEU", "ADIOS", "ADITS", "ADMAN", "ADMEN", "ADMIN", "ADMIT", 
            "ADMIX", "ADOBE", "ADOLF", "ADOPT", "ADORE", "ADORN", "ADOZE", 
            "ADULT", "ADYTA", "ADZES", "AEGIS", "AEONS", "AERIE", "AESOP", 
            "AFARS", "AFFIX", "AFIRE", "AFOOT", "AFORE", "AFOUL", "AFRIT", 
            "AFROS", "AFTER", "AGAIN", "AGAMA", "AGAPE", "AGARS", "AGATE", 
            "AGAVE", "AGAZE", "AGENT", "AGERS", "AGGIE", "AGHAS", "AGILE", 
            "AGING", "AGIST", "AGLEE", "AGLET", "AGLEY", "AGLOW", "AGONE", 
            "AGONS", "AGONY", "AGORA", "AGREE", "AGRIC", "AGUES", "AHEAD", 
            "AHEMS", "AHOLD", "AIDED", "AIDER", "AIDES", "AILED", "AIMED", 
            "AIMER", "AINUS", "AIRED", "AIRER", "AISLE", "AITCH", "AJIVA", 
            "AKRON", "ALACK", "ALAMO", "ALANS", "ALARM", "ALARY", "ALATE", 
            "ALBUM", "ALDER", "ALECS", "ALEFS", "ALEPH", "ALERT", "ALFAS", 
            "ALGAE", "ALGAL", "ALGAS", "ALGID", "ALGIN", "ALIAS", "ALIBI", 
            "ALICE", "ALIEN", "ALIGN", "ALIKE", "ALINE", "ALIVE", "ALKYD", 
            "ALKYL", "ALLAH", "ALLAY", "ALLEN", "ALLER", "ALLEY", "ALLOT", 
            "ALLOW", "ALLOY", "ALOES", "ALOFT", "ALOHA", "ALONE", "ALONG", 
            "ALOOF", "ALOUD", "ALPHA", "ALTAR", "ALTER", "ALTHO", "ALTOS", 
            "ALUMS", "ALWAY", "AMAHS", "AMAIN", "AMASS", "AMAZE", "AMBER", 
            "AMBLE", "AMEBA", "AMEER", "AMEND", "AMENS", "AMENT", "AMIAS", 
            "AMICE", "AMICI", "AMIDE", "AMIDS", "AMIES", "AMIGO", "AMINO", 
            "AMIRE", "AMIRS", "AMISH", "AMISS", "AMITY", "AMMOS", "AMOKS", 
            "AMOLE", "AMONG", "AMORT", "AMOUR", "AMOVE", "AMPLE", "AMPLY", 
            "AMPUL", "AMUCK", "AMUSE", "AMYLS", "ANDES", "ANDRE", "ANEAR", 
            "ANELE", "ANENT", "ANGAS", "ANGEL", "ANGER", "ANGLE", "ANGLO", 
            "ANGRY", "ANGST", "ANGUS", "ANILE", "ANILS", "ANIMA", "ANIMO", 
            "ANION", "ANISE", "ANKHS", "ANKLE", "ANKUS", "ANNAL", "ANNAS", 
            "ANNAT", "ANNEX", "ANNIE", "ANNOY", "ANNUL", "ANNUM", "ANODE", 
            "ANOIA", "ANOLE", "ANOMY", "ANTED", "ANTES", "ANTIC", "ANTIS", 
            "ANTRA", "ANTRE", "ANVIL", "AORTA", "AOUAD", "APACE", "APART", 
            "APEAK", "APEEK", "APERS", "APERY", "APHID", "APHIS", "APIAN", 
            "APING", "APISH", "APIUM", "APNEA", "APORT", "APPAL", "APPLE", 
            "APPLY", "APRES", "APRIL", "APRON", "APSES", "APTER", "APTLY", 
            "AQUAE", "AQUAS", "ARABS", "ARAKS", "ARBOR", "ARCED", "ARCUS", 
            "ARDOR", "AREAL", "AREAS", "ARENA", "ARETE", "ARGAL", "ARGLE", 
            "ARGON", "ARGOT", "ARGUE", "ARGUS", "ARHAT", "ARIAS", "ARIEL", 
            "ARIES", "ARILS", "ARISE", "ARITH", "ARMED", "ARMER", "ARMOR", 
            "AROMA", "AROSE", "ARRAS", "ARRAY", "ARROW", "ARSES", "ARSIS", 
            "ARSON", "ARUMS", "ARYAN", "ARYLS", "ASCII", "ASCOT", "ASHED", 
            "ASHEN", "ASHES", "ASIAN", "ASIDE", "ASKED", "ASKER", "ASKEW", 
            "ASPCA", "ASPEN", "ASPER", "ASPIC", "ASSAM", "ASSAY", "ASSES", 
            "ASSET", "ASSOC", "ASTER", "ASTIR", "ASYLA", "ASYNC", "ATAXY", 
            "ATILT", "ATLAS", "ATMAN", "ATMAS", "ATOLL", "ATOMS", "ATOMY", 
            "ATONE", "ATRIA", "ATRIP", "ATTAR", "ATTIC", "AUDAD", "AUDIO", 
            "AUDIT", "AUGER", "AUGHT", "AUGUR", "AUNTS", "AUNTY", "AURAE", 
            "AURAL", "AURAS", "AURIC", "AURUM", "AUTOS", "AUTRE", "AUXIN", 
            "AVAIL", "AVAST", "AVERS", "AVERT", "AVIAN", "AVION", "AVISO", 
            "AVOID", "AVOWS", "AWACS", "AWAIT", "AWAKE", "AWARD", "AWARE", 
            "AWASH", "AWFUL", "AWING", "AWNED", "AWOKE", "AWOLS", "AXELS", 
            "AXIAL", "AXILS", "AXING", "AXIOM", "AXLED", "AXLES", "AXMAN", 
            "AXMEN", "AXONE", "AXONS", "AYAHS", "AZIDE", "AZIDO", "AZINE", 
            "AZOIC", "AZOLE", "AZONS", "AZOTE", "AZOTH", "AZTEC", "AZURE", 
            "BAAED", "BAALS", "BABAS", "BABEL", "BABES", "BABKA", "BABOO", 
            "BABUL", "BABUS", "BACCA", "BACKS", "BACON", "BADDY", "BADGE", 
            "BADLY", "BAGEL", "BAGGY", "BAHTS", "BAILS", "BAIRN", "BAITS", 
            "BAIZE", "BAKED", "BAKER", "BAKES", "BALDS", "BALED", "BALER", 
            "BALES", "BALKS", "BALKY", "BALLO", "BALLS", "BALLY", "BALMS", 
            "BALMY", "BALSA", "BANAL", "BANCO", "BANDS", "BANDY", "BANED", 
            "BANES", "BANGS", "BANJO", "BANKS", "BANNS", "BANTU", "BARBS", 
            "BARDS", "BARED", "BARER", "BARES", "BARFS", "BARGE", "BARIC", 
            "BARKS", "BARKY", "BARMY", "BARNS", "BARNY", "BARON", "BARRE", 
            "BASAL", "BASED", "BASER", "BASES", "BASIC", "BASIL", "BASIN", 
            "BASIS", "BASKS", "BASSI", "BASSO", "BASSY", "BASTE", "BASTS", 
            "BATCH", "BATED", "BATES", "BATHE", "BATHS", "BATIK", "BATON", 
            "BATTS", "BATTY", "BAUDS", "BAULK", "BAWDS", "BAWDY", "BAWLS", 
            "BAYED", "BAYOU", "BAZAR", "BEACH", "BEADS", "BEADY", "BEAKS", 
            "BEAKY", "BEAMS", "BEAMY", "BEANO", "BEANS", "BEARD", "BEARS", 
            "BEAST", "BEATS", "BEAUS", "BEAUT", "BEAUX", "BEBOP", "BECKS", 
            "BEDEW", "BEDIM", "BEECH", "BEEFS", "BEEFY", "BEEPS", "BEERS", 
            "BEERY", "BEETS", "BEFIT", "BEFOG", "BEGAN", "BEGAT", "BEGET", 
            "BEGIN", "BEGOT", "BEGUM", "BEGUN", "BEIGE", "BEIGY", "BEING", 
            "BELAY", "BELCH", "BELIE", "BELLE", "BELLI", "BELLO", "BELLS", 
            "BELLY", "BELOW", "BELTS", "BEMAS", "BEMIX", "BENCH", "BENDS", 
            "BENDY", "BENES", "BENIN", "BENNY", "BENTS", "BERET", "BERGH", 
            "BERGS", "BERMS", "BERRY", "BERTH", "BERYL", "BESET", "BESOM", 
            "BESOT", "BESTS", "BETAS", "BETEL", "BETES", "BETHS", "BETTA", 
            "BETTY", "BEVEL", "BEWIG", "BEZEL", "BHANG", "BIALY", "BIBBS", 
            "BIBLE", "BIDDY", "BIDED", "BIDER", "BIDES", "BIDET", "BIENS", 
            "BIERS", "BIFFS", "BIFFY", "BIFID", "BIGHT", "BIGLY", "BIGOT", 
            "BIJOU", "BIKED", "BIKER", "BIKES", "BILBO", "BILES", "BILGE", 
            "BILGY", "BILKS", "BILLS", "BILLY", "BIMAH", "BINAL", "BINDS", 
            "BINES", "BINGE", "BINGO", "BINTS", "BIOME", "BIONT", "BIOTA", 
            "BIPED", "BIPOD", "BIRCH", "BIRDS", "BIRTH", "BISON", "BITCH", 
            "BITER", "BITES", "BITSY", "BITTE", "BITTS", "BITTY", "BLABS", 
            "BLACK", "BLADE", "BLAHS", "BLAIN", "BLAME", "BLANC", "BLAND", 
            "BLANK", "BLARE", "BLASE", "BLAST", "BLATS", "BLAZE", "BLEAK", 
            "BLEAR", "BLEAT", "BLEED", "BLEEP", "BLEND", "BLENT", "BLESS", 
            "BLEST", "BLIMP", "BLIMY", "BLIND", "BLINI", "BLINK", "BLIPS", 
            "BLISS", "BLITZ", "BLOAT", "BLOBS", "BLOCK", "BLOCS", "BLOKE", 
            "BLOND", "BLOOD", "BLOOM", "BLOOP", "BLOTS", "BLOWN", "BLOWS", 
            "BLOWY", "BLUED", "BLUER", "BLUES", "BLUET", "BLUEY", "BLUFF", 
            "BLUNT", "BLURB", "BLURS", "BLURT", "BLUSH", "BOARD", "BOARS", 
            "BOAST", "BOATS", "BOBBY", "BOCCE", "BOCCI", "BOCHE", "BOCKS", 
            "BODED", "BODES", "BOERS", "BOFFO", "BOFFS", "BOGEY", "BOGGY", 
            "BOGIE", "BOGLE", "BOGUS", "BOILS", "BOISE", "BOLAS", "BOLES", 
            "BOLLS", "BOLOS", "BOLTS", "BOLUS", "BOMBE", "BOMBS", "BONDS", 
            "BONED", "BONER", "BONES", "BONEY", "BONGO", "BONGS", "BONNY", 
            "BONOS", "BONUM", "BONUS", "BONZE", "BOOBS", "BOOBY", "BOOED", 
            "BOOKS", "BOOMS", "BOOMY", "BOONS", "BOORS", "BOOST", "BOOTH", 
            "BOOTS", "BOOTY", "BOOZE", "BOOZY", "BORAX", "BORED", "BORES", 
            "BORIC", "BORNE", "BORON", "BORTS", "BORTY", "BORTZ", "BOSKS", 
            "BOSKY", "BOSOM", "BOSON", "BOSSA", "BOSSY", "BOSUN", "BOTCH", 
            "BOUGH", "BOULE", "BOUND", "BOURG", "BOURN", "BOUSE", "BOUSY", 
            "BOUTS", "BOVID", "BOWED", "BOWEL", "BOWER", "BOWIE", "BOWLS", 
            "BOWSE", "BOXED", "BOXER", "BOXES", "BOYOS", "BOZOS", "BRACE", 
            "BRACT", "BRADS", "BRAES", "BRAGS", "BRAID", "BRAIL", "BRAIN", 
            "BRAKE", "BRAKY", "BRAND", "BRANS", "BRASH", "BRASS", "BRATS", 
            "BRAVE", "BRAVO", "BRAWL", "BRAWN", "BRAYS", "BRAZE", "BREAD", 
            "BREAK", "BREAM", "BREDE", "BREED", "BRENT", "BREVE", "BREVI", 
            "BREWS", "BRIAN", "BRIAR", "BRIBE", "BRICK", "BRIDE", "BRIEF", 
            "BRIER", "BRIES", "BRIGS", "BRILL", "BRIMS", "BRINE", "BRING", 
            "BRINK", "BRINY", "BRIOS", "BRISK", "BROAD", "BROCK", "BROIL", 
            "BROKE", "BROMO", "BRONC", "BRONX", "BROOD", "BROOK", "BROOM", 
            "BROTH", "BROWN", "BROWS", "BRUCE", "BRUIN", "BRUIT", "BRUNT", 
            "BRUSH", "BRUSK", "BRUTE", "BRYAN", "BUBBY", "BUCHU", "BUCKO", 
            "BUCKS", "BUDDY", "BUDGE", "BUFFO", "BUFFS", "BUFFY", "BUGGY", 
            "BUGLE", "BUICK", "BUILD", "BUILT", "BULBS", "BULGE", "BULGY", 
            "BULKS", "BULKY", "BULLS", "BULLY", "BUMPS", "BUMPY", "BUNCH", 
            "BUNCO", "BUNDS", "BUNGS", "BUNKO", "BUNKS", "BUNNS", "BUNNY", 
            "BUNTS", "BUOYS", "BURGH", "BURGS", "BURIN", "BURKE", "BURLS", 
            "BURLY", "BURMA", "BURNS", "BURNT", "BURPS", "BURRO", "BURRS", 
            "BURRY", "BURSA", "BURSE", "BURST", "BUSBY", "BUSED", "BUSES", 
            "BUSHY", "BUSTS", "BUSTY", "BUTCH", "BUTTE", "BUTTS", "BUTTY", 
            "BUTYL", "BUXOM", "BUYER", "BWANA", "BYLAW", "BYRES", "BYRON", 
            "BYTES", "BYWAY", "CABAL", "CABBY", "CABER", "CABIN", "CABLE", 
            "CABOB", "CABOT", "CACAO", "CACHE", "CACTI", "CADDY", "CADES", 
            "CADET", "CADGE", "CADGY", "CADIS", "CADRE", "CAFES", "CAGED", 
            "CAGER", "CAGES", "CAGEY", "CAINS", "CAIRN", "CAIRO", "CAJON", 
            "CAJUN", "CAKED", "CAKES", "CALFS", "CALIF", "CALIX", "CALKS", 
            "CALLA", "CALLI", "CALLS", "CALMS", "CALOR", "CALVE", "CALYX", 
            "CAMEL", "CAMEO", "CAMPI", "CAMPO", "CAMPS", "CAMPY", "CANAL", 
            "CANDY", "CANED", "CANER", "CANES", "CANNA", "CANNY", "CANOE", 
            "CANON", "CANST", "CANTO", "CANTS", "CANTY", "CAPED", "CAPER", 
            "CAPES", "CAPON", "CAPOS", "CAPPY", "CAPUT", "CARAT", "CARBO", 
            "CARDS", "CARED", "CARER", "CARES", "CARET", "CARGO", "CARLO", 
            "CARNE", "CARNY", "CAROB", "CAROL", "CAROM", "CARPE", "CARPI", 
            "CARPS", "CARRY", "CARTE", "CARTS", "CARVE", "CASAS", "CASED", 
            "CASES", "CASKS", "CASTE", "CASTS", "CASUS", "CATCH", "CATER", 
            "CATES", "CATHY", "CATTY", "CAULK", "CAULS", "CAUSE", "CAVED", 
            "CAVER", "CAVES", "CAVIE", "CAVIL", "CAWED", "CEASE", "CECAL", 
            "CECIL", "CECUM", "CEDAR", "CEDED", "CEDER", "CEDES", "CEILS", 
            "CELEB", "CELLI", "CELLO", "CELLS", "CELTS", "CENSE", "CENTO", 
            "CENTS", "CERED", "CERES", "CERIA", "CHADS", "CHAFE", "CHAFF", 
            "CHAIN", "CHAIR", "CHALK", "CHAMP", "CHAMS", "CHANG", "CHANT", 
            "CHAOS", "CHAPS", "CHAPT", "CHARD", "CHARE", "CHARM", "CHARS", 
            "CHART", "CHARY", "CHASE", "CHASM", "CHATS", "CHAWS", "CHEAP", 
            "CHEAT", "CHECK", "CHEEK", "CHEEP", "CHEER", "CHEFS", "CHELA", 
            "CHERT", "CHESS", "CHEST", "CHEVY", "CHEWS", "CHEWY", "CHIAO", 
            "CHIAS", "CHICK", "CHICO", "CHICS", "CHIDE", "CHIEF", "CHIEL", 
            "CHILD", "CHILE", "CHILI", "CHILL", "CHIME", "CHIMP", "CHINA", 
            "CHINE", "CHINK", "CHINO", "CHINS", "CHIPS", "CHIRK", "CHIRP", 
            "CHITS", "CHIVE", "CHIVY", "CHOCK", "CHOIR", "CHOKE", "CHOKY", 
            "CHOMP", "CHOPS", "CHORD", "CHORE", "CHOSE", "CHOWS", "CHUBS", 
            "CHUCK", "CHUFF", "CHUGS", "CHUMP", "CHUMS", "CHUNK", "CHURL", 
            "CHURN", "CHUTE", "CHYME", "CIDER", "CIGAR", "CILIA", "CINCH", 
            "CINES", "CIONS", "CIRCA", "CIRCE", "CISTS", "CITED", "CITER", 
            "CITES", "CIVET", "CIVIC", "CIVIL", "CIVVY", "CLACK", "CLADS", 
            "CLAGS", "CLAIM", "CLAIR", "CLAMP", "CLAMS", "CLANG", "CLANK", 
            "CLANS", "CLAPS", "CLAPT", "CLARK", "CLASH", "CLASP", "CLASS", 
            "CLAUS", "CLAVE", "CLAWS", "CLAYS", "CLEAN", "CLEAR", "CLEAT", 
            "CLEFS", "CLEFT", "CLEPE", "CLEPT", "CLERK", "CLEWS", "CLICK", 
            "CLIFF", "CLIFT", "CLIMB", "CLIME", "CLINE", "CLING", "CLINK", 
            "CLIPS", "CLIPT", "CLOAK", "CLOCK", "CLODS", "CLOGS", "CLOMB", 
            "CLOMP", "CLONE", "CLONK", "CLOPS", "CLOSE", "CLOTH", "CLOTS", 
            "CLOUD", "CLOUT", "CLOVE", "CLOWN", "CLOYS", "CLUBS", "CLUCK", 
            "CLUED", "CLUES", "CLUMP", "CLUNG", "CLUNK", "COACH", "COACT", 
            "COALS", "COAST", "COATI", "COATS", "COBBY", "COBOL", "COBRA", 
            "COCAS", "COCCI", "COCKS", "COCKY", "COCOA", "COCOS", "CODAL", 
            "CODAS", "CODED", "CODER", "CODES", "CODEX", "CODON", "COEDS", 
            "COFFS", "COHEN", "COHOS", "COIFS", "COIGN", "COILS", "COINS", 
            "COIRS", "COKED", "COKES", "COLAS", "COLDS", "COLES", "COLIC", 
            "COLIN", "COLLY", "COLON", "COLOR", "COLTS", "COMAS", "COMBE", 
            "COMBO", "COMBS", "COMER", "COMES", "COMET", "COMFY", "COMIC", 
            "COMMA", "COMME", "COMMY", "COMPS", "COMPT", "COMTE", "CONCH", 
            "CONDO", "CONED", "CONES", "CONEY", "CONGA", "CONGO", "CONIC", 
            "CONKS", "CONKY", "CONNS", "CONTE", "CONTO", "CONUS", "COOCH", 
            "COOED", "COOEE", "COOER", "COOEY", "COOKS", "COOKY", "COOLS", 
            "COOLY", "COOMB", "COONS", "COOPS", "COOPT", "COOTS", "COPAL", 
            "COPED", "COPER", "COPES", "COPRA", "COPSE", "CORAL", "CORDS", 
            "CORED", "CORER", "CORES", "CORGI", "CORKS", "CORKY", "CORMS", 
            "CORNS", "CORNU", "CORNY", "CORPS", "CORSE", "COSEC", "COSET", 
            "COSEY", "COSIE", "COSMO", "COSTS", "COTAN", "COTED", "COTES", 
            "COTTA", "COUCH", "COUGH", "COULD", "COUNT", "COUPE", "COUPS", 
            "COURT", "COUTH", "COVED", "COVEN", "COVER", "COVES", "COVET", 
            "COVEY", "COWED", "COWER", "COWLS", "COWRY", "COYER", "COYLY", 
            "COYPU", "COZEN", "COZES", "COZEY", "COZIE", "CRAAL", "CRABS", 
            "CRACK", "CRAFT", "CRAGS", "CRAMP", "CRAMS", "CRANE", "CRANK", 
            "CRAPE", "CRAPS", "CRASH", "CRASS", "CRATE", "CRAVE", "CRAWL", 
            "CRAWS", "CRAZE", "CRAZY", "CREAK", "CREAM", "CREDO", "CREED", 
            "CREEK", "CREEL", "CREEP", "CREES", "CREME", "CREPE", "CREPT", 
            "CREPY", "CRESS", "CREST", "CRETE", "CREWS", "CRIBS", "CRICK", 
            "CRIED", "CRIER", "CRIES", "CRIME", "CRIMP", "CRISP", "CROAK", 
            "CROCI", "CROCK", "CROFT", "CRONE", "CRONY", "CROOK", "CROON", 
            "CROPS", "CROSS", "CROUP", "CROWD", "CROWN", "CROWS", "CRUDE", 
            "CRUDS", "CRUEL", "CRUET", "CRUMB", "CRUMP", "CRUSE", "CRUSH", 
            "CRUST", "CRYPT", "CUBAN", "CUBBY", "CUBED", "CUBER", "CUBES", 
            "CUBIC", "CUBIT", "CUDDY", "CUFFS", "CUING", "CUISH", "CUKES", 
            "CULLS", "CULLY", "CULMS", "CULPA", "CULTS", "CUMIN", "CUNNI", 
            "CUNTS", "CUPID", "CUPPA", "CUPPY", "CURBS", "CURDS", "CURDY", 
            "CURED", "CURER", "CURES", "CURIA", "CURIE", "CURIO", "CURLS", 
            "CURLY", "CURRY", "CURSE", "CURST", "CURVE", "CURVY", "CUSHY", 
            "CUSPS", "CUTER", "CUTES", "CUTEY", "CUTIE", "CUTIN", "CUTIS", 
            "CUTTY", "CUTUP", "CYANS", "CYCAD", "CYCLE", "CYCLO", "CYMES", 
            "CYNIC", "CYSTS", "CZARS", "CZECH", "DACES", "DACHA", "DADAS", 
            "DADDY", "DADOS", "DAFFY", "DAGOS", "DAILY", "DAIRY", "DAISY", 
            "DALES", "DALLY", "DAMES", "DAMNS", "DAMPS", "DANCE", "DANDY", 
            "DANES", "DANGS", "DANTE", "DARED", "DARER", "DARES", "DARKS", 
            "DARKY", "DARNS", "DARTS", "DASHY", "DATED", "DATER", "DATES", 
            "DATUM", "DAUBS", "DAUBY", "DAUNT", "DAVID", "DAVIS", "DAVIT", 
            "DAWNS", "DAZED", "DAZES", "DEADS", "DEAIR", "DEALS", "DEALT", 
            "DEANS", "DEARS", "DEARY", "DEASH", "DEATH", "DEBAR", "DEBIT", 
            "DEBTS", "DEBUG", "DEBUT", "DECAL", "DECAY", "DECKS", "DECOR", 
            "DECOY", "DECRY", "DEEDS", "DEEDY", "DEEMS", "DEEPS", "DEERS", 
            "DEFAT", "DEFER", "DEFOG", "DEGAS", "DEGUM", "DEICE", "DEIFY", 
            "DEIGN", "DEISM", "DEIST", "DEITY", "DELAY", "DELED", "DELES", 
            "DELFT", "DELHI", "DELIS", "DELLS", "DELLY", "DELTA", "DELVE", 
            "DEMIT", "DEMOB", "DEMON", "DEMOS", "DEMUR", "DENIM", "DENSE", 
            "DENTS", "DEPOT", "DEPTH", "DERAT", "DERAY", "DERBY", "DERMA", 
            "DERMS", "DESEX", "DESKS", "DETER", "DEUCE", "DEVAS", "DEVIL", 
            "DEVON", "DEWAX", "DEWED", "DEXES", "DHOLE", "DHOTI", "DHOWS", 
            "DIALS", "DIANA", "DIANE", "DIARY", "DIAZO", "DICED", "DICER", 
            "DICES", "DICEY", "DICKS", "DICKY", "DICOT", "DICTA", "DIDOS", 
            "DIDST", "DIETS", "DIGHT", "DIGIT", "DIKED", "DIKER", "DIKES", 
            "DILDO", "DILLS", "DILLY", "DIMER", "DIMES", "DIMLY", "DINAR", 
            "DINED", "DINER", "DINES", "DINGO", "DINGS", "DINGY", "DINKY", 
            "DINTS", "DIODE", "DIPPY", "DIRER", "DIRGE", "DIRKS", "DIRTS", 
            "DIRTY", "DISCO", "DISCS", "DISHY", "DISKS", "DISTR", "DITCH", 
            "DITES", "DITTO", "DITTY", "DIVAN", "DIVAS", "DIVED", "DIVER", 
            "DIVES", "DIVOT", "DIVVY", "DIXIE", "DIXIT", "DIZZY", "DJINN", 
            "DJINS", "DOCKS", "DODGE", "DODGY", "DODOS", "DOERS", "DOEST", 
            "DOETH", "DOFFS", "DOGES", "DOGEY", "DOGGO", "DOGGY", "DOGIE", 
            "DOGMA", "DOILY", "DOING", "DOJOS", "DOLCE", "DOLCI", "DOLED", 
            "DOLES", "DOLLS", "DOLLY", "DOLOR", "DOLTS", "DOMED", "DOMES", 
            "DON'T", "DONEE", "DONGS", "DONNA", "DONNE", "DONOR", "DONUT", 
            "DOOMS", "DOORS", "DOOZY", "DOPED", "DOPER", "DOPES", "DOPEY", 
            "DORIC", "DORIS", "DORMS", "DORMY", "DORSA", "DORSI", "DOSED", 
            "DOSER", "DOSES", "DOTED", "DOTER", "DOTES", "DOTTY", "DOUBT", 
            "DOUCE", "DOUGH", "DOUSE", "DOVER", "DOVES", "DOWDY", "DOWEL", 
            "DOWER", "DOWNS", "DOWNY", "DOWRY", "DOWSE", "DOXIE", "DOYEN", 
            "DOYLY", "DOZED", "DOZEN", "DOZER", "DOZES", "DRABS", "DRAFT", 
            "DRAGS", "DRAIN", "DRAKE", "DRAMA", "DRAMS", "DRANK", "DRAPE", 
            "DRATS", "DRAVE", "DRAWL", "DRAWN", "DRAWS", "DRAYS", "DREAD", 
            "DREAM", "DREAR", "DRECK", "DREGS", "DREKS", "DRESS", "DREST", 
            "DRIBS", "DRIED", "DRIER", "DRIES", "DRIFT", "DRILL", "DRILY", 
            "DRINK", "DRIPS", "DRIPT", "DRIVE", "DROIT", "DROLL", "DRONE", 
            "DROOL", "DROOP", "DROPS", "DROPT", "DROSS", "DROVE", "DROWN", 
            "DRUBS", "DRUGS", "DRUID", "DRUMS", "DRUNK", "DRUPE", "DRYAD", 
            "DRYER", "DRYLY", "DUADS", "DUALS", "DUBIO", "DUCAL", "DUCAT", 
            "DUCES", "DUCHY", "DUCKS", "DUCKY", "DUCTS", "DUDDY", "DUDES", 
            "DUELS", "DUETS", "DUFFS", "DUFFY", "DUKES", "DULLS", "DULLY", 
            "DULSE", "DUMBS", "DUMMY", "DUMPS", "DUMPY", "DUNCE", "DUNES", 
            "DUNGS", "DUNGY", "DUNKS", "DUPED", "DUPER", "DUPES", "DUPLE", 
            "DURAL", "DURNS", "DURRA", "DURRS", "DURST", "DURUM", "DUSKS", 
            "DUSKY", "DUSTS", "DUSTY", "DUTCH", "DWARF", "DWELL", "DWELT", 
            "DYADS", "DYERS", "DYING", "DYKES", "DYNES", "EAGER", "EAGLE", 
            "EARED", "EARLS", "EARLY", "EARNS", "EARTH", "EASED", "EASEL", 
            "EASER", "EASES", "EASTS", "EATEN", "EATER", "EAVED", "EAVES", 
            "EBBED", "EBONS", "EBONY", "ECLAT", "ECOLE", "ECRUS", "EDEMA", 
            "EDGAR", "EDGED", "EDGER", "EDGES", "EDICT", "EDIFY", "EDITH", 
            "EDITS", "EDUCE", "EDUCT", "EERIE", "EGADS", "EGGED", "EGGER", 
            "EGRET", "EGYPT", "EIDER", "EIDOS", "EIGHT", "EIKON", "EJECT", 
            "EKING", "ELAND", "ELANS", "ELATE", "ELBOW", "ELDER", "ELECT", 
            "ELEGY", "ELFIN", "ELIDE", "ELITE", "ELLEN", "ELOPE", "ELSES", 
            "ELUDE", "ELVER", "ELVES", "ELVIS", "EMBAR", "EMBAY", "EMBED", 
            "EMBER", "EMBOW", "EMCEE", "EMEER", "EMEND", "EMERY", "EMILY", 
            "EMIRS", "EMITS", "EMMET", "EMOTE", "EMPTY", "ENACT", "ENATE", 
            "ENDED", "ENDER", "ENDOW", "ENDUE", "ENEMA", "ENEMY", "ENFIN", 
            "ENJOY", "ENNUI", "ENROL", "ENSKY", "ENSUE", "ENTER", "ENTRE", 
            "ENTRY", "ENURE", "ENVOI", "ENVOY", "EPEES", "EPICS", "EPOCH", 
            "EPODE", "EPOXY", "EPSOM", "EQUAL", "EQUIP", "ERASE", "ERECT", 
            "ERGOT", "ERICA", "ERODE", "EROSE", "ERRED", "ERROR", "ERUCT", 
            "ERUPT", "ESSAY", "ESSES", "ESTER", "ESTOP", "ETAPE", "ETHEL", 
            "ETHER", "ETHIC", "ETHOS", "ETHYL", "ETNAS", "ETUDE", "EULER", 
            "EVADE", "EVANS", "EVENS", "EVENT", "EVERT", "EVERY", "EVICT", 
            "EVILS", "EVOKE", "EWERS", "EWING", "EXACT", "EXALT", "EXAMS", 
            "EXCEL", "EXECS", "EXERT", "EXILE", "EXIST", "EXITS", "EXPEL", 
            "EXPOS", "EXPWY", "EXTOL", "EXTRA", "EXUDE", "EXULT", "EXURB", 
            "EXXON", "EYERS", "EYING", "EYRIE", "EYRIR", "FABLE", "FACED", 
            "FACER", "FACES", "FACET", "FACIA", "FACIE", "FACTO", "FACTS", 
            "FADDY", "FADED", "FADER", "FADES", "FAERY", "FAGOT", "FAILS", 
            "FAINT", "FAIRE", "FAIRS", "FAIRY", "FAITH", "FAITS", "FAKED", 
            "FAKER", "FAKES", "FAKIR", "FALLS", "FALME", "FAMED", "FAMES", 
            "FANCY", "FANES", "FANGS", "FANNY", "FAQIR", "FARAD", "FARCE", 
            "FARCY", "FARDS", "FARED", "FARER", "FARES", "FARMS", "FAROS", 
            "FARTS", "FASTS", "FATAL", "FATED", "FATES", "FATLY", "FATSO", 
            "FATTY", "FAUGH", "FAULT", "FAUNA", "FAUNS", "FAUST", "FAUVE", 
            "FAVOR", "FAWNS", "FAWNY", "FAXED", "FAXES", "FAZED", "FAZES", 
            "FEARS", "FEASE", "FEAST", "FEATS", "FEAZE", "FECAL", "FECES", 
            "FEEDS", "FEELS", "FEIGN", "FEINT", "FEIST", "FELIX", "FELLA", 
            "FELLS", "FELLY", "FELON", "FELTS", "FEMME", "FEMUR", "FENCE", 
            "FENDS", "FENNY", "FEOFF", "FERAL", "FERMI", "FERNS", "FERNY", 
            "FERRY", "FESSE", "FETAL", "FETAS", "FETCH", "FETED", "FETES", 
            "FETID", "FETOR", "FETUS", "FEUDS", "FEVER", "FEWER", "FEYER", 
            "FEZES", "FIATS", "FIBER", "FIBRE", "FICHE", "FICHU", "FIDEL", 
            "FIDES", "FIDOS", "FIEFS", "FIELD", "FIEND", "FIERY", "FIFED", 
            "FIFER", "FIFES", "FIFTH", "FIFTY", "FIGHT", "FILAR", "FILCH", 
            "FILED", "FILER", "FILES", "FILET", "FILII", "FILLE", "FILLS", 
            "FILLY", "FILMS", "FILMY", "FILTH", "FINAL", "FINCH", "FINDS", 
            "FINED", "FINER", "FINES", "FINIS", "FINKS", "FINNS", "FINNY", 
            "FIORD", "FIRED", "FIRER", "FIRES", "FIRMA", "FIRMS", "FIRRY", 
            "FIRST", "FIRTH", "FISHY", "FISTS", "FITLY", "FIVER", "FIVES", 
            "FIXED", "FIXER", "FIXES", "FIXUP", "FIZZY", "FJORD", "FLABS", 
            "FLACK", "FLAGS", "FLAIL", "FLAIR", "FLAKE", "FLAKY", "FLAME", 
            "FLAMS", "FLAMY", "FLANK", "FLANS", "FLAPS", "FLARE", "FLASH", 
            "FLASK", "FLATS", "FLAWS", "FLAWY", "FLAXY", "FLAYS", "FLEAS", 
            "FLECK", "FLEER", "FLEES", "FLEET", "FLESH", "FLEWS", "FLICK", 
            "FLIED", "FLIER", "FLIES", "FLING", "FLINT", "FLIPS", "FLIRT", 
            "FLITE", "FLITS", "FLOAT", "FLOCK", "FLOES", "FLOGS", "FLOOD", 
            "FLOOR", "FLOPS", "FLORA", "FLOSS", "FLOUR", "FLOUT", "FLOWN", 
            "FLOWS", "FLUBS", "FLUED", "FLUES", "FLUFF", "FLUID", "FLUKE", 
            "FLUKY", "FLUME", "FLUMP", "FLUNG", "FLUNK", "FLUOR", "FLUSH", 
            "FLUTE", "FLUTY", "FLYBY", "FLYER", "FOALS", "FOAMS", "FOAMY", 
            "FOCAL", "FOCUS", "FOEHN", "FOETI", "FOGEY", "FOGGY", "FOGIE", 
            "FOILS", "FOINS", "FOIST", "FOLDS", "FOLIA", "FOLIC", "FOLIO", 
            "FOLKS", "FOLLY", "FONDS", "FONDU", "FONTS", "FOODS", "FOOLS", 
            "FOOTS", "FOOTY", "FORAY", "FORCE", "FORDS", "FORES", "FORGE", 
            "FORGO", "FORKS", "FORKY", "FORMA", "FORMS", "FORTE", "FORTH", 
            "FORTS", "FORTY", "FORUM", "FOSSA", "FOSSE", "FOULS", "FOUND", 
            "FOUNT", "FOURS", "FOVEA", "FOWLS", "FOXED", "FOXES", "FOYER", 
            "FRAGS", "FRAIL", "FRAME", "FRANC", "FRANK", "FRANZ", "FRAPS", 
            "FRATS", "FRAUD", "FRAUS", "FRAYS", "FREAK", "FREED", "FREER", 
            "FREES", "FREON", "FRERE", "FRESH", "FRETS", "FREUD", "FRIAR", 
            "FRIED", "FRIER", "FRIES", "FRIGS", "FRILL", "FRISK", "FRIZZ", 
            "FROCK", "FROES", "FROGS", "FROND", "FRONT", "FROSH", "FROST", 
            "FROTH", "FROWN", "FROZE", "FRUGS", "FRUIT", "FRUMP", "FRYER", 
            "FUCKS", "FUDGE", "FUELS", "FUGAL", "FUGGY", "FUGIT", "FUGUE", 
            "FUJIS", "FULLS", "FULLY", "FUMED", "FUMER", "FUMES", "FUMET", 
            "FUNDI", "FUNDS", "FUNGI", "FUNKS", "FUNKY", "FUNNY", "FURLS", 
            "FUROR", "FURRY", "FURZE", "FURZY", "FUSED", "FUSEE", "FUSEL", 
            "FUSES", "FUSIL", "FUSSY", "FUSTY", "FUZED", "FUZEE", "FUZES", 
            "FUZIL", "FUZZY", "GABBY", "GABLE", "GABON", "GAELS", "GAFFE", 
            "GAFFS", "GAGED", "GAGER", "GAGES", "GAILY", "GAINS", "GAITS", 
            "GALAS", "GALAX", "GALES", "GALLS", "GALLY", "GALOP", "GAMBA", 
            "GAMED", "GAMER", "GAMES", "GAMEY", "GAMIC", "GAMIN", "GAMMA", 
            "GAMUT", "GANEF", "GANEV", "GANGS", "GANJA", "GAOLS", "GAPED", 
            "GAPER", "GAPES", "GAPPY", "GARBO", "GARBS", "GARDE", "GARTH", 
            "GASES", "GASPS", "GASSY", "GATED", "GATES", "GATOR", "GAUDS", 
            "GAUDY", "GAUGE", "GAULS", "GAUNT", "GAUSS", "GAUZE", "GAUZY", 
            "GAVEL", "GAVOT", "GAWKS", "GAWKY", "GAYER", "GAYLY", "GAZED", 
            "GAZER", "GAZES", "GEARS", "GECKO", "GECKS", "GEEKS", "GEESE", 
            "GELDS", "GELEE", "GELID", "GELTS", "GEMMY", "GENAL", "GENES", 
            "GENET", "GENIC", "GENIE", "GENII", "GENOA", "GENRE", "GENTS", 
            "GENUS", "GEODE", "GEOID", "GERMS", "GERMY", "GESSO", "GESTE", 
            "GESTS", "GETUP", "GEUMS", "GHANA", "GHAST", "GHATS", "GHEES", 
            "GHOST", "GHOUL", "GIANT", "GIBED", "GIBER", "GIBES", "GIDDY", 
            "GIFTS", "GIGAS", "GIGUE", "GILDS", "GILLS", "GILLY", "GILTS", 
            "GIMEL", "GIMPS", "GIMPY", "GINKS", "GINNY", "GIPSY", "GIRDS", 
            "GIRLS", "GIRLY", "GIRNS", "GIROS", "GIRTH", "GIRTS", "GISMO", 
            "GISTS", "GIVEN", "GIVER", "GIVES", "GIVIN", "GIZMO", "GLACE", 
            "GLADE", "GLADS", "GLADY", "GLAND", "GLANS", "GLARE", "GLARY", 
            "GLASS", "GLAZE", "GLAZY", "GLEAM", "GLEAN", "GLEBA", "GLEBE", 
            "GLEES", "GLENS", "GLIDE", "GLIMS", "GLINT", "GLOAM", "GLOAT", 
            "GLOBE", "GLOBS", "GLOGG", "GLOMS", "GLOOM", "GLOPS", "GLORY", 
            "GLOSS", "GLOVE", "GLOWS", "GLOZE", "GLUED", "GLUER", "GLUES", 
            "GLUEY", "GLUTS", "GLYPH", "GNARL", "GNARS", "GNASH", "GNATS", 
            "GNAWN", "GNAWS", "GNOME", "GOADS", "GOALS", "GOATS", "GOBOS", 
            "GODLY", "GOERS", "GOFER", "GOGOS", "GOING", "GOLDS", "GOLEM", 
            "GOLFS", "GOLLY", "GOMBO", "GONAD", "GONER", "GONGS", "GONIF", 
            "GONOF", "GOODS", "GOODY", "GOOEY", "GOOFS", "GOOFY", "GOOKS", 
            "GOOKY", "GOONS", "GOONY", "GOOPS", "GOOSE", "GOOSY", "GORED", 
            "GORES", "GORGE", "GORKI", "GORSE", "GORSY", "GOTHS", "GOUDA", 
            "GOUGE", "GOURD", "GOUTS", "GOUTY", "GOWNS", "GOYIM", "GRAAL", 
            "GRABS", "GRACE", "GRADE", "GRADS", "GRAFT", "GRAIL", "GRAIN", 
            "GRAMP", "GRAMS", "GRANA", "GRAND", "GRANT", "GRAPE", "GRAPH", 
            "GRAPY", "GRASP", "GRASS", "GRATA", "GRATE", "GRAVE", "GRAVY", 
            "GRAYS", "GRAZE", "GREAT", "GREBE", "GRECO", "GREED", "GREEK", 
            "GREEN", "GREET", "GRETA", "GREYS", "GRIDS", "GRIEF", "GRIFT", 
            "GRIGS", "GRILL", "GRIME", "GRIMM", "GRIMY", "GRIND", "GRINS", 
            "GRIOT", "GRIPE", "GRIPS", "GRIPT", "GRIPY", "GRIST", "GRITS", 
            "GROAN", "GROAT", "GROGS", "GROIN", "GROOM", "GROPE", "GROSS", 
            "GROSZ", "GROTS", "GROUP", "GROUT", "GROVE", "GROWL", "GROWN", 
            "GROWS", "GRUBS", "GRUEL", "GRUFF", "GRUMP", "GRUNT", "GUACO", 
            "GUANO", "GUARD", "GUARS", "GUAVA", "GUCKS", "GUESS", "GUEST", 
            "GUFFS", "GUIDE", "GUILD", "GUILE", "GUILT", "GUIRO", "GUISE", 
            "GULCH", "GULFS", "GULFY", "GULLS", "GULLY", "GULPS", "GULPY", 
            "GUMBO", "GUMMY", "GUNKS", "GUNNY", "GUPPY", "GURUS", "GUSHY", 
            "GUSSY", "GUSTO", "GUSTS", "GUSTY", "GUTSY", "GUTTA", "GUTTY", 
            "GUYED", "GYPSY", "GYRAL", "GYRED", "GYRES", "GYROS", "GYRUS", 
            "GYVED", "GYVES", "HABIT", "HACKS", "HADES", "HADJI", "HADST", 
            "HAETS", "HAFTS", "HAGUE", "HAIKU", "HAILS", "HAIRS", "HAIRY", 
            "HAITI", "HAJIS", "HAJJI", "HAKES", "HALED", "HALER", "HALES", 
            "HALLO", "HALLS", "HALOS", "HALTS", "HALVA", "HALVE", "HAMMY", 
            "HANCE", "HANDS", "HANDY", "HANGS", "HANKS", "HANKY", "HANOI", 
            "HANTS", "HAOLE", "HAPLY", "HAPPY", "HARDS", "HARDY", "HARED", 
            "HAREM", "HARES", "HARKS", "HARMS", "HARPS", "HARPY", "HARRY", 
            "HARSH", "HARTS", "HASID", "HASPS", "HASTA", "HASTE", "HASTY", 
            "HATCH", "HATED", "HATER", "HATES", "HAUGH", "HAULS", "HAUNT", 
            "HAUTE", "HAVEN", "HAVER", "HAVES", "HAVOC", "HAWED", "HAWKS", 
            "HAWSE", "HAYDN", "HAYED", "HAYER", "HAYES", "HAZED", "HAZEL", 
            "HAZER", "HAZES", "HDQRS", "HEADS", "HEADY", "HEALS", "HEAPS", 
            "HEARD", "HEARS", "HEART", "HEATH", "HEATS", "HEAVE", "HEAVY", 
            "HECKS", "HEDGE", "HEDGY", "HEEDS", "HEELS", "HEFTS", "HEFTY", 
            "HEIGH", "HEILS", "HEIRS", "HEIST", "HELEN", "HELIO", "HELIX", 
            "HELLO", "HELLS", "HELMS", "HELOT", "HELPS", "HELVE", "HEMAN", 
            "HEMPS", "HEMPY", "HENCE", "HENNA", "HENRY", "HENTS", "HERBS", 
            "HERBY", "HERDS", "HERES", "HERON", "HEROS", "HERTZ", "HEWED", 
            "HEWER", "HEXAD", "HEXED", "HEXER", "HEXES", "HEXYL", "HICKS", 
            "HIDED", "HIDER", "HIDES", "HIGHS", "HIGHT", "HIKED", "HIKER", 
            "HIKES", "HILLS", "HILLY", "HILTS", "HINDI", "HINDS", "HINDU", 
            "HINGE", "HINNY", "HINTS", "HIPPO", "HIPPY", "HIRED", "HIRER", 
            "HIRES", "HISTS", "HITCH", "HIVED", "HIVES", "HOAGY", "HOARD", 
            "HOARY", "HOBBY", "HOBOS", "HOCKS", "HOCUS", "HODAD", "HOERS", 
            "HOGAN", "HOGGS", "HOISE", "HOIST", "HOKEY", "HOKUM", "HOLDS", 
            "HOLED", "HOLER", "HOLES", "HOLEY", "HOLLO", "HOLLY", "HOLTS", 
            "HOMED", "HOMER", "HOMES", "HOMEY", "HOMOS", "HONAN", "HONDA", 
            "HONED", "HONER", "HONES", "HONEY", "HONKS", "HONKY", "HONOR", 
            "HOOCH", "HOODS", "HOOEY", "HOOFS", "HOOKA", "HOOKS", "HOOKY", 
            "HOOPS", "HOOTS", "HOPED", "HOPER", "HOPES", "HOPIS", "HORAH", 
            "HORAL", "HORAS", "HORDE", "HORNS", "HORNY", "HORSE", "HORST", 
            "HORSY", "HOSED", "HOSES", "HOSTS", "HOTEL", "HOTLY", "HOUND", 
            "HOURI", "HOURS", "HOUSE", "HOVEL", "HOVER", "HOWDY", "HOWES", 
            "HOWLS", "HOYLE", "HUBBY", "HUCKS", "HUFFS", "HUFFY", "HUGER", 
            "HULAS", "HULKS", "HULKY", "HULLO", "HULLS", "HUMAN", "HUMID", 
            "HUMOR", "HUMPH", "HUMPS", "HUMPY", "HUMUS", "HUNCH", "HUNKS", 
            "HUNKY", "HUNTS", "HURLS", "HURLY", "HURON", "HURRY", "HURTS", 
            "HUSKS", "HUSKY", "HUSSY", "HUTCH", "HUZZA", "HYDRA", "HYDRO", 
            "HYENA", "HYING", "HYMEN", "HYMNS", "HYPED", "HYPER", "HYPES", 
            "HYPOS", "HYRAX", "HYSON", "IAMBI", "IAMBS", "ICHOR", "ICIER", 
            "ICILY", "ICING", "ICKER", "ICONS", "ICTUS", "IDAHO", "IDEAL", 
            "IDEAS", "IDIOM", "IDIOT", "IDLED", "IDLER", "IDLES", "IDOLS", 
            "IDYLL", "IDYLS", "IGLOO", "IGNIS", "IKONS", "ILEAL", "ILEUM", 
            "ILIAD", "ILIUM", "ILLER", "IMAGE", "IMAGO", "IMAMS", "IMBED", 
            "IMBUE", "IMMIX", "IMPED", "IMPEL", "IMPER", "IMPLY", "INANE", 
            "INAPT", "INARM", "INCAS", "INCOG", "INCUR", "INCUS", "INDEX", 
            "INDIA", "INDOL", "INDOW", "INDUE", "INEPT", "INERT", "INFER", 
            "INFIX", "INFOS", "INFRA", "INGLE", "INGOT", "INKED", "INKER", 
            "INKLE", "INLAY", "INLET", "INNED", "INNER", "INPUT", "INSET", 
            "INSTR", "INTEL", "INTER", "INTRA", "INTRO", "INURE", "INURN", 
            "IODIN", "IONIC", "IOTAS", "IOWAN", "IRAQI", "IRATE", "IRENE", 
            "IRING", "IRISH", "IRKED", "IRONS", "IRONY", "ISAAC", "ISLAM", 
            "ISLED", "ISLES", "ISLET", "ISSEI", "ISSUE", "ISTLE", "ITALY", 
            "ITCHY", "ITEMS", "IVIED", "IVIES", "IVORY", "IXIAS", "JABOT", 
            "JACAL", "JACKS", "JACKY", "JACOB", "JADED", "JADES", "JAGGS", 
            "JAGGY", "JAILS", "JAKES", "JALAP", "JAMBS", "JAMES", "JANES", 
            "JANET", "JANUS", "JAPAN", "JAPED", "JAPER", "JAPES", "JASON", 
            "JATOS", "JAUNT", "JAVAS", "JAWED", "JAZZY", "JEANS", "JEEPS", 
            "JEERS", "JEFES", "JEHAD", "JEHUS", "JELLS", "JELLY", "JEMMY", 
            "JENNY", "JERKS", "JERKY", "JERRY", "JESSE", "JESTS", "JESUS", 
            "JETTY", "JEWED", "JEWEL", "JEWRY", "JIBED", "JIBER", "JIBES", 
            "JIFFS", "JIFFY", "JIHAD", "JILLS", "JILTS", "JIMMY", "JINGO", 
            "JINNI", "JINNS", "JIVED", "JIVES", "JNANA", "JOCKO", "JOCKS", 
            "JOEYS", "JOHNS", "JOINS", "JOINT", "JOIST", "JOKED", "JOKER", 
            "JOKES", "JOLLY", "JOLTS", "JOLTY", "JONAH", "JONES", "JORAM", 
            "JORUM", "JOTTY", "JOULE", "JOUST", "JOWLS", "JOWLY", "JOYCE", 
            "JOYED", "JUANS", "JUDAS", "JUDGE", "JUDOS", "JUICE", "JUICY", 
            "JUJUS", "JUKED", "JUKES", "JULEP", "JUMBO", "JUMPS", "JUMPY", 
            "JUNCO", "JUNKS", "JUNKY", "JUNTA", "JUNTO", "JUROR", "JUSTS", 
            "JUTES", "JUTTY", "JUXTA", "KABOB", "KADIS", "KAFIR", "KAFKA", 
            "KAIAK", "KAKAS", "KAKIS", "KALES", "KALIF", "KALPA", "KAMES", 
            "KANAS", "KANJI", "KAONS", "KAPOK", "KAPPA", "KAPUT", "KARAT", 
            "KAREN", "KARMA", "KARST", "KARTS", "KASHA", "KATHY", "KAYAK", 
            "KAYOS", "KAZOO", "KEATS", "KEBAB", "KEBOB", "KEDGE", "KEEFS", 
            "KEELS", "KEENS", "KEEPS", "KEFIR", "KELPS", "KELPY", "KELTS", 
            "KEMPT", "KENDO", "KENNY", "KENOS", "KENYA", "KEPIS", "KERBS", 
            "KERFS", "KERNS", "KERRY", "KETCH", "KEYED", "KHAKI", "KHANS", 
            "KHATS", "KICKS", "KICKY", "KIDDO", "KIDDY", "KIEFS", "KIKES", 
            "KILLS", "KILNS", "KILOS", "KILTS", "KILTY", "KINDS", "KINES", 
            "KINGS", "KINKS", "KINKY", "KIOSK", "KIOWA", "KIRKS", "KITED", 
            "KITER", "KITES", "KITHS", "KITTY", "KIVAS", "KIWIS", "KLANS", 
            "KLEIG", "KLIEG", "KLUTZ", "KNACK", "KNAPS", "KNAVE", "KNEAD", 
            "KNEED", "KNEEL", "KNEES", "KNELL", "KNELT", "KNIFE", "KNISH", 
            "KNITS", "KNOBS", "KNOCK", "KNOLL", "KNOTS", "KNOUT", "KNOWN", 
            "KNOWS", "KNURL", "KOALA", "KOANS", "KODAK", "KOHLS", "KOLAS", 
            "KOOKS", "KOOKY", "KOPEK", "KOPHS", "KOPJE", "KORAN", "KOREA", 
            "KOTOS", "KRAAL", "KRAFT", "KRAIT", "KRAUT", "KREBS", "KRILL", 
            "KRONA", "KRONE", "KUDOS", "KUDUS", "KUDZU", "KULAK", "KYATS", 
            "KYOTO", "KYRIE", "LABEL", "LABIA", "LABOR", "LACED", "LACER", 
            "LACES", "LACEY", "LACKS", "LADED", "LADEN", "LADER", "LADES", 
            "LADLE", "LAGER", "LAIRD", "LAIRS", "LAITY", "LAKED", "LAKER", 
            "LAKES", "LAMAS", "LAMBS", "LAMED", "LAMER", "LAMES", "LAMIA", 
            "LAMPS", "LANAI", "LANCE", "LANDS", "LANES", "LANKY", "LAPEL", 
            "LAPIN", "LAPIS", "LAPPS", "LAPSE", "LARCH", "LARDS", "LARDY", 
            "LARES", "LARGE", "LARGO", "LARKS", "LARKY", "LARRY", "LARVA", 
            "LASED", "LASER", "LASES", "LASSO", "LASTS", "LATCH", "LATED", 
            "LATEN", "LATER", "LATEX", "LATHE", "LATHS", "LATHY", "LATIN", 
            "LAUDE", "LAUDS", "LAUGH", "LAURA", "LAVAS", "LAVED", "LAVER", 
            "LAVES", "LAWED", "LAWNS", "LAWNY", "LAXER", "LAXLY", "LAYED", 
            "LAYER", "LAZAR", "LAZED", "LAZES", "LEACH", "LEADS", "LEADY", 
            "LEAFS", "LEAFY", "LEAKS", "LEAKY", "LEANS", "LEANT", "LEAPS", 
            "LEAPT", "LEARN", "LEARY", "LEASE", "LEASH", "LEAST", "LEAVE", 
            "LEDGE", "LEDGY", "LEECH", "LEEDS", "LEEKS", "LEERS", "LEERY", 
            "LEFTS", "LEFTY", "LEGAL", "LEGER", "LEGGY", "LEGIT", "LEMAN", 
            "LEMMA", "LEMON", "LEMUR", "LENDS", "LENIN", "LENSE", "LENTO", 
            "LEONE", "LEPER", "LETCH", "LETHE", "LETUP", "LEVEE", "LEVEL", 
            "LEVER", "LEVIN", "LEVIS", "LEWIS", "LIANA", "LIARS", "LIBEL", 
            "LIBER", "LIBRA", "LIBRE", "LIBYA", "LICHI", "LICHT", "LICIT", 
            "LICKS", "LIDAR", "LIDOS", "LIEGE", "LIENS", "LIERS", "LIEUT", 
            "LIFER", "LIFTS", "LIGER", "LIGHT", "LIKED", "LIKEN", "LIKER", 
            "LIKES", "LILAC", "LILLY", "LILTS", "LIMAS", "LIMBO", "LIMBS", 
            "LIMBY", "LIMED", "LIMES", "LIMEY", "LIMIT", "LIMNS", "LIMOS", 
            "LIMPS", "LINAC", "LINDA", "LINDY", "LINED", "LINEN", "LINER", 
            "LINES", "LINEY", "LINGO", "LINGS", "LINKS", "LINKY", "LINOS", 
            "LINTS", "LINTY", "LINUM", "LIONS", "LIPID", "LIPPY", "LIRAS", 
            "LISLE", "LISPS", "LISTS", "LISZT", "LITER", "LITES", "LITHE", 
            "LITHO", "LITRE", "LIVED", "LIVEN", "LIVER", "LIVES", "LIVID", 
            "LIVRE", "LLAMA", "LLANO", "LOACH", "LOADS", "LOAFS", "LOAMS", 
            "LOAMY", "LOANS", "LOATH", "LOBAR", "LOBBY", "LOBED", "LOBES", 
            "LOBOS", "LOCAL", "LOCHS", "LOCKS", "LOCOS", "LOCUS", "LODEN", 
            "LODES", "LODGE", "LOESS", "LOFTS", "LOFTY", "LOGAN", "LOGES", 
            "LOGGY", "LOGIA", "LOGIC", "LOGOS", "LOINS", "LOLLS", "LOLLY", 
            "LONER", "LONGS", "LOOFA", "LOOFS", "LOOKS", "LOOMS", "LOONS", 
            "LOONY", "LOOPS", "LOOPY", "LOOSE", "LOOTS", "LOPED", "LOPER", 
            "LOPES", "LOPPY", "LORAN", "LORDS", "LORES", "LORIS", "LORRY", 
            "LOSER", "LOSES", "LOSSY", "LOTOS", "LOTTO", "LOTUS", "LOUGH", 
            "LOUIE", "LOUIS", "LOUPE", "LOUPS", "LOURS", "LOURY", "LOUSE", 
            "LOUSY", "LOUTS", "LOVED", "LOVER", "LOVES", "LOWED", "LOWER", 
            "LOWLY", "LOXES", "LOYAL", "LUAUS", "LUBES", "LUCES", "LUCIA", 
            "LUCID", "LUCKS", "LUCKY", "LUCRE", "LUFFS", "LUGES", "LULLS", 
            "LULUS", "LUMEN", "LUMPS", "LUMPY", "LUNAR", "LUNAS", "LUNCH", 
            "LUNES", "LUNET", "LUNGE", "LUNGS", "LUNKS", "LUPIN", "LUPUS", 
            "LURCH", "LURED", "LURER", "LURES", "LURID", "LURKS", "LUSTS", 
            "LUSTY", "LUTED", "LUTES", "LUXES", "LYCEE", "LYING", "LYMPH", 
            "LYNCH", "LYRES", "LYRIC", "LYSED", "LYSES", "LYSIN", "MACAW", 
            "MACED", "MACER", "MACES", "MACHO", "MACHS", "MACKS", "MACLE", 
            "MACRO", "MADAM", "MADLY", "MADRE", "MAFIA", "MAGES", "MAGIC", 
            "MAGMA", "MAGUS", "MAIDS", "MAILS", "MAIMS", "MAINE", "MAINS", 
            "MAIST", "MAIZE", "MAJOR", "MAKER", "MAKES", "MALAY", "MALES", 
            "MALLS", "MALTA", "MALTS", "MALTY", "MAMAS", "MAMBA", "MAMBO", 
            "MAMIE", "MAMMA", "MAMMY", "MANAS", "MANED", "MANES", "MANGE", 
            "MANGO", "MANGY", "MANIA", "MANIC", "MANLY", "MANNA", "MANOR", 
            "MANOS", "MANSE", "MANTA", "MANUS", "MAORI", "MAPLE", "MAQUI", 
            "MARCH", "MARCS", "MARDI", "MARES", "MARGE", "MARIA", "MARIE", 
            "MARKS", "MARRY", "MARSE", "MARSH", "MARTS", "MASER", "MASHY", 
            "MASKS", "MASON", "MASSA", "MASSE", "MASSY", "MASTS", "MATCH", 
            "MATED", "MATER", "MATES", "MATEY", "MATHS", "MATIN", "MATTE", 
            "MATTS", "MATZO", "MAULS", "MAUVE", "MAVEN", "MAVIN", "MAXIM", 
            "MAXIS", "MAYAN", "MAYAS", "MAYBE", "MAYOR", "MAYST", "MAZED", 
            "MAZEL", "MAZER", "MAZES", "MEADS", "MEALS", "MEALY", "MEANS", 
            "MEANT", "MEANY", "MEATS", "MEATY", "MECCA", "MECUM", "MEDAL", 
            "MEDIA", "MEDIC", "MEEDS", "MEETS", "MELBA", "MELDS", "MELEE", 
            "MELON", "MELTS", "MEMOS", "MENDS", "MENUS", "MEOWS", "MERCI", 
            "MERCY", "MERER", "MERES", "MERGE", "MERIT", "MERRY", "MESAS", 
            "MESHY", "MESON", "MESSY", "METAL", "METED", "METER", "METES", 
            "METRE", "METRO", "MEWED", "MEWLS", "MEZZO", "MIAMI", "MIAOU", 
            "MIAOW", "MIASM", "MIAUL", "MICAS", "MICKS", "MICRO", "MIDAS", 
            "MIDDY", "MIDGE", "MIDIS", "MIDST", "MIENS", "MIFFS", "MIFFY", 
            "MIGGS", "MIGHT", "MIKES", "MILAN", "MILCH", "MILER", "MILES", 
            "MILKS", "MILKY", "MILLE", "MILLS", "MILOS", "MIMED", "MIMEO", 
            "MIMER", "MIMES", "MIMIC", "MINCE", "MINCY", "MINDS", "MINED", 
            "MINER", "MINES", "MINGY", "MINIM", "MINIS", "MINKS", "MINNY", 
            "MINOR", "MINTS", "MINTY", "MINUS", "MIRED", "MIRES", "MIRKS", 
            "MIRKY", "MIRTH", "MIRVS", "MISDO", "MISER", "MISOS", "MISSY", 
            "MISTS", "MISTY", "MITER", "MITES", "MITRE", "MITTS", "MIXED", 
            "MIXER", "MIXES", "MIXUP", "MOANS", "MOATS", "MOBIL", "MOCHA", 
            "MOCKS", "MODAL", "MODEL", "MODEM", "MODES", "MODUS", "MOGUL", 
            "MOILS", "MOIRE", "MOIST", "MOLAR", "MOLDS", "MOLDY", "MOLES", 
            "MOLLS", "MOLLY", "MOLTO", "MOLTS", "MOMMA", "MOMMY", "MONAD", 
            "MONDE", "MONDO", "MONEY", "MONKS", "MONOS", "MONTE", "MONTH", 
            "MOOCH", "MOODS", "MOODY", "MOOED", "MOOLA", "MOONS", "MOONY", 
            "MOORE", "MOORS", "MOORY", "MOOSE", "MOOTS", "MOPED", "MOPER", 
            "MOPES", "MOPEY", "MORAL", "MORAY", "MOREL", "MORES", "MORNS", 
            "MORON", "MORPH", "MORSE", "MOSES", "MOSEY", "MOSKS", "MOSSY", 
            "MOSTS", "MOTEL", "MOTES", "MOTET", "MOTEY", "MOTHS", "MOTHY", 
            "MOTIF", "MOTOR", "MOTTO", "MOUES", "MOULD", "MOULT", "MOUND", 
            "MOUNT", "MOURN", "MOUSE", "MOUSY", "MOUTH", "MOVED", "MOVER", 
            "MOVES", "MOVIE", "MOWED", "MOWER", "MOXAS", "MOXIE", "MUCKS", 
            "MUCKY", "MUCUS", "MUDDY", "MUDRA", "MUFFS", "MUFTI", "MUGGS", 
            "MUGGY", "MUJIK", "MULCH", "MULCT", "MULED", "MULES", "MULEY", 
            "MULLA", "MULLS", "MULTI", "MULTO", "MUMBO", "MUMMS", "MUMMY", 
            "MUMPS", "MUNCH", "MUONS", "MURAL", "MUREX", "MURKS", "MURKY", 
            "MUSED", "MUSER", "MUSES", "MUSHY", "MUSIC", "MUSKS", "MUSKY", 
            "MUSSY", "MUSTS", "MUSTY", "MUTED", "MUTER", "MUTES", "MUTTS", 
            "MUZZY", "MYLAR", "MYNAH", "MYNAS", "MYOPE", "MYOPY", "MYRRH", 
            "MYTHS", "NABOB", "NACRE", "NADIR", "NAIAD", "NAIFS", "NAILS", 
            "NAIVE", "NAKED", "NAMED", "NAMER", "NAMES", "NANCE", "NANCY", 
            "NANNY", "NAPES", "NAPPY", "NARCO", "NARCS", "NARES", "NARIS", 
            "NARKS", "NASAL", "NASTY", "NATAL", "NATES", "NATTY", "NAVAL", 
            "NAVEL", "NAVES", "NAVVY", "NAZIS", "NEAPS", "NEARS", "NEATH", 
            "NEATS", "NECKS", "NEEDS", "NEEDY", "NEGRO", "NEGUS", "NEHRU", 
            "NEIGH", "NEONS", "NEPAL", "NERDS", "NERTS", "NERTZ", "NERVE", 
            "NERVY", "NESTS", "NETTY", "NEVER", "NEVUS", "NEWEL", "NEWER", 
            "NEWLY", "NEWSY", "NEWTS", "NEXUS", "NICER", "NICHE", "NICKS", 
            "NIECE", "NIFTY", "NIGHS", "NIGHT", "NIHIL", "NILLS", "NIMBI", 
            "NINES", "NINNY", "NINON", "NINTH", "NIPPY", "NISEI", "NITER", 
            "NITRE", "NITRO", "NITTY", "NIXED", "NIXES", "NIXIE", "NIXON", 
            "NOBBY", "NOBEL", "NOBLE", "NOBLY", "NOCKS", "NODAL", "NODDY", 
            "NODES", "NODUS", "NOELS", "NOGGS", "NOHOW", "NOIRE", "NOISE", 
            "NOISY", "NOLLE", "NOMAD", "NONCE", "NONES", "NOOKS", "NOOKY", 
            "NOONS", "NOOSE", "NORMA", "NORMS", "NORSE", "NORTH", "NOSED", 
            "NOSES", "NOSEY", "NOTAL", "NOTCH", "NOTED", "NOTER", "NOTES", 
            "NOTRE", "NOUNS", "NOVAE", "NOVAS", "NOVEL", "NOWAY", "NUBBY", 
            "NUBIA", "NUDER", "NUDES", "NUDGE", "NUDIE", "NUDUM", "NUKES", 
            "NULLO", "NULLS", "NUMBS", "NURSE", "NUTTY", "NYLON", "NYMPH", 
            "OAKEN", "OAKUM", "OARED", "OASES", "OASIS", "OASTS", "OATEN", 
            "OATER", "OATHS", "OBEAH", "OBELI", "OBESE", "OBEYS", "OBITS", 
            "OBOES", "OBOLS", "OCCUR", "OCEAN", "OCHER", "OCHRE", "OCTAD", 
            "OCTAL", "OCTET", "OCTYL", "OCULI", "ODDER", "ODDLY", "ODEON", 
            "ODIUM", "ODORS", "ODOUR", "OFAYS", "OFFAL", "OFFED", "OFFER", 
            "OFTEN", "OFTER", "OGEES", "OGHAM", "OGIVE", "OGLED", "OGLER", 
            "OGLES", "OGRES", "OHING", "OHMIC", "OILED", "OILER", "OINKS", 
            "OKAPI", "OKAYS", "OKRAS", "OLDEN", "OLDER", "OLDIE", "OLEOS", 
            "OLIOS", "OLIVE", "OLLAS", "OLOGY", "OMAHA", "OMBRE", "OMEGA", 
            "OMENS", "OMITS", "ONCES", "ONERY", "ONION", "ONSET", "OOHED", 
            "OOMPH", "OOZED", "OOZES", "OPALS", "OPENS", "OPERA", "OPINE", 
            "OPIUM", "OPTED", "OPTIC", "ORALS", "ORANG", "ORATE", "ORBED", 
            "ORBIT", "ORCAS", "ORDER", "ORDOS", "OREAD", "ORGAN", "ORGIC", 
            "ORIEL", "ORION", "ORLON", "ORRIS", "ORTHO", "OSAGE", "OSAKA", 
            "OSCAR", "OSIER", "OSMIC", "OSSEA", "OSSIA", "OSTIA", "OTHER", 
            "OTTER", "OUGHT", "OUIJA", "OUNCE", "OUSEL", "OUSTS", "OUTDO", 
            "OUTED", "OUTER", "OUTGO", "OUTRE", "OUZEL", "OUZOS", "OVALS", 
            "OVARY", "OVATE", "OVENS", "OVERS", "OVERT", "OVINE", "OVOID", 
            "OVOLO", "OVULE", "OWING", "OWLET", "OWNED", "OWNER", "OXBOW", 
            "OXEYE", "OXIDE", "OXLIP", "OXTER", "OYERS", "OZONE", "PACED", 
            "PACER", "PACES", "PACKS", "PACTA", "PACTS", "PADDY", "PADRE", 
            "PADRI", "PAEAN", "PAGAN", "PAGED", "PAGES", "PAILS", "PAINE", 
            "PAINS", "PAINT", "PAIRS", "PALED", "PALER", "PALES", "PALLS", 
            "PALLY", "PALMS", "PALMY", "PALPS", "PALSY", "PAMPA", "PANDA", 
            "PANED", "PANEL", "PANES", "PANGA", "PANGS", "PANIC", "PANSY", 
            "PANTS", "PANTY", "PAPAL", "PAPAS", "PAPAW", "PAPER", "PAPPY", 
            "PAPUA", "PARAS", "PARCH", "PARED", "PARER", "PARES", "PARIS", 
            "PARKA", "PARKS", "PARRY", "PARSE", "PARTE", "PARTI", "PARTS", 
            "PARTY", "PARVE", "PASEO", "PASHA", "PASSE", "PASTA", "PASTE", 
            "PASTS", "PASTY", "PATCH", "PATED", "PATEN", "PATER", "PATES", 
            "PATHS", "PATIO", "PATLY", "PATSY", "PATTY", "PAUSE", "PAVAN", 
            "PAVED", "PAVER", "PAVES", "PAWED", "PAWER", "PAWKY", "PAWLS", 
            "PAWNS", "PAXES", "PAYED", "PAYEE", "PAYER", "PEACE", "PEACH", 
            "PEAKS", "PEAKY", "PEALS", "PEARL", "PEARS", "PEART", "PEASE", 
            "PEATS", "PEATY", "PEAVY", "PECAN", "PECKS", "PECKY", "PEDAL", 
            "PEDES", "PEDRO", "PEEKS", "PEELS", "PEENS", "PEEPS", "PEERS", 
            "PEERY", "PEEVE", "PEGGY", "PEINS", "PEKES", "PEKIN", "PEKOE", 
            "PELFS", "PELTS", "PENAL", "PENCE", "PENDS", "PENES", "PENIS", 
            "PENNA", "PENNY", "PENSE", "PEONS", "PEONY", "PEPPY", "PEPSI", 
            "PERCH", "PERDU", "PERDY", "PERES", "PERIL", "PERIS", "PERKS", 
            "PERKY", "PERMS", "PERRY", "PESKY", "PESOS", "PESTS", "PETAL", 
            "PETER", "PETIT", "PETRI", "PETRO", "PETTY", "PEWEE", "PEWIT", 
            "PHAGE", "PHARM", "PHASE", "PHIAL", "PHLOX", "PHONE", "PHONO", 
            "PHONS", "PHONY", "PHOTO", "PHREN", "PHYLA", "PIANO", "PICAS", 
            "PICKS", "PICKY", "PICOT", "PIECE", "PIERS", "PIETA", "PIETY", 
            "PIGGY", "PIGMY", "PIKED", "PIKER", "PIKES", "PILAF", "PILAR", 
            "PILED", "PILES", "PILLS", "PILOT", "PIMAS", "PIMPS", "PINCH", 
            "PINED", "PINES", "PINEY", "PINGS", "PINKO", "PINKS", "PINKY", 
            "PINNA", "PINON", "PINTA", "PINTO", "PINTS", "PINUP", "PIONS", 
            "PIOUS", "PIPED", "PIPER", "PIPES", "PIPET", "PIPIT", "PIQUE", 
            "PIROG", "PITAS", "PITCH", "PITHS", "PITHY", "PITON", "PIVOT", 
            "PIXEL", "PIXES", "PIXIE", "PIZZA", "PLACE", "PLACK", "PLAID", 
            "PLAIN", "PLAIT", "PLANE", "PLANK", "PLANS", "PLANT", "PLASH", 
            "PLASM", "PLATE", "PLATO", "PLATS", "PLATY", "PLAYA", "PLAYS", 
            "PLAZA", "PLEAD", "PLEAS", "PLEAT", "PLEBE", "PLEBS", "PLENA", 
            "PLIED", "PLIER", "PLIES", "PLINK", "PLODS", "PLONK", "PLOPS", 
            "PLOTS", "PLOWS", "PLOYS", "PLUCK", "PLUGS", "PLUMB", "PLUME", 
            "PLUMP", "PLUMS", "PLUMY", "PLUNK", "PLUSH", "PLUTO", "PLYER", 
            "POACH", "POCKS", "POCKY", "PODGY", "PODIA", "POEMS", "POESY", 
            "POETS", "POILU", "POINT", "POISE", "POKED", "POKER", "POKES", 
            "POKEY", "POLAR", "POLED", "POLER", "POLES", "POLIO", "POLIS", 
            "POLIT", "POLKA", "POLLS", "POLYP", "POLYS", "POMES", "POMPS", 
            "PONCE", "PONDS", "PONES", "POOCH", "POOHS", "POOLS", "POOPS", 
            "POPES", "POPPA", "POPPY", "PORCH", "PORED", "PORES", "PORGY", 
            "PORKS", "PORKY", "PORNO", "PORNS", "PORTS", "POSED", "POSER", 
            "POSES", "POSIT", "POSSE", "POSTS", "POTSY", "POTTY", "POUCH", 
            "POUFF", "POUFS", "POULT", "POUND", "POURS", "POUTS", "POUTY", 
            "POWER", "POXED", "POXES", "PRAMS", "PRANA", "PRANK", "PRAOS", 
            "PRATE", "PRATS", "PRAUS", "PRAWN", "PRAYS", "PREEN", "PREPS", 
            "PRESS", "PREST", "PREXY", "PREYS", "PRICE", "PRICK", "PRICY", 
            "PRIDE", "PRIED", "PRIER", "PRIES", "PRIGS", "PRIMA", "PRIME", 
            "PRIMO", "PRIMP", "PRIMS", "PRINK", "PRINT", "PRIOR", "PRISE", 
            "PRISM", "PRISS", "PRIVY", "PRIZE", "PROAS", "PROBE", "PRODS", 
            "PROEM", "PROFS", "PROGS", "PROLE", "PROMS", "PRONE", "PRONG", 
            "PROOF", "PROPS", "PROSE", "PROSY", "PROUD", "PROVE", "PROWL", 
            "PROWS", "PROXY", "PRUDE", "PRUNE", "PRYER", "PSALM", "PSEUD", 
            "PSHAW", "PSYCH", "PUBES", "PUBIC", "PUBIS", "PUCES", "PUCKS", 
            "PUDGY", "PUFFS", "PUFFY", "PUGGY", "PUKED", "PUKES", "PUKKA", 
            "PULED", "PULER", "PULES", "PULLS", "PULPS", "PULPY", "PULSE", 
            "PUMAS", "PUMPS", "PUNCH", "PUNKS", "PUNKY", "PUNNY", "PUNTS", 
            "PUNTY", "PUPAE", "PUPAL", "PUPAS", "PUPIL", "PUPPY", "PUREE", 
            "PURER", "PURGE", "PURIM", "PURLS", "PURRS", "PURSE", "PURSY", 
            "PUSHY", "PUSSY", "PUTON", "PUTTS", "PUTTY", "PYGMY", "PYLON", 
            "PYRES", "PYREX", "PYRIC", "PYXES", "PYXIE", "PYXIS", "QAIDS", 
            "QATAR", "QIANA", "QOPHS", "QUACK", "QUADS", "QUAFF", "QUAGS", 
            "QUAIL", "QUAIS", "QUAKE", "QUAKY", "QUALE", "QUALM", "QUANT", 
            "QUARK", "QUART", "QUASH", "QUASI", "QUAYS", "QUEAN", "QUEEN", 
            "QUEER", "QUELL", "QUERN", "QUERY", "QUEST", "QUEUE", "QUEYS", 
            "QUICK", "QUIDS", "QUIET", "QUILL", "QUILT", "QUINT", "QUIPS", 
            "QUIPU", "QUIRE", "QUIRK", "QUIRT", "QUITE", "QUITO", "QUITS", 
            "QUODS", "QUOIN", "QUOIT", "QUOTA", "QUOTE", "QUOTH", "QURSH", 
            "RABBI", "RABIC", "RABID", "RACED", "RACER", "RACES", "RACKS", 
            "RADAR", "RADII", "RADIO", "RADIX", "RADON", "RAFTS", "RAGAS", 
            "RAGED", "RAGES", "RAGGY", "RAIDS", "RAILS", "RAINS", "RAINY", 
            "RAISE", "RAJAH", "RAJAS", "RAKED", "RAKER", "RAKES", "RALES", 
            "RALLY", "RALPH", "RAMIE", "RAMPS", "RANCH", "RANDS", "RANDY", 
            "RANEE", "RANGE", "RANGY", "RANIS", "RANKS", "RANTS", "RAPED", 
            "RAPER", "RAPES", "RAPID", "RARER", "RASED", "RASER", "RASES", 
            "RASPS", "RASPY", "RATCH", "RATED", "RATER", "RATES", "RATIO", 
            "RATTY", "RAVED", "RAVEL", "RAVEN", "RAVER", "RAVES", "RAWER", 
            "RAWLY", "RAYED", "RAYON", "RAZED", "RAZEE", "RAZER", "RAZES", 
            "RAZOR", "REACH", "REACT", "READD", "READS", "READY", "REALM", 
            "REALS", "REAMS", "REAPS", "REARM", "REARS", "REAVE", "REBBE", 
            "REBEC", "REBEL", "REBID", "REBOP", "REBUS", "REBUT", "RECAP", 
            "RECKS", "RECON", "RECTA", "RECTI", "RECTO", "RECUR", "RECUT", 
            "REDED", "REDES", "REDID", "REDIP", "REDLY", "REDOS", "REDOX", 
            "REDRY", "REDUX", "REDYE", "REEDS", "REEDY", "REEFS", "REEFY", 
            "REEKS", "REEKY", "REELS", "REEVE", "REFED", "REFER", "REFIT", 
            "REFIX", "REFLY", "REFRY", "REGAL", "REGES", "REGIA", "REHEM", 
            "REICH", "REIFY", "REIGN", "REINS", "REKEY", "RELAX", "RELAY", 
            "RELET", "RELIC", "RELIT", "REMAN", "REMAP", "REMET", "REMIT", 
            "REMIX", "RENAL", "RENDS", "RENEW", "RENIG", "RENTS", "REOIL", 
            "REPAY", "REPEL", "REPIN", "REPLY", "REPRO", "RERAN", "RERUN", 
            "RESAW", "RESAY", "RESEE", "RESET", "RESEW", "RESIN", "RESOW", 
            "RESTS", "RETCH", "RETIE", "RETRO", "RETRY", "REUSE", "REVEL", 
            "REVUE", "REWAX", "REWED", "REWIN", "REWON", "REXES", "RHEAS", 
            "RHEUM", "RHINE", "RHINO", "RHOMB", "RHUMB", "RHYME", "RHYTA", 
            "RIALS", "RIBBY", "RICED", "RICER", "RICES", "RICHE", "RICKS", 
            "RIDER", "RIDES", "RIDGE", "RIDGY", "RIELS", "RIFER", "RIFFS", 
            "RIFLE", "RIFTS", "RIGHT", "RIGID", "RIGOR", "RILED", "RILES", 
            "RILLS", "RIMED", "RIMES", "RINDS", "RINGS", "RINKS", "RINSE", 
            "RIOTS", "RIPEN", "RIPER", "RIPES", "RISEN", "RISER", "RISES", 
            "RISHI", "RISKS", "RISKY", "RISUS", "RITES", "RITZY", "RIVAL", 
            "RIVED", "RIVEN", "RIVER", "RIVES", "RIVET", "RIYAL", "ROACH", 
            "ROADS", "ROAMS", "ROANS", "ROARS", "ROAST", "ROBED", "ROBES", 
            "ROBIN", "ROBLE", "ROBOT", "ROCKS", "ROCKY", "RODEO", "ROGER", 
            "ROGUE", "ROILS", "ROILY", "ROLES", "ROLLS", "ROMAN", "ROMEO", 
            "ROMPS", "RONDO", "ROODS", "ROOFS", "ROOKS", "ROOKY", "ROOMS", 
            "ROOMY", "ROOST", "ROOTS", "ROOTY", "ROPED", "ROPER", "ROPES", 
            "ROSED", "ROSES", "ROSHI", "ROSIN", "ROTES", "ROTOR", "ROUES", 
            "ROUGE", "ROUGH", "ROUND", "ROUSE", "ROUST", "ROUTE", "ROUTS", 
            "ROVED", "ROVER", "ROVES", "ROWAN", "ROWDY", "ROWED", "ROWEL", 
            "ROWER", "ROYAL", "RUBES", "RUBLE", "RUCKS", "RUDDY", "RUDER", 
            "RUERS", "RUFFS", "RUGBY", "RUING", "RUINS", "RULED", "RULER", 
            "RULES", "RUMBA", "RUMMY", "RUMOR", "RUMPS", "RUNES", "RUNGS", 
            "RUNIC", "RUNNY", "RUNTS", "RUNTY", "RUPEE", "RURAL", "RUSES", 
            "RUSHY", "RUSKS", "RUSSE", "RUSTS", "RUSTY", "RUTHS", "RUTTY", 
            "SABER", "SABLE", "SABOT", "SABRA", "SACKS", "SACRA", "SADHU", 
            "SADLY", "SAFER", "SAFES", "SAGAS", "SAGER", "SAGES", "SAGGY", 
            "SAGOS", "SAHIB", "SAIDS", "SAILS", "SAINT", "SAITH", "SAKES", 
            "SAKIS", "SALAD", "SALEM", "SALES", "SALLY", "SALON", "SALSA", 
            "SALTS", "SALTY", "SALVE", "SALVO", "SAMBA", "SAMBO", "SAMOA", 
            "SANDS", "SANDY", "SANED", "SANER", "SANES", "SANGA", "SANGH", 
            "SANKA", "SANTA", "SAPID", "SAPOR", "SAPPY", "SARAH", "SARAN", 
            "SAREE", "SARGE", "SARIS", "SAROD", "SASSY", "SATAN", "SATED", 
            "SATES", "SATIN", "SATYR", "SAUCE", "SAUCY", "SAUDI", "SAULS", 
            "SAULT", "SAUNA", "SAUTE", "SAVED", "SAVER", "SAVES", "SAVOR", 
            "SAVOY", "SAVVY", "SAWED", "SAWER", "SAXES", "SAXON", "SAYEE", 
            "SAYER", "SAYST", "SCABS", "SCADS", "SCAGS", "SCALD", "SCALE", 
            "SCALP", "SCALY", "SCAMP", "SCAMS", "SCANS", "SCANT", "SCAPE", 
            "SCARE", "SCARF", "SCARP", "SCARS", "SCARY", "SCATS", "SCENE", 
            "SCENT", "SCHMO", "SCHUL", "SCHWA", "SCION", "SCOFF", "SCOLD", 
            "SCONE", "SCOOP", "SCOOT", "SCOPE", "SCORE", "SCORN", "SCOTS", 
            "SCOTT", "SCOUR", "SCOUT", "SCOWL", "SCOWS", "SCRAG", "SCRAM", 
            "SCRAP", "SCREE", "SCREW", "SCRIM", "SCRIP", "SCROD", "SCRUB", 
            "SCUBA", "SCUDS", "SCUFF", "SCULK", "SCULL", "SCULP", "SCUMS", 
            "SCUPS", "SCURF", "SCUTA", "SCUTE", "SCUTS", "SEALS", "SEAMS", 
            "SEAMY", "SEARS", "SEATS", "SECTS", "SEDAN", "SEDER", "SEDGE", 
            "SEDGY", "SEDUM", "SEEDS", "SEEDY", "SEEKS", "SEELS", "SEEMS", 
            "SEEPS", "SEEPY", "SEERS", "SEGNO", "SEGOS", "SEGUE", "SEIGE", 
            "SEINE", "SEISM", "SEIZE", "SELFS", "SELLS", "SEMEN", "SEMIS", 
            "SENDS", "SENNA", "SENOR", "SENSE", "SENSU", "SENTI", "SEOUL", 
            "SEPAL", "SEPIA", "SEPOY", "SEPTA", "SEPTS", "SERAL", "SERED", 
            "SERER", "SERES", "SERFS", "SERGE", "SERIF", "SERIN", "SEROW", 
            "SERUM", "SERVE", "SERVO", "SETAE", "SETAL", "SETON", "SETUP", 
            "SEVEN", "SEVER", "SEWED", "SEWER", "SEXED", "SEXES", "SEXTO", 
            "SEXTS", "SHACK", "SHADE", "SHADS", "SHADY", "SHAFT", "SHAGS", 
            "SHAHS", "SHAKE", "SHAKO", "SHAKY", "SHALE", "SHALL", "SHALT", 
            "SHALY", "SHAME", "SHAMS", "SHANK", "SHAPE", "SHARD", "SHARE", 
            "SHARK", "SHARP", "SHAVE", "SHAWL", "SHAWM", "SHAWN", "SHAWS", 
            "SHAYS", "SHEAF", "SHEAR", "SHEDS", "SHEEN", "SHEEP", "SHEER", 
            "SHEET", "SHEIK", "SHELF", "SHELL", "SHEOL", "SHERD", "SHEWN", 
            "SHEWS", "SHIED", "SHIER", "SHIES", "SHIFT", "SHILL", "SHILY", 
            "SHIMS", "SHINE", "SHINS", "SHINY", "SHIPS", "SHIPT", "SHIRE", 
            "SHIRK", "SHIRR", "SHIRT", "SHISH", "SHIST", "SHITS", "SHIVA", 
            "SHIVE", "SHIVS", "SHLEP", "SHOAL", "SHOAT", "SHOCK", "SHOED", 
            "SHOER", "SHOES", "SHOJI", "SHONE", "SHOOK", "SHOOS", "SHOOT", 
            "SHOPE", "SHOPS", "SHORE", "SHORN", "SHORT", "SHOTE", "SHOTS", 
            "SHOUT", "SHOVE", "SHOWN", "SHOWS", "SHOWY", "SHRED", "SHREW", 
            "SHRUB", "SHRUG", "SHUCK", "SHULS", "SHUNS", "SHUNT", "SHUSH", 
            "SHUTE", "SHUTS", "SHYER", "SHYLY", "SIBYL", "SICKS", "SIDED", 
            "SIDES", "SIDLE", "SIEGE", "SIEUR", "SIEVE", "SIFTS", "SIGHS", 
            "SIGHT", "SIGIL", "SIGMA", "SIGNS", "SIKHS", "SILEX", "SILKS", 
            "SILKY", "SILLS", "SILLY", "SILOS", "SILTS", "SILTY", "SILVA", 
            "SIMON", "SIMPS", "SINCE", "SINES", "SINEW", "SINGE", "SINGS", 
            "SINHS", "SINKS", "SINUS", "SIOUX", "SIPPY", "SIRED", "SIREE", 
            "SIREN", "SIRES", "SIRUP", "SISAL", "SISSY", "SITAR", "SITED", 
            "SITES", "SITUP", "SITUS", "SIXES", "SIXTE", "SIXTH", "SIXTY", 
            "SIZED", "SIZER", "SIZES", "SKAGS", "SKALD", "SKATE", "SKEAN", 
            "SKEET", "SKEIN", "SKEWS", "SKIDS", "SKIED", "SKIER", "SKIES", 
            "SKIEY", "SKIFF", "SKIIS", "SKILL", "SKIMP", "SKIMS", "SKINK", 
            "SKINS", "SKIPS", "SKIRL", "SKIRT", "SKITS", "SKOAL", "SKUAS", 
            "SKULK", "SKULL", "SKUNK", "SKYED", "SKYEY", "SLABS", "SLACK", 
            "SLAGS", "SLAIN", "SLAKE", "SLAMS", "SLANG", "SLANT", "SLAPS", 
            "SLASH", "SLATE", "SLATS", "SLATY", "SLAVE", "SLAVS", "SLAWS", 
            "SLAYS", "SLEDS", "SLEEK", "SLEEP", "SLEET", "SLEPT", "SLEWS", 
            "SLICE", "SLICK", "SLIDE", "SLIER", "SLILY", "SLIME", "SLIMS", 
            "SLIMY", "SLING", "SLINK", "SLIPS", "SLIPT", "SLITS", "SLOBS", 
            "SLOES", "SLOGS", "SLOOP", "SLOPE", "SLOPS", "SLOSH", "SLOTH", 
            "SLOTS", "SLOWS", "SLUBS", "SLUED", "SLUES", "SLUGS", "SLUMP", 
            "SLUMS", "SLUNG", "SLUNK", "SLURP", "SLURS", "SLUSH", "SLUTS", 
            "SLYER", "SLYLY", "SMACK", "SMALL", "SMART", "SMASH", "SMEAR", 
            "SMELL", "SMELT", "SMILE", "SMIRK", "SMITE", "SMITH", "SMOCK", 
            "SMOKE", "SMOKY", "SMOTE", "SMUTS", "SNACK", "SNAFU", "SNAGS", 
            "SNAIL", "SNAKE", "SNAKY", "SNAPS", "SNARE", "SNARK", "SNARL", 
            "SNEAK", "SNEER", "SNICK", "SNIDE", "SNIFF", "SNIPE", "SNIPS", 
            "SNITS", "SNOBS", "SNOOD", "SNOOP", "SNOOT", "SNORE", "SNORT", 
            "SNOTS", "SNOUT", "SNOWS", "SNOWY", "SNUBS", "SNUCK", "SNUFF", 
            "SNUGS", "SOAKS", "SOAPS", "SOAPY", "SOARS", "SOAVE", "SOBER", 
            "SOCKS", "SODAS", "SODDY", "SODOM", "SOFAR", "SOFAS", "SOFIA", 
            "SOFTS", "SOFTY", "SOGGY", "SOILS", "SOLAR", "SOLED", "SOLES", 
            "SOLID", "SOLOS", "SOLVE", "SOMAS", "SONAR", "SONDE", "SONES", 
            "SONGS", "SONIC", "SONNY", "SOOEY", "SOOTH", "SOOTS", "SOOTY", 
            "SOPHS", "SOPHY", "SOPOR", "SOPPY", "SOREL", "SORER", "SORES", 
            "SORRY", "SORTS", "SOUGH", "SOULS", "SOUND", "SOUPS", "SOUPY", 
            "SOURS", "SOUSE", "SOUTH", "SOWED", "SOWER", "SOYAS", "SPACE", 
            "SPADE", "SPAIN", "SPAKE", "SPALE", "SPANK", "SPANS", "SPARE", 
            "SPARK", "SPARS", "SPASM", "SPATE", "SPATS", "SPAWN", "SPAYS", 
            "SPEAK", "SPEAR", "SPECK", "SPECS", "SPEED", "SPELL", "SPELT", 
            "SPEND", "SPENT", "SPERM", "SPEWS", "SPICA", "SPICE", "SPICK", 
            "SPICS", "SPICY", "SPIED", "SPIEL", "SPIER", "SPIES", "SPIFF", 
            "SPIKE", "SPIKY", "SPILL", "SPILT", "SPINE", "SPINS", "SPINY", 
            "SPIRE", "SPIRY", "SPITE", "SPITS", "SPITZ", "SPLAT", "SPLAY", 
            "SPLIT", "SPOIL", "SPOKE", "SPOOF", "SPOOK", "SPOOL", "SPOON", 
            "SPOOR", "SPORE", "SPORT", "SPOTS", "SPOUT", "SPRAT", "SPRAY", 
            "SPREE", "SPRIG", "SPRIT", "SPUDS", "SPUED", "SPUES", "SPUME", 
            "SPUMY", "SPUNK", "SPURN", "SPURS", "SPURT", "SPUTA", "SQUAB", 
            "SQUAD", "SQUAT", "SQUAW", "SQUIB", "SQUID", "STABS", "STACK", 
            "STAFF", "STAGE", "STAGS", "STAGY", "STAID", "STAIN", "STAIR", 
            "STAKE", "STALE", "STALK", "STALL", "STAMP", "STAND", "STANK", 
            "STAPH", "STARE", "STARK", "STARS", "START", "STASH", "STATE", 
            "STATS", "STAVE", "STAYS", "STEAD", "STEAK", "STEAL", "STEAM", 
            "STEED", "STEEL", "STEEP", "STEER", "STEIN", "STELE", "STEMS", 
            "STENO", "STEPS", "STERE", "STERN", "STETS", "STEVE", "STEWS", 
            "STICK", "STIED", "STIES", "STIFF", "STILE", "STILL", "STILT", 
            "STIMY", "STING", "STINK", "STINT", "STIRS", "STOAS", "STOAT", 
            "STOCK", "STOGY", "STOIC", "STOKE", "STOLE", "STOMP", "STONE", 
            "STONY", "STOOD", "STOOL", "STOOP", "STOPS", "STOPT", "STORE", 
            "STORK", "STORM", "STORY", "STOUP", "STOUT", "STOVE", "STOWS", 
            "STRAP", "STRAW", "STRAY", "STREP", "STREW", "STRIA", "STRIP", 
            "STROP", "STRUM", "STRUT", "STUBS", "STUCK", "STUDS", "STUDY", 
            "STUFF", "STUMP", "STUNG", "STUNK", "STUNS", "STUNT", "STUPA", 
            "STUPE", "STYED", "STYES", "STYLE", "STYLI", "STYMY", "SUAVE", 
            "SUCKS", "SUCRE", "SUDAN", "SUDOR", "SUDSY", "SUEDE", "SUERS", 
            "SUETS", "SUETY", "SUGAR", "SUING", "SUITE", "SUITS", "SULFA", 
            "SULKS", "SULKY", "SULLY", "SUMAC", "SUMMA", "SUMOS", "SUMPS", 
            "SUNNY", "SUNUP", "SUPER", "SUPES", "SUPRA", "SURDS", "SURER", 
            "SURFS", "SURFY", "SURGE", "SURGY", "SURLY", "SUSAN", "SUTRA", 
            "SUTTA", "SWABS", "SWAGE", "SWAIL", "SWAIN", "SWALE", "SWAMI", 
            "SWAMP", "SWANG", "SWANK", "SWANS", "SWAPS", "SWARD", "SWARM", 
            "SWART", "SWASH", "SWATH", "SWATS", "SWAYS", "SWEAR", "SWEAT", 
            "SWEDE", "SWEEP", "SWEET", "SWELL", "SWEPT", "SWIFT", "SWIGS", 
            "SWILL", "SWIMS", "SWINE", "SWING", "SWIPE", "SWIRL", "SWISH", 
            "SWISS", "SWOON", "SWOOP", "SWOPS", "SWORD", "SWORE", "SWORN", 
            "SWUNG", "SYLPH", "SYNCH", "SYNCS", "SYNOD", "SYREN", "SYRIA", 
            "SYRUP", "TABBY", "TABLA", "TABLE", "TABOO", "TABOR", "TACET", 
            "TACHS", "TACIT", "TACKS", "TACKY", "TACOS", "TACTS", "TAELS", 
            "TAFFY", "TAIGA", "TAILS", "TAINT", "TAKEN", "TAKER", "TAKES", 
            "TALCS", "TALER", "TALES", "TALKS", "TALKY", "TALLY", "TALON", 
            "TALUS", "TAMED", "TAMER", "TAMES", "TAMMY", "TAMPA", "TAMPS", 
            "TANGO", "TANGS", "TANGY", "TANKA", "TANKS", "TANSY", "TANTO", 
            "TAPED", "TAPER", "TAPES", "TAPIR", "TARDE", "TARDO", "TARDY", 
            "TARED", "TARES", "TARNS", "TAROS", "TAROT", "TARPS", "TARRY", 
            "TARSI", "TARTS", "TASKS", "TASTE", "TASTY", "TATAR", "TATER", 
            "TATOO", "TATTY", "TAUNT", "TAUPE", "TAUTS", "TAWNY", "TAXED", 
            "TAXER", "TAXES", "TAXIS", "TAZZA", "TAZZE", "TEACH", "TEAKS", 
            "TEALS", "TEAMS", "TEARS", "TEARY", "TEASE", "TEATS", "TECHY", 
            "TECUM", "TEDDY", "TEEMS", "TEENS", "TEENY", "TEETH", "TELEX", 
            "TELLS", "TELLY", "TEMPI", "TEMPO", "TEMPS", "TEMPT", "TENCH", 
            "TENDS", "TENET", "TENON", "TENOR", "TENSE", "TENTH", "TENTS", 
            "TENTY", "TEPEE", "TEPID", "TERCE", "TERMS", "TERNE", "TERNS", 
            "TERRA", "TERRE", "TERRY", "TERSE", "TESLA", "TESTS", "TESTY", 
            "TETRA", "TEXAN", "TEXAS", "TEXTS", "THANE", "THANK", "THATS", 
            "THAWS", "THEFT", "THEIR", "THEME", "THENS", "THERE", "THERM", 
            "THESE", "THETA", "THEWS", "THEWY", "THICK", "THIEF", "THIGH", 
            "THINE", "THING", "THINK", "THINS", "THIRD", "THOLE", "THONG", 
            "THORN", "THORO", "THORP", "THOSE", "THOUS", "THREE", "THREW", 
            "THRIP", "THROB", "THROE", "THROW", "THRUM", "THUDS", "THUGS", 
            "THUMB", "THUMP", "THYME", "THYMI", "THYMY", "TIARA", "TIBER", 
            "TIBET", "TIBIA", "TICKS", "TIDAL", "TIDED", "TIDES", "TIERS", 
            "TIFFS", "TIGER", "TIGHT", "TIKES", "TIKIS", "TILDE", "TILED", 
            "TILER", "TILES", "TILLS", "TILTH", "TILTS", "TIMED", "TIMER", 
            "TIMES", "TIMID", "TINCT", "TINED", "TINES", "TINGE", "TINGS", 
            "TINNY", "TINTS", "TIPIS", "TIPPY", "TIPSY", "TIRED", "TIRES", 
            "TIROS", "TITAN", "TITER", "TITHE", "TITLE", "TITRE", "TITTY", 
            "TIZZY", "TOADS", "TOADY", "TOAST", "TODAY", "TODDY", "TOFFS", 
            "TOFFY", "TOFTS", "TOFUS", "TOGAE", "TOGAS", "TOILS", "TOKAY", 
            "TOKED", "TOKEN", "TOKES", "TOKYO", "TOLLS", "TOMBS", "TOMES", 
            "TOMMY", "TONAL", "TONED", "TONER", "TONES", "TONGS", "TONIC", 
            "TONNE", "TOOLS", "TOOTH", "TOOTS", "TOPAZ", "TOPED", "TOPER", 
            "TOPES", "TOPIC", "TOPOS", "TOQUE", "TORAH", "TORAS", "TORCH", 
            "TORCS", "TORES", "TORII", "TOROS", "TORSI", "TORSO", "TORTE", 
            "TORTS", "TORUS", "TOTAL", "TOTED", "TOTEM", "TOTER", "TOTES", 
            "TOUCH", "TOUGH", "TOURS", "TOUTS", "TOWED", "TOWEL", "TOWER", 
            "TOWNS", "TOWNY", "TOXIC", "TOXIN", "TOYED", "TOYER", "TOYON", 
            "TOYOS", "TRACE", "TRACK", "TRACT", "TRADE", "TRAIL", "TRAIN", 
            "TRAIT", "TRAMP", "TRAMS", "TRAPS", "TRAPT", "TRASH", "TRAVE", 
            "TRAWL", "TRAYS", "TREAD", "TREAT", "TREED", "TREES", "TREKS", 
            "TREND", "TRESS", "TRETS", "TREWS", "TREYS", "TRIAD", "TRIAL", 
            "TRIBE", "TRICE", "TRICK", "TRIED", "TRIER", "TRIES", "TRILL", 
            "TRIMS", "TRINE", "TRIOS", "TRIPE", "TRIPS", "TRITE", "TRODE", 
            "TROIS", "TROLL", "TROMP", "TROOP", "TROPE", "TROTH", "TROTS", 
            "TROUT", "TROVE", "TROWS", "TROYS", "TRUCE", "TRUCK", "1DORT", 
            "1ROTE", "1SROT", "TRULL", "TRULY", "TRUMP", "TRUNK", "TRUSS", 
            "TRUST", "TRUTH", "TRYST", "TSARS", "TSKED", "TSUBA", "TUBAL", 
            "TUBAS", "TUBBY", "TUBED", "TUBER", "TUBES", "TUCKS", "TUDOR", 
            "TUFAS", "TUFFS", "TUFTS", "TUFTY", "TULES", "TULIP", "TULLE", 
            "TULSA", "TUMID", "TUMMY", "TUMOR", "TUMPS", "TUNAS", "TUNED", 
            "TUNER", "TUNES", "TUNIC", "TUNIS", "TUNNY", "TUQUE", "TURBO", 
            "TURDS", "TURFS", "TURFY", "TURKS", "TURNS", "TURPS", "TUSKS", 
            "TUTEE", "TUTOR", "TUTTI", "TUTUS", "TUXES", "TWAIN", "TWANG", 
            "TWATS", "TWEAK", "TWEED", "TWEEN", "TWEET", "TWERP", "TWICE", 
            "TWIER", "TWIGS", "TWILL", "TWINE", "TWINS", "TWINY", "TWIRL", 
            "TWIRP", "TWIST", "TWITS", "TWIXT", "TYING", "TYKES", "TYLER", 
            "TYPAL", "TYPED", "TYPES", "TYPIC", "TYPOS", "TYRED", "TYRES", 
            "TYROS", "TZARS", "UDDER", "UGLIS", "UKASE", "ULCER", "ULNAE", 
            "ULNAR", "ULNAS", "ULTRA", "ULVAS", "UMBEL", "UMBER", "UMBRA", 
            "UMIAK", "UMPED", "UNAPT", "UNARM", "UNARY", "UNBAR", "UNBID", 
            "UNBOX", "UNCAP", "UNCLE", "UNCOS", "UNCUT", "UNDER", "UNDID", 
            "UNDUE", "UNFED", "UNFIT", "UNFIX", "UNGOT", "UNHAT", "UNHIP", 
            "UNIFY", "UNION", "UNITE", "UNITS", "UNITY", "UNLAW", "UNLAY", 
            "UNLED", "UNLET", "UNLIT", "UNMAN", "UNMET", "UNPEG", "UNPEN", 
            "UNPIN", "UNRIG", "UNRIP", "UNSAY", "UNSET", "UNSEW", "UNSEX", 
            "UNTIE", "UNTIL", "UNWED", "UNWIT", "UNWON", "UNZIP", "UPEND", 
            "UPPED", "UPPER", "UPSET", "URBAN", "UREAL", "UREAS", "UREIC", 
            "URGED", "URGER", "URGES", "URINE", "URSAE", "USAGE", "USERS", 
            "USHER", "USING", "USUAL", "USURP", "USURY", "UTERI", "UTERO", 
            "UTILE", "UTTER", "UVEAL", "UVEAS", "UVULA", "VACUA", "VACUO", 
            "VADIS", "VAGAL", "VAGUE", "VAGUS", "VALES", "VALET", "VALID", 
            "VALOR", "VALSE", "VALUE", "VALVA", "VALVE", "VAMPS", "VANED", 
            "VANES", "VAPID", "VAPOR", "VASES", "VASTS", "VASTY", "VATIC", 
            "VAULT", "VAUNT", "VEALS", "VEALY", "VEDIC", "VEEPS", "VEERS", 
            "VEERY", "VEGAN", "VEGAS", "VEILS", "VEINS", "VEINY", "VELAR", 
            "VELDS", "VELDT", "VELUM", "VENAL", "VENDS", "VENIN", "VENOM", 
            "VENTS", "VENUE", "VENUS", "VERBS", "VERDE", "VERDI", "VERGE", 
            "VERSA", "VERSE", "VERSO", "VERVE", "VESTS", "VETCH", "VEXED", 
            "VEXER", "VEXES", "VIALS", "VIAND", "VIBES", "VICAR", "VICED", 
            "VICES", "VICHY", "VIDEO", "VIERS", "VIEWS", "VIEWY", "VIGIL", 
            "VIGOR", "VILER", "VILLA", "VILLI", "VINAL", "VINAS", "VINCA", 
            "VINED", "VINES", "VINIC", "VINOS", "VINYL", "VIOLA", "VIOLS", 
            "VIPER", "VIRAL", "VIREO", "VIRGO", "VIRID", "VIRTU", "VIRUS", 
            "VISAS", "VISED", "VISES", "VISIT", "VISOR", "VISTA", "VITAE", 
            "VITAL", "VITRO", "VIVID", "VIVRE", "VIXEN", "VIZIR", "VIZOR", 
            "VOCAL", "VOCES", "VODKA", "VOGUE", "VOICE", "VOIDS", "VOILA", 
            "VOILE", "VOLES", "VOLGA", "VOLTA", "VOLTS", "VOMIT", "VOTED", 
            "VOTER", "VOTES", "VOUCH", "VOWED", "VOWEL", "VOWER", "VROOM", 
            "VROUW", "VROWS", "VUGGS", "VUGGY", "VUGHS", "VULGO", "VULVA", 
            "VYING", "WACKS", "WACKY", "WADDY", "WADED", "WADER", "WADES", 
            "WADIS", "WAFER", "WAFTS", "WAGED", "WAGER", "WAGES", "WAGON", 
            "WAHOO", "WAIFS", "WAILS", "WAINS", "WAIST", "WAITS", "WAIVE", 
            "WAKED", "WAKEN", "WAKER", "WAKES", "WALED", "WALER", "WALES", 
            "WALKS", "WALLA", "WALLS", "WALLY", "WALTZ", "WANDS", "WANED", 
            "WANES", "WANLY", "WANTS", "WARDS", "WARED", "WARES", "WARKS", 
            "WARMS", "WARNS", "WARPS", "WARTS", "WARTY", "WASHY", "WASPS", 
            "WASPY", "WASTE", "WASTS", "WATCH", "WATER", "WATTS", "WAUGH", 
            "WAULS", "WAVED", "WAVER", "WAVES", "WAVEY", "WAWLS", "WAXED", 
            "WAXEN", "WAXER", "WAXES", "WAYNE", "WE'RE", "WE'VE", "WEALD", 
            "WEALS", "WEANS", "WEARS", "WEARY", "WEAVE", "WEBBY", "WEBER", 
            "WEDGE", "WEDGY", "WEEDS", "WEEDY", "WEEKS", "WEENS", "WEENY", 
            "WEEPS", "WEEPY", "WEEST", "WEFTS", "WEIGH", "WEIRD", "WEIRS", 
            "WELCH", "WELDS", "WELLS", "WELSH", "WELTS", "WENCH", "WENDS", 
            "WENNY", "WESTS", "WETLY", "WHACK", "WHALE", "WHAMS", "WHANG", 
            "WHAPS", "WHARF", "WHATS", "WHEAL", "WHEAT", "WHEEL", "WHELK", 
            "WHELM", "WHELP", "WHENS", "WHERE", "WHETS", "WHEWS", "WHEYS", 
            "WHICH", "WHIFF", "WHIGS", "WHILE", "WHIMS", "WHINE", "WHINY", 
            "WHIPS", "WHIPT", "WHIRL", "WHIRR", "WHIRS", "WHISH", "WHISK", 
            "WHIST", "WHITE", "WHITS", "WHITY", "WHIZZ", "WHOLE", "WHOMP", 
            "WHOOP", "WHOPS", "WHORE", "WHORL", "WHOSE", "WHOSO", "WHUMP", 
            "WICKS", "WIDEN", "WIDER", "WIDES", "WIDOW", "WIDTH", "WIELD", 
            "WIERD", "WIFED", "WIFES", "WIGHT", "WILCO", "WILDS", "WILED", 
            "WILES", "WILLS", "WILLY", "WILTS", "WINCE", "WINCH", "WINDS", 
            "WINDY", "WINED", "WINES", "WINEY", "WINGS", "WINGY", "WINKS", 
            "WINOS", "WIPED", "WIPER", "WIPES", "WIRED", "WIRER", "WIRES", 
            "WISED", "WISER", "WISES", "WISHY", "WISPS", "WISPY", "WISTS", 
            "WITCH", "WITHE", "WITHY", "WITTY", "WIVED", "WIVER", "WIVES", 
            "WIZEN", "WIZES", "WOADS", "WOALD", "WOFUL", "WOKEN", "WOLDS", 
            "WOLFS", "WOMAN", "WOMBS", "WOMBY", "WOMEN", "WONKY", "WONTS", 
            "WOODS", "WOODY", "WOOED", "WOOER", "WOOFS", "WOOLS", "WOOLY", 
            "WOOPS", "WOOSH", "WOOZY", "WORDS", "WORDY", "WORKS", "WORLD", 
            "WORMS", "WORMY", "WORRY", "WORSE", "WORST", "WORTH", "WORTS", 
            "WOULD", "WOUND", "WOVEN", "WOWED", "WRACK", "WRANG", "WRAPS", 
            "WRAPT", "WRATH", "WREAK", "WRECK", "WRENS", "WREST", "WRIED", 
            "WRIER", "WRIES", "WRING", "WRIST", "WRITE", "WRITS", "WRONG", 
            "WROTE", "WROTH", "WRUNG", "WRYER", "WRYLY", "WURST", "XEBEC", 
            "XENIA", "XENIC", "XENON", "XERIC", "XEROX", "XVIII", "XXIII", 
            "XYLAN", "XYLEM", "XYSTS", "YACHT", "YACKS", "YAHOO", "YAMEN", 
            "YAMUN", "YANKS", "YARDS", "YARER", "YARNS", "YAWED", "YAWLS", 
            "YAWNS", "YAWPS", "YEARN", "YEARS", "YEAST", "YEGGS", "YELLS", 
            "YELPS", "YEMEN", "YENTA", "YERBA", "YESES", "YETIS", "YIELD", 
            "YIPES", "YODEL", "YODHS", "YODLE", "YOGAS", "YOGEE", "YOGHS", 
            "YOGIC", "YOGIN", "YOGIS", "YOKED", "YOKEL", "YOKES", "YOLKS", 
            "YOLKY", "YONIS", "YORES", "YOU'D", "YOUNG", "YOURN", "YOURS", 
            "YOUSE", "YOUTH", "YOWED", "YOWIE", "YOWLS", "YUCCA", "YUKON", 
            "YULES", "YUMMY", "YURTS", "ZAIRE", "ZARFS", "ZAZEN", "ZEALS", 
            "ZEBRA", "ZEBUS", "ZEINS", "ZEISS", "ZENDO", "ZEROS", "ZESTS", 
            "ZESTY", "ZETAS", "ZILCH", "ZINCS", "ZINCY", "ZINGS", "ZINGY", 
            "ZINKY", "ZIPPY", "ZITIS", "ZLOTY", "ZOEAS", "ZOMBI", "ZONAL", 
            "ZONED", "ZONER", "ZONES", "ZOOID", "ZOOKS", "ZOOMS", "ZOONS", 
            "ZOWIE", "ZULUS", "ZUNIS"
        }; */// (1198 - 211)* 7 + 3 

    // TODO get these values from the file
    static const int WORD_POOL_SIZE = 6919; 
    static const int WORD_SIZE = 5;
    static const int WORDS_CHOSEN = 15; 
    //file_write_module(wordArr, WORD_POOL_SIZE);
    //char** wordArr = PasswordFileReader("passwords5");

    /* Place a word in the string total times, making sure it doesn't 
       overwrite another word or get placed right next to it */
    int place;                      /* Current place for checking and word insertion*/
    int takenWords[WORDS_CHOSEN];   /* Words already placed in bigString */
    int valid;                      /* 1 if selected word is not already used and 
                                       does not conflict with other words, 0 otherwise */
    int pickedWord = 0;             /* Indicate whether or not we've chosen the correct word */
    int left = WORDS_CHOSEN;        /* # of words that still need to be chosen */
    char correctWord[WORD_SIZE];    /* the correct word */
    while(left>0){  
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
                    bigString[i] = wordArr[wordLoc][i-place];
                    /* If this is the first word chosen, it is the correct word. */
                    if(!pickedWord)
                        correctWord[i-place] = wordArr[wordLoc][i-place];
                }
                pickedWord = 1;
                left--;
            }
        }
    }
    
    
    /* Create and fill an array to keep track of which brackets were used */
    int usedBrackets[BIGSTRING_SIZE];
    for(i=0; i<BIGSTRING_SIZE; i++){
        usedBrackets[i] = 1;
    }
    
    
    /* Print the hex and the filled bigString */
    char temp[12];  
    int current = 0;
    int j = 0;
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
    j = 0;
    
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
    int allowances = 4;
    
    while(1){
        getyx(stdscr,y,x);
        
        /* Get allowances left */
        mvprintw(1,0,"                                 ");
        mvprintw(3,0,"                              ");
        switch(allowances){
            case 1: mvprintw(3,0,"1 ATTEMPT(S) LEFT: *");
                    attron(A_BLINK);
                    mvprintw(1,0,"!!! WARNING: LOCKOUT IMNINENT !!!");
                    attroff(A_BLINK);
                    attron(A_BOLD);
                    break;
            case 2: mvprintw(3,0,"2 ATTEMPT(S) LEFT: * *");
                    mvprintw(1,0,"ENTER PASSWORD NOW");
                    break;
            case 3: mvprintw(3,0,"3 ATTEMPT(S) LEFT: * * *");
                    mvprintw(1,0,"ENTER PASSWORD NOW");
                    break;
            case 4: mvprintw(3,0,"4 ATTEMPT(S) LEFT: * * * *");
                    mvprintw(1,0,"ENTER PASSWORD NOW");
                    break;
            case 0: clear();
                    mvprintw(10,20,"TERMINAL LOCKED");
                    mvprintw(12,12,"PLEASE CONTACT AN ADMINISTRATOR");
                    refresh();
                    getch(); //AUTHOR ERR  PATH AT THE NEXT LINE
                    endwin(); // 
                    exit(0);
        }
        refresh();
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
        if(needsClearingMultiLine){
            charCounter = 0;
            int timesDown = 0;
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
                if(rand()%5==0){
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
                    allowances = 4;
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
                    allowances--;
                }
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
