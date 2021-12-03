#include "FEHLCD.h"
#include "FEHUtility.h"
#include <string.h>
#include <iostream>

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
        Main_Menu(Button *pl, Button *ru, Button *qu);
        int touchedButton(float touched_x, float touched_y);
        void draw();
    private:
        Button *playBTN;
        Button *rulesBTN;
        Button *quitBTN;
};

// constructs the Main Menu class
Main_Menu::Main_Menu(Button *pl, Button *ru, Button *qu){
    playBTN = pl;
    rulesBTN = ru;
    quitBTN = qu;
}

// draws the main menu with given buttons
void Main_Menu::draw(){
    LCD.Clear();
    (*playBTN).draw();
    (*rulesBTN).draw();
    (*quitBTN).draw();
}

// returns the button in Main Menu that was touched. 1 = Play, 2 = Rules, 3 = Quit, and 0 = N/A.
int Main_Menu::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*playBTN).touched(touched_x,touched_y)){
        touched = 1;
    }
    else if((*rulesBTN).touched(touched_x,touched_y)){
        touched = 2;
    }
    else if((*quitBTN).touched(touched_x,touched_y)){
        touched = 3;
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
        Play(Button *bk);
        void draw();
        int touchedButton(float touched_x, float touched_y);
    private:
        Button *back;
};

Play::Play(Button *bk){
    back = bk;
}

// Arad: Work on formatting Rules
void Play::draw(){
    LCD.Clear();
    (*back).draw();
    LCD.WriteAt("Game here.",0,0);
}

// touching the back button in Rules returns 1. Else, returns 0.
int Play::touchedButton(float touched_x, float touched_y){
    int touched = 0;
    if((*back).touched(touched_x,touched_y)){
        touched = 1;
    }
    return touched;
}


// Checks if a button was clicked on given "onScreen".
bool touchedAnyButton(int x, int y, int *onScreen, Main_Menu *menu, Rules *rules, Play *play){
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

        // "Quit" button was touched in main menu
        else if(buttonTouched == 3){
            *onScreen = 4;
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
    }

    // in Rules Screen
    else if((*onScreen) == 3){
        // Back button was touched in Rules
        if((*rules).touchedButton(x,y) == 1){
            *onScreen = 1;
            touched = true;
        }
    }

    return touched;
}

// The initiater of the given screen. 
void initiate(int *onScreen, Main_Menu *menu, Rules *rules, Play *play)
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
    else {
        // the actions quit performs
        LCD.Clear();
        LCD.WriteLine("Game Terminated.");
        Sleep(3.0);
        LCD.Clear();
        LCD.WriteLine("Bye");
        Sleep(1.0);
    }
}

/*
 * Entry point to the application
 */
int main() {

    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();


    Button playBTN("PLAY",90,120,4*12 + 2, 19);
    Button rulesBTN("RULES",170,120,5*12 + 2, 19);
    Button quitBTN("QUIT",130,150,4*12 + 2, 19);
    Button backBTN("<-",280,0,2*12 + 2, 19);

    Main_Menu myMenu(&playBTN, &rulesBTN, &quitBTN);
    Rules myRules(&backBTN);
    Play myPlay(&backBTN);

    // declear user's click location.
    float x;
    float y;

    // draw the main menu
    myMenu.draw();

    /*
     * 1 = Main Menu Screen, 2 = Play Screen, 3 = Rules Screen, 4 = Quit Screen
     */
    int onScreen = 1;

    // Loop till not on quit screen.
    while(onScreen != 4){

        // Setting arbitrary values outside of LCD's display to start 
        x = 1000;
        y = 1000;

        // waiting to click a button on given screen
        while(!touchedAnyButton(x,y, &onScreen, &myMenu, &myRules, &myPlay)){
             while(!LCD.Touch(&x,&y)){
            // wait until screen is not being touched
            }
            while(LCD.Touch(&x,&y)){
            // wait until person lets go
            }
        }

        // Screen on "onScreen" will be run
        initiate(&onScreen, &myMenu, &myRules, &myPlay);
    }


    // Stuff below is un-needed. Ignore.
    // while (1) {
    //     LCD.Update();
    //     // Never end
    // }

    return 0;
}