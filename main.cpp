#include "FEHLCD.h"
#include "FEHUtility.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "FEHImages.h"

using namespace std;


// Function prototypes
void getCurrentAndHighScore(int *, int *);
void writeScoreToFile(int);

/*
 * Button class. Takes in position and content text value. 
 */
class Button{
    public: 
        Button(char txt[100], float tl_x, float tl_y, int wid, int hei);
        void draw();
        bool touched(float touch_x, float touch_y);
        char content[30];
    private:
        float x;
        float y;
        int width;
        int height;
};

// constructs Button
Button::Button(char txt[100], float tl_x, float tl_y, int wid, int hei){
    strcpy(content,txt);
    x = tl_x;
    y = tl_y;
    width = wid;
    height = hei;
}


// draws the button with content
void Button::draw(){
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(x, y, width, height);
    LCD.WriteAt(content, x, y+2);
}

// Returns true if the given coordiantes are inside the Button. Else, returns false.
bool Button::touched(float touch_x, float touch_y){
    bool inside = false;
    if( (touch_x >= x && touch_x <= x + width) && (touch_y <= y + height && touch_y >= y)){
        inside = true;
    }
    return inside;
}


/*
 * Main Menu class. Takes in position and content text value. 
 */
class Main_Menu{
    public:
        Main_Menu(Button *pl, Button *ru,Button *st, Button *qu);
        int touchedButton(float touched_x, float touched_y);
        void draw();
    private:
        Button *playBTN;
        Button *rulesBTN;
        Button *quitBTN;
        Button *statsBTN;
};

// constructs the Main Menu class
Main_Menu::Main_Menu(Button *pl, Button *ru,Button *st, Button *qu){
    playBTN = pl;
    rulesBTN = ru;
    statsBTN = st;
    quitBTN = qu;
}

// draws the main menu with given buttons
void Main_Menu::draw(){
    FEHIMAGE mainMenubg;
    mainMenubg.Open("pics/FlappyFEHFEH.pic");
    LCD.Clear();
    mainMenubg.Draw(0,0);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("FlappyFEH",100, 80);
    (*playBTN).draw();
    (*rulesBTN).draw();
    (*statsBTN).draw();
    (*quitBTN).draw();
}

// returns the button in Main Menu that was touched. 1 = Play, 2 = Rules, 3 = Stats, 4 = Quit, and 0 = N/A.
int Main_Menu::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*playBTN).touched(touched_x,touched_y)){
        touched = 1;
    }
    else if((*rulesBTN).touched(touched_x,touched_y)){
        touched = 2;
    }
    else if((*statsBTN).touched(touched_x,touched_y)){
        touched = 3;
    }
    else if((*quitBTN).touched(touched_x,touched_y)){
        touched = 4;
    }
    return touched;
}

/*
 * Rules class
 */
class Rules{
    public:
        Rules(Button *bk);
        void draw();
        int touchedButton(float touched_x, float touched_y);
    private:
        Button *back;
};

Rules::Rules(Button *bk){
    back = bk;
}

// Arad: Work on formatting Rules
void Rules::draw(){
    LCD.Clear();
    (*back).draw();
    LCD.WriteAt("The Rules are:",0,0);
}

// touching the back button in Rules returns 1. Else, returns 0.
int Rules::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*back).touched(touched_x,touched_y)){
        touched = 1;
    }
    return touched;
}

/*
 * Play class
 */
class Play{
    public:
        Play(Button *bk, Button *rtry);
        void draw();
        int touchedButton(float touched_x, float touched_y);
        void drawPipes(int x, int top_y, int width);
        void clearPipes(int x, int top_y, int width);
        void movePipes();
        void drawFlappy(float x, float old_y, float y);
        void displayScore();
        void retryMenu();
        // sets the value of "hit" given the x value of the pipe
        void collided(int x, int top_y);
    private:
        Button *backBTN;
        Button *retryBTN;
        int scoreCounter = 0;
        int ground_x = 0;
        float initial_y, final_y;
        float initial_v, final_v;
        // hit = true means flappy hit a pipe or floor. hit = false means flappy hasn't hit anything
        bool hit = false;
};

// constructs the Play class with play and retry buttons
Play::Play(Button *bk, Button *rtry){
    backBTN = bk;
    retryBTN = rtry;
}

// NOT COMPLETE
void Play::draw(){
    LCD.Clear();

    //drawing in LIGHTSKY BLUE backgrounds 
    LCD.SetFontColor(LIGHTSKYBLUE);
    LCD.FillRectangle(0,0,320,220);

    // Drawing the ground
    FEHIMAGE ground;
    ground.Open("pics/groundFEH.pic");
    ground.Draw(220,ground_x);
    // LCD.SetFontColor(LIGHTGREEN);
    // LCD.FillRectangle(0,220,320,20);

    
    // setting initial y location to middle of screen
    initial_y = 110;
    // setting initial y velocity and final velocity to 0
    initial_v = 0;
    final_v = 0;

    // drawing flappy at initial y.
    drawFlappy(130,initial_y,initial_y);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("click to start", 120, 150);
    int arb_x, arb_y;
    // waiting for user input 
     while(!LCD.Touch(&arb_x, &arb_y)){
        // wait until screen is not being touched
    }
    while(LCD.Touch(&arb_x, &arb_y)){
        // wait until person lets go
    }

    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(130,0,320,220);

    //drawing in LIGHTSKY BLUE backgrounds 
    LCD.SetFontColor(LIGHTSKYBLUE);
    LCD.FillRectangle(0,0,320,220);

    // moving the pipes constantly to the left till flappy hasn't "hit" anything
    while(!hit){
        movePipes();
    }

    // writes to a local file the score, if it was higher than previous high score.
    writeScoreToFile(scoreCounter);
    retryMenu();

    // resetting collision to false and counter to 0
    hit = false;
    scoreCounter = 0;

    std::cout << hit;
    LCD.SetFontColor(WHITE);
}



// displays the pipes' left end at given x
void Play::drawPipes(int x, int top_y, int width){
    LCD.SetFontColor(DARKGREEN);
    LCD.FillRectangle(x,top_y,width,220-top_y);
    LCD.FillRectangle(x,0,width,top_y-66);
} 

// clears the previously drawn pipes located at x
void Play::clearPipes(int x, int top_y, int width){
    LCD.SetFontColor(LIGHTSKYBLUE);
    LCD.FillRectangle(x,top_y,width,220-top_y);
    LCD.FillRectangle(x,0,width,top_y-66);
}

// moves the given pipe from x = 320 to x = -40, while avoiding wrapping. Also increments the score counter
void Play::movePipes(){
    int x = 320;
    int top_y = rand() % (210 + 1 - 76) + 76;

    // repeat till the pipes are completely off the screen and the flappy bird hasn't hit anything.
    while(x >= -40 && !hit){

        // display score
        displayScore();

        // arbitrary x and y 
        float arb_x; 
        float arb_y;

        if(LCD.Touch(&arb_x, &arb_y)){
            // set the bird going up
            final_v = 4;
        } else {  
            // increase velocity of the bird downward
            final_v = initial_v - 0.8;
            // terminal velocity
            if(final_v <= -2){
                final_v = -4;
            }
        }

        float distanceFlappyMoved = (initial_v + final_v)/2;
        final_y = initial_y - distanceFlappyMoved;
        drawFlappy(160,initial_y, final_y);
        // updating the value of "hit".
        collided(x, top_y);

        initial_v = final_v;
        initial_y = final_y; 

        // removes wrapping of the pipe at x < 0
        if(x < 0){
            clearPipes(0,top_y, 30 + x+5);
            drawPipes(0, top_y, 30 + x);
            Sleep(3);
        } 
        // removes wrapping of the pipe at x > 290
        else if(x > 290){
            clearPipes(x+5,top_y, 320 - (x+5));
            drawPipes(x, top_y, 320 - x);
            Sleep(3);
        }
        // displays the pipe when x => 0 and x <= 290
        else {
            clearPipes(x+5,top_y, 30);
            drawPipes(x, top_y, 30);
            Sleep(3);
        }

        
        // incrementing counter if pipe pass center of screens
        if(x == 130){
            scoreCounter++;
        }

        // moves pipes back one
        x -= 5;
        

        // drawing the new ground 
        ground_x -=5;
        FEHIMAGE ground;
        ground.Open("pics/groundFEH.pic");
        ground.Draw(220,ground_x);
    }
}

// clears flappy bird at (x,old_y) and draws a flappy character at (x,y)
void Play::drawFlappy(float x, float old_y, float y){
    // opening balaji's picture
    FEHIMAGE balaji;
    balaji.Open("pics/balajiheadFEH.pic");

    FEHIMAGE ground;
    ground.Open("pics/groundFEH.pic");

    // Drawing flappy if the flappy is within bounds of LCD display
    if(y > -13){
        LCD.SetFontColor(LIGHTSKYBLUE);
        LCD.FillCircle(x,old_y,24);
        
        // drawing the flappy bird 
        balaji.Draw(y-8,x-8);

        // Drawing the ground to prevent marks on ground.
        ground.Draw(220,ground_x);
    }
}

// returns true if the flappy bird hit a pipe or the ground. Takes in the value
// of the pipe's x.
void Play::collided(int x, int top_y){
    // hiting the floor
    if(final_y >= 212){
        hit = true;
    }

    // hitting the pipe

    // checking if the pipe is inside the locations of the bird
    if(x >= 138 && x<=168){
        if(top_y - final_y <= 10 || final_y - (top_y - 56) <= 5){
            hit = true;
        } 
    }

}

// creates retry menu
void Play::retryMenu(){
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Score:",120,45);
    LCD.WriteAt(scoreCounter,144,80);
    (*backBTN).draw();
    (*retryBTN).draw();
}

// touching the back button in Play returns 1. Touching the retry button in Play returns 2. Else, returns 0.
int Play::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*backBTN).touched(touched_x,touched_y)){
        touched = 1;
    } else if((*retryBTN).touched(touched_x, touched_y)){
        touched = 2;
    }
    return touched;
}


void Play::displayScore(){
        // clearing the previous score and setting new score
        LCD.SetFontColor(LIGHTSKYBLUE);
        LCD.FillRectangle(290,0,26,19);
        LCD.SetFontColor(WHITE);
        LCD.WriteAt(scoreCounter,290,0);
}

/*
 * Stats class
 */
class Stats{
    public:
        Stats(Button *bk);
        void draw();
        int touchedButton(float touched_x, float touched_y);
    private:
        Button *back;
        int currentScore;
        int highScore;
};

// constructor for Stats class
Stats::Stats(Button *bk){
    back = bk;
}

// touching the back button in Stats returns 1. Else, returns 0.
int Stats::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*back).touched(touched_x,touched_y)){
        touched = 1;
    }
    return touched;
}

// draws the stats page including buttons, max score, and current score.
void Stats::draw(){
    LCD.SetFontColor(WHITE);
    LCD.Clear();
    (*back).draw();
    getCurrentAndHighScore(&currentScore, &highScore);
    LCD.WriteAt("Max Score:", 100, 0);
    LCD.WriteAt(highScore,159,59);
    LCD.WriteAt("Current Score:", 80, 119);
    LCD.WriteAt(currentScore,159, 178);
}

// writing the score to the file, if current score is higher than high score
void writeScoreToFile(int scoreCounter){
    int highScore;
    ifstream inputScores;
    ofstream outputScores;

    // opening the input scores file
    inputScores.open("scores.txt"); // 3 3
    
    // storing the current high score
    inputScores >> highScore; // 3

    // opening the output file
    outputScores.open("scores.txt");

    // checking if high score is less than input score
    if(highScore < scoreCounter){ // 3 < 1
        outputScores << scoreCounter << endl; 
    } else {
        outputScores << highScore << endl;
    }

    // printing the current score to the file
    outputScores << scoreCounter << endl; 

    // closing the input and output streams
    inputScores.close();
    outputScores.close();
}

// returns array with current and high score
void getCurrentAndHighScore(int *currentScore, int *highScore){
    ifstream inputScores;
    inputScores.open("scores.txt");
    inputScores >> *highScore >> *currentScore;
    inputScores.close();
}

// Checks if a button was clicked on given "onScreen".
bool touchedAnyButton(int x, int y, int *onScreen, Main_Menu *menu, Rules *rules, Play *play, Stats *stats){
    bool touched = false;

    // in Main Menu
    if((*onScreen) == 1){
        int buttonTouched = (*menu).touchedButton(x,y);
        
        // Play button was touched in main menu
        if(buttonTouched  == 1){
            *onScreen = 2;
            touched = true;
        }

        // "Rules" button was touched in main menu
        else if(buttonTouched == 2){
            *onScreen = 3;
            touched = true;
        }

        // "Stats" button was touched in main menu
        else if(buttonTouched == 3){
            // change to 4
            *onScreen = 4;
            touched = true;
        }

        // "Quit" button was touched in main menu
        else if(buttonTouched == 4){
            *onScreen = 5;
            touched = true;
        }
    }

    // in Play Screen
    else if((*onScreen) == 2){
        // Back button was touched in Play
        if((*play).touchedButton(x,y) == 1){
            *onScreen = 1;
            touched = true;
        }
        
        // Retry button was touched in Play
        if((*play).touchedButton(x,y) == 2){
            *onScreen = 2;
            touched = true;
        }
    }

    // in Rules Screen
    else if((*onScreen) == 3){
        // Back button was touched in Rules
        if((*rules).touchedButton(x,y) == 1){
            *onScreen = 1;
            touched = true;
        }
    }

    // in Stats Screen
    else if((*onScreen) == 4){
        // Back button was touched in Stats
        if((*stats).touchedButton(x,y) == 1){
            *onScreen = 1;
            touched = true;
        }
    }

    return touched;
}

// The initiater of the given screen. 
void initiate(int *onScreen, Main_Menu *menu, Rules *rules, Play *play, Stats *stats)
{
    if(*onScreen == 1){
        // the actions main menu performs
        (*menu).draw();
    }
    else if(*onScreen == 2){
        // the actions play performs
        (*play).draw();
    }
    else if(*onScreen == 3){
        // the actions rules performs
        (*rules).draw();
    }
    else if(*onScreen == 4){
        // the actions stats performs
        (*stats).draw();
    }
    else{
        // the actions quit performs
        LCD.Clear();
        LCD.WriteLine("Game Terminated.");
        Sleep(2.0);
        LCD.Clear();
        LCD.WriteLine("Bye");
        Sleep(1.0);
    }
}

// prints the credits at the beginning of the game 
void printCredits(){
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Created by",0,0);
    LCD.WriteAt("Kishore Prakash Sailaja", 0, 20);
    LCD.WriteAt("and", 0, 40);
    LCD.WriteAt("Arad Peregoudov", 0, 60);
    Sleep(3.0);
}

/*
 * Entry point to the application
 */
int main() {

    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();


    // display credits
    //printCredits();


    Button playBTN("PLAY",90,120,4*12 + 2, 19);
    Button rulesBTN("RULES",180,120,5*12 + 2, 19);
    Button statsBTN("STATS",90,150,5*12 + 2, 19);
    Button quitBTN("QUIT",180,150,4*12 + 2, 19);
    Button backBTN("<-",280,0,2*12 + 2, 19);
    Button playBackBTN("<-",100,120,2*12 + 2, 19);
    Button playRetryBTN("RETRY",180,120,5*12 + 2, 19);

    Main_Menu myMenu(&playBTN, &rulesBTN, &statsBTN, &quitBTN);
    Rules myRules(&backBTN);
    Play myPlay(&playBackBTN,&playRetryBTN);
    Stats myStats(&backBTN);


    // declear user's click location.
    float x;
    float y;

    // draw the main menu
    myMenu.draw();

    /*
     * 1 = Main Menu Screen, 2 = Play Screen, 3 = Rules Screen, 4 = Stats Screen, and 5 = Quit Screen
     */
    int onScreen = 1;

    // Loop till not on quit screen.
    while(onScreen != 5){

        // Setting arbitrary values outside of LCD's display to start 
        x = 1000;
        y = 1000;

        // waiting to click a button on given screen
        while(!touchedAnyButton(x,y, &onScreen, &myMenu, &myRules, &myPlay, &myStats)){
             while(!LCD.Touch(&x,&y)){
            // wait until screen is not being touched
            }
            while(LCD.Touch(&x,&y)){
            // wait until person lets go
            }
        }

        // Screen on "onScreen" will be run
        initiate(&onScreen, &myMenu, &myRules, &myPlay, &myStats);
    }

    // Updating score file to reset scores to 0
    writeScoreToFile(0);

    // Stuff below is un-needed. Ignore.
    // while (1) {
    //     LCD.Update();
    //     // Never end
    // }

    return 0;
}
