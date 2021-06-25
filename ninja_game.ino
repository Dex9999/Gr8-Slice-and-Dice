#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//Handling Different Sections
enum gameMode
{
  gameMenu,
  diffiMenu,
  game,
  pause,
  gameOver,
  tutorial,
  credits,
  settings,
  count,
  secrets,
  help
};
gameMode state = gameMenu;
int score = 0;
int player_direction = 0;
int obstacle_direction = 0;
int boll_direction = 0;
int shur_direction = 0;
int difficulty = 0;
int ball_size = 6;
int ball_x = 0;
int ball_y = 0;
int ball_vx = 3;
int ball_vy = 2;
int ballStart = 1;
int shur_x = 0;
int shur_y = 0;
int shur_vx = 3;
int shur_vy = 2;
int shurStart = 1;
int boll_x = 0;
int boll_y = 0;
int bollStart = 1;
int sameStart = 1;
int countdown = 1;
int names = 0;
int sett = 0;
int sec = 0;
int secret = 5;
int left = 0;
int dBall = 0;
int sOne = 2;
int sTwo = 2;
int sThree = 2;
int sFour = 2;
int win = 0;
int winE = 0;
int winM = 0;
int winH = 0;
int winEH = 0;
int easy_vy = 0;
int hard_vy = 0;
int special = 0;
int noS = 0;
// 2,3,4 is a black ball you slice, 1 is a gray ball you absorb
int ball_type = 2;
char mode[] = "Easy";
unsigned long referenceTime;

//default menu declaration
const char strP[] PROGMEM = "Play!";
const char strT[] PROGMEM = "Tutorial";
const char strC[] PROGMEM = "Credits";
const char strS[] PROGMEM = "Settings";
const char strSe[] PROGMEM = "Secrets";
const char* const mainMenu[5] PROGMEM = {
  strP,
  strT,
  strC,
  strS,
  strSe
};

//difficulty menu declaration
const char strE[] PROGMEM = "Easy";
const char strN[] PROGMEM = "Medium";
const char strH[] PROGMEM = "Hard";
const char strEH[] PROGMEM = "Even Harder";
const char* const difficultyMenu[4] PROGMEM = {
  strE,
  strN,
  strH,
  strEH,
};

//difficulty menu declaration
const char strFE[] PROGMEM = "Hah Pleb";
const char strFN[] PROGMEM = "Ur Still Bad";
const char strFH[] PROGMEM = "Not Terrible";
const char strFEH[] PROGMEM = "HOW THE-";
const char* const funnyMenu[4] PROGMEM = {
  strFE,
  strFN,
  strFH,
  strFEH,
};

const PROGMEM byte smile[]{
8,5,
B00000,
B01010,
B00000,
B10001,
B01110
};
//ALL of the Ninja Sprites, holy
const PROGMEM byte defaultN[]{
//The size of the bitmap
8,14,
//The bitmap is 14x8
B00100000,
B10010000,
B01010000,
B00110000,
B00011100,
B00111110,
B00101010,
B00111110,
B00011100,
B00111110,
B01011101,
B01011101,
B00010100,
B00010100
};
const PROGMEM byte defaultSword[]{
//The size of the bitmap
8,14,
//The bitmap is 14x8
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000001,
B00000010,
B00000000,
B00100000,
B00100000,
B11100000,
B11000000
};
const PROGMEM byte swordE[]{
8,14,
//The bitmap is 14x8
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000011,
B00000110,
B00001100,
B00011100,
B00011000,
B01010000,
B00000000,
B00000000
};
const PROGMEM byte bandana[]{
//The size of the bitmap
8,14,
//The bitmap is 14x8
B00100000,
B10010000,
B01010000,
B00110000,
B00000000,
B00111110,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000
};
const PROGMEM byte holdA[]{
//The size of the bitmap
8,14,
//The bitmap is 14x8
B00100000,
B10010000,
B01010000,
B00110000,
B00011100,
B00111110,
B00101010,
B00111110,
B00011100,
B00111110,
B00011100,
B00011100,
B00010100,
B00010100
};
const PROGMEM byte sword[]{
//The size of the bitmap
8,7,
//The bitmap is 7x7
B0000011,
B0000111,
B0001110,
B1011100,
B0111000,
B0110000,
B1001000
};
const PROGMEM byte hilt[]{
//The size of the bitmap
8,7,
//The bitmap is 8x7
B0000000,
B0000000,
B0000000,
B1000000,
B0100000,
B0110000,
B1001000
};
const PROGMEM byte sideSword[]{
//The size of the bitmap
8,8,
B1,
B1,
B1,
B1,
B1,
B1,
B1,
B1
};
const PROGMEM byte sideHilt[]{
//The size of the bitmap
8,8,
B0,
B0,
B0,
B0,
B0,
B0,
B1,
B1
};
//Heart Sprite
const PROGMEM byte livesH[]{
//The size of the bitmap
8,7,
//The bitmap is 3x3
B1010000,
B1110000,
B0100000,
B0000000,
B0000000,
B0000000,
B0000000
};
const PROGMEM byte No[]{
//The size of the bitmap
8,14,
//The bitmap is 14x8
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B00000000,
B01000001,
B00100010,
B00010100,
B00001000,
B00010100,
B00100010,
B01000001
};
const PROGMEM byte shuriken[]{
6,6,
B010000,
B011100,
B111000,
B001000,
B000000,
B000000
};
void setup() {
  // put your setup code here, to run once:
  gb.begin();
  initPrograme();
  gb.pickRandomSeed();
  gb.battery.show = false;
  gb.display.fontSize = 1;
  obstacle_direction = random(1, 5);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(gb.update()){
    gb.battery.show = false;
    if(gb.buttons.pressed(BTN_A))
      { 
        if(state == gameOver){
        reset();
        roset();
        score = 0;
        win = 0;
        if(countdown == 1){
        referenceTime = gb.frameCount;
        state = count;}
        else{state = game;}
        }
        else if(state == credits){state = gameMenu;}
        if(state == pause){state = game;}
      }
    if(gb.buttons.pressed(BTN_B))
      {
        if(state == pause){state = diffiMenu;}
        else if(state == game){state = pause;}
        else if(state == count){state = diffiMenu;}
        else if(state == secrets){state = help;}
        else if(state == help){state = secrets;}
        else{
        reset();
        roset();
        score = 0;
        if(state == gameOver){win = 0; state = diffiMenu;}
        else{state = gameMenu;}
        }
      }
    if(gb.buttons.pressed(BTN_C))
      {
        if(state == pause){state = gameMenu;}
        else if(state == game){state = pause;}
        else{
        reset();
        roset();
        score = 0;
        if(state == gameOver){win = 0; state = gameMenu;}
        else{state = gameMenu;}
        }
      }
        switch(state)
        {
        case gameMenu :
        reset();
        roset();
        score = 0;
        easy_vy = 0;
        if(winE == 1)if(sOne == 1){}else{sOne = 0;}
        if(winM == 1)if(sTwo == 1){}else{sTwo = 0;}
        if(winH == 1)if(sThree == 1){}else{sThree = 0;}
        if(winEH == 1)if(sFour == 1){}else{sFour = 0;}
        switch(gb.menu(mainMenu, secret)){
          case 0: 
          state = diffiMenu;
            break;
          case 1: 
          referenceTime = gb.frameCount;
          state = tutorial;
            break;
          case 2: 
          state = credits; 
            break;
          case 3: 
          state = settings; 
            break;
          case 4: 
          state = secrets;
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Slice & Dice"));
          }
          default:
            state = gameMenu;
        }
        break;

        case diffiMenu :
        reset();
        roset();
        score = 0;
        easy_vy = 0;
        hard_vy = 0;
        if(sameStart == 1){left = 1;}
        if(sThree == 1){dBall = 1;}
        if(names == 0){
        switch(gb.menu(difficultyMenu, 4)){
          case 0: 
          difficulty = 0;
          ball_vx = 1;
          ball_vy = 1;
          easy_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 1: 
          difficulty = 1;
          ball_vx = 2;
          ball_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 2: 
          difficulty = 2;
          ball_vx = 3;
          ball_vy = 2;
          hard_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 3: 
          difficulty = 3;
          ball_vx = 4.5;
          ball_vy = 2;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Slice & Dice"));
          }
          default:
            state = diffiMenu;
        }
        }
        else{
          switch(gb.menu(funnyMenu, 4)){
          case 0: 
          difficulty = 0;
          ball_vx = 1;
          ball_vy = 1;
          easy_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 1: 
          difficulty = 1;
          ball_vx = 2;
          ball_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 2: 
          difficulty = 2;
          ball_vx = 3;
          ball_vy = 2;
          hard_vy = 1;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          case 3: 
          difficulty = 3;
          ball_vx = 4.5;
          ball_vy = 2;
          if(countdown == 1){
          referenceTime = gb.frameCount;
          state = count;}
          else{state = game;}
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Slice & Dice"));
          }
          default:
            state = diffiMenu;
        }
          }
        
        break;
        
        case game :
        gb.display.fontSize = 1;
        if(sFour == 1){
        shurShoot();
        shurMove();
        shurRange();
        }
        
        //make easy and hard verticle speed slightly slower
        if(easy_vy == 1){ball_vy = 1; easy_vy = 2;}
        else if(easy_vy == 2){ball_vy = 0; easy_vy = 1;}

        if(hard_vy == 1){ball_vy = 2; hard_vy = 2;}
        else if(hard_vy == 2){ball_vy = 0; hard_vy = 1;}

        //show which color the ball is based on if you can absorb it or not
        if(ball_type == 1){gb.display.setColor(GRAY);}
        else{gb.display.setColor(BLACK);}

        //Checks if the player is pressing a button and changes 
        //the direction variable accordingly
        playerInput();
        
        //sends the ball in the direction starting from the border
        //random direction indicated by the obstacle_direction variable
        obstacleDirection();

        //extra ball
        if(dBall == 1){bollDirection();}
        
        //checks if the ball has reached the ninja and stops the game if so
        checkMiddle();
        
        //detect if the ball is in the sword slicing range and if the player slices
        //also shuriken if enabled
        detectinRange();
        
        //print where the ball is
        gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);
        //print where the second ball is
        if(dBall == 1){
        gb.display.fillRect(boll_x, boll_y, ball_size, ball_size);}
        
        gb.display.setColor(BLACK);
        if(sFour == 1){
        //print where the shuriken is
        if(shur_y == LCDHEIGHT/2 && shur_x == LCDWIDTH/2-4){}
        else{gb.display.drawBitmap(shur_x, shur_y,shuriken,NOROT,NOFLIP);}
        }
        
        //change where it prints so the numbers don't go off the screen
        formatScore();
        //print the score
        if(score < 0){score = 0;}
        gb.display.print(score);
        
        //prints the character in the direction the player is pressing
        chrSprite();
        break;

        case pause :
        pauseScreen();
        break;
        
        case gameOver :
        gameoverScreen();
        break;

        case count :
        countDown();
        break;

        case tutorial :
        doTutorial();
        break;
        
        case credits :
        if(gb.buttons.pressed(BTN_RIGHT)){
        if(special == 0){special = 2;}
        else if(special == 4){special = 1;}
        else{special = 0;}
        }
        if(gb.buttons.pressed(BTN_LEFT)){
        if(special == 2){special = 3;}
        else{special = 0;}
        }
        if(gb.buttons.pressed(BTN_UP)){
        if(special == 3){special = 4;}
        else{special = 0;}
        }
        gb.display.setColor(BLACK);
        gb.display.cursorX = 2;
        gb.display.cursorY = 2;
        gb.display.print("Programming: Declan");
        gb.display.cursorX = 2;
        gb.display.cursorY = 10;
        gb.display.print("Art & SFX: Evan");
        gb.display.cursorX = 2;
        gb.display.cursorY = 25;
        gb.display.print("Special Thanks To");
        gb.display.cursorX = 2;
        gb.display.cursorY = 33;
        gb.display.print("Trevor, Taysen, Josh");
        gb.display.cursorX = 2;
        gb.display.cursorY = 41;
        gb.display.print("For Playtesting");
        if(special == 1){
        sTwo = 1;
        winM = 1;
        gb.display.drawBitmap(70,10,defaultN,NOROT,NOFLIP);
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(70,10,No,NOROT,NOFLIP);
        }
        break;
        
        case settings :
        if(gb.buttons.pressed(BTN_RIGHT)){
        if(noS == 3){noS = 4;}
        else{noS = 0;}
        }
        if(gb.buttons.pressed(BTN_LEFT)){
        if(noS == 0){noS = 2;}
        else if(noS == 2){noS = 3;}
        else if(noS == 4){noS = 1;}
        else{noS = 0;}
        }
        if(gb.buttons.pressed(BTN_A)){
        if(sett == 0){if(countdown == 1){countdown = 0;}else{countdown = 1;}}
        if(sett == 1){if(sameStart == 0){sameStart = 1;}else{sameStart = 0;}}
        if(sett == 2){if(names == 0){names = 1;}else{names = 0;}}
        }
        if(gb.buttons.pressed(BTN_DOWN)){
        if(sett < 2){sett = sett + 1;}
        else{sett = 0;}
        }
        if(gb.buttons.pressed(BTN_UP)){
        if(sett > 0){sett = sett - 1;}
        else{sett = 2;}
        }
        gb.display.setColor(BLACK);
        if(sett == 0){gb.display.fillRect(51.5, 1, 7, 7);}
        if(sett == 1){gb.display.fillRect(51.5, 9, 7, 7);}
        if(sett == 2){gb.display.fillRect(51.5, 17, 7, 7);}
        gb.display.cursorX = 2;
        gb.display.cursorY = 2;
        gb.display.print("Countdown");
        gb.display.cursorX = 2;
        gb.display.cursorY = 10;
        gb.display.print("Same Start");
        gb.display.cursorX = 2;
        gb.display.cursorY = 18;
        gb.display.print("Funny Names");
        gb.display.fillRect(52.5, 2, 5, 5);
        gb.display.fillRect(52.5, 10, 5, 5);
        gb.display.fillRect(52.5, 18, 5, 5);
        gb.display.setColor(INVERT);
        gb.display.fillRect(53.5, 3, 3, 3);
        gb.display.fillRect(53.5, 11, 3, 3);
        gb.display.fillRect(53.5, 19, 3, 3);
        gb.display.setColor(INVERT);
        if(countdown == 1){gb.display.fillRect(54.5, 4, 1, 1);}
        if(names == 1){gb.display.fillRect(54.5, 20, 1, 1);}
        if(sameStart == 1){gb.display.fillRect(54.5, 12, 1, 1);}

        if(noS == 1){
        sOne = 1;
        winE = 1;
        gb.display.drawBitmap(66,8,sword,NOROT,NOFLIP);
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(66,1,No,NOROT,NOFLIP);
        }
        break;
        
        case secrets :
        if(sOne == 1){noS = 1;}else{noS = 0;}
        if(sTwo == 1){special = 1;}else{special = 0;} 
        if(gb.buttons.pressed(BTN_A)){
        if(sec == 0){if(sOne == 1){sOne = 0;}else if(sOne == 2){} else {sOne = 1;}}
        if(sec == 1){if(sTwo == 0){sTwo = 1;}else if(sOne == 2){} else {sTwo = 0;}}
        if(sec == 2){if(sThree == 0){sThree = 1;}else if(sOne == 2){} else {sThree = 0;}}
        if(sec == 3){if(sFour == 0){sFour = 1;}else if(sOne == 2){} else {sFour = 0;}}
        }
        if(gb.buttons.pressed(BTN_RIGHT)){
        if(sec < 3){sec = sec + 1;}
        else{sec = 0;}
        }
        if(gb.buttons.pressed(BTN_LEFT)){
        if(sec > 0){sec = sec - 1;}
        else{sec = 3;}
        }
        if(gb.buttons.pressed(BTN_DOWN)){
        if(sec == 0){sec = 2;}
        else if(sec == 1){sec = 3;}
        }
        if(gb.buttons.pressed(BTN_UP)){
        if(sec == 2){sec = 0;}
        else if(sec == 3){sec = 1;}
        }
        gb.display.setColor(BLACK);
        if(sec == 0){gb.display.fillRect(18, 16, 7, 7);}
        if(sec == 1){gb.display.fillRect(58.5, 16, 7, 7);}
        if(sec == 2){gb.display.fillRect(18, 39, 7, 7);}
        if(sec == 3){gb.display.fillRect(58.5, 39, 7, 7);}
        
        gb.display.cursorX = 10;
        gb.display.cursorY = 2;
        gb.display.print("Secret");
        gb.display.cursorX = 16;
        gb.display.cursorY = 9;
        gb.display.print("One");
        gb.display.cursorX = 50;
        gb.display.cursorY = 2;
        gb.display.print("Secret");
        gb.display.cursorX = 56;
        gb.display.cursorY = 9;
        gb.display.print("Two");
        gb.display.cursorX = 10;
        gb.display.cursorY = 25;
        gb.display.print("Secret");
        gb.display.cursorX = 12;
        gb.display.cursorY = 32;
        gb.display.print("Three");
        gb.display.cursorX = 50;
        gb.display.cursorY = 25;
        gb.display.print("Secret");
        gb.display.cursorX = 54;
        gb.display.cursorY = 32;
        gb.display.print("Four");
        gb.display.fillRect(19, 17, 5, 5);
        gb.display.fillRect(59.5, 17, 5, 5);
        gb.display.fillRect(19, 40, 5, 5);
        gb.display.fillRect(59.5, 40, 5, 5);
        gb.display.cursorX = LCDWIDTH/2-2;
        gb.display.cursorY = LCDHEIGHT/2-17;
        gb.display.print("B");
        gb.display.cursorX = LCDWIDTH/2-5;
        gb.display.cursorY = LCDHEIGHT/2-11;
        gb.display.print("for");
        gb.display.cursorX = LCDWIDTH/2-7;
        gb.display.cursorY = LCDHEIGHT/2-5;
        gb.display.print("help");
        
        gb.display.setColor(INVERT);
        gb.display.fillRect(20, 18, 3, 3);
        gb.display.fillRect(60.5, 18, 3, 3);
        gb.display.fillRect(20, 41, 3, 3);
        gb.display.fillRect(60.5, 41, 3, 3);
        gb.display.setColor(INVERT);
        if(sOne == 1){gb.display.fillRect(21, 19, 1, 1);}
        if(sTwo == 1){gb.display.fillRect(61.5, 19, 1, 1);}
        if(sThree == 1){gb.display.fillRect(21, 42, 1, 1);}
        if(sFour == 1){gb.display.fillRect(61.5, 42, 1, 1);}

        if(sOne == 2){
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(17,9,No,NOROT,NOFLIP);}
        if(sTwo == 2){
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(57.5,9,No,NOROT,NOFLIP);}
        if(sThree == 2){
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(17,32,No,NOROT,NOFLIP);}
        if(sFour == 2){
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(57.5,32,No,NOROT,NOFLIP);}
        break;

        case help:
        gb.display.cursorX = 1;
        gb.display.print("Complete difficulties");
        gb.display.cursorX = 1;
        gb.display.cursorY = 9;
        gb.display.print("by getting to 32,700");
        gb.display.cursorX = 1;
        gb.display.cursorY = 18;
        gb.display.print("points and unlock");
        gb.display.cursorX = 1;
        gb.display.cursorY = 27;
        gb.display.print("new secrets!");
        gb.display.cursorX = 1;
        gb.display.cursorY = 36;
        gb.display.print("B to go back");
        break;
        }
      }
}

void playerInput(){
        //I make it repeat so it checks over and over rather than just showing the sprite once
        //I originally had the sprite code in here but realized it would be simpler to 
        //use a variable for each direction so now each of those directions activate 
        //the specific sprites so it can flick back and forth
        //0 is neutral 1 is up 2 is right 3 is down and 4 is left
        if(gb.buttons.repeat(BTN_UP, 1)){player_direction = 1;}
        else if(gb.buttons.repeat(BTN_RIGHT, 1)){player_direction = 2;}
        else if(gb.buttons.repeat(BTN_DOWN, 1)){player_direction = 3;}
        else if(gb.buttons.repeat(BTN_LEFT, 1)){player_direction = 4;}
        //this else makes it so if no buttons are pressed it goes back to neutral
        else{player_direction = 0;}
    }

void shurShoot(){
if(gb.buttons.pressed(BTN_A)){
    shur_y = LCDHEIGHT/2;
    shur_x = LCDWIDTH/2-4;
    if(player_direction == 0){shur_y = LCDHEIGHT/2;
    shur_x = LCDWIDTH/2-4;}
    else if(player_direction == 1){
      shur_direction = 1;
          if(shurStart == 1){
            shur_y = LCDHEIGHT/2;
            shurStart = 0;
            }
        }
    else if(player_direction == 2){
      shur_direction = 2;
          if(shurStart == 1){
            shur_x = LCDWIDTH/2-4;
            shurStart = 0;
            }
      }
    else if(player_direction == 3){
      shur_direction = 3;
          if(shurStart == 1){
            shur_y = LCDHEIGHT/2-5;
            shurStart = 0;
            }
          }
    else if(player_direction == 4){
      shur_direction = 4;
          if(shurStart == 1){
            shur_x = LCDWIDTH/2;
            shurStart = 0;
            }
        }
  else{}
  }
}

void shurMove(){
  if(shur_direction == 0){
    shur_y = LCDHEIGHT/2;
    shur_x = LCDWIDTH/2-4;
    shurStart = 1;}
  if(shur_direction == 1){
    shur_y = shur_y - shur_vy;
    shur_x = LCDWIDTH/2-5;
    if(0 > shur_y){shur_direction = 0;}
    }
  if(shur_direction == 2){
    shur_x = shur_x + shur_vx;
    shur_y = LCDHEIGHT/2;
    if(LCDWIDTH < shur_x){shur_direction = 0;}
    }
  if(shur_direction == 3){
    shur_y = shur_y + shur_vy;
    shur_x = LCDWIDTH/2-4;
    if(LCDHEIGHT < shur_y){shur_direction = 0;}
    }
  if(shur_direction == 4){
    shur_x = shur_x - shur_vx;
    shur_y = LCDHEIGHT/2;
    if(0 > shur_x){shur_direction = 0;}
    }
  }

void chrSprite()
{
  if (player_direction == 0){
  if(special == 1){if(noS == 1){}
  else{gb.display.setColor(GRAY);  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,swordE,NOROT,NOFLIP);}}
  else{
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,defaultN,NOROT,NOFLIP);
  }
  if(noS == 1){}
  else{
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,defaultSword,NOROT,NOFLIP);
  }
  }
  if (player_direction == 1){
  if(special == 1){}
  else{
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  }
  if(noS == 1){}
  else{
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-7-1,LCDHEIGHT/2-5-1,sideSword,NOROT,NOFLIP);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-1,LCDHEIGHT/2+1-1);  
  }
  }
  if (player_direction == 2){
  if(special == 1){}
  else{
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.fillRect(LCDWIDTH/2-4-1+7-6,LCDHEIGHT/2-7-1+10,1,2);
  }
  if(noS == 1){}
  else{
  gb.display.setColor(GRAY);
  gb.display.fillRect(LCDWIDTH/2-4-1+1-6+12,LCDHEIGHT/2+2-1,7,1);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-4-1+7-7+8,LCDHEIGHT/2-7-1+9); 
  }
  }
  if (player_direction == 3){
  if(special == 1){}
  else{
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  }
  if(noS == 1){}
  else{
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-7-1,LCDHEIGHT/2+2-1,sideSword,NOROT,NOFLIP);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-1,LCDHEIGHT/2+3-1);  
  }
  }
  if (player_direction == 4){
  if(special == 1){}
  else{
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.fillRect(LCDWIDTH/2-4-1+7,LCDHEIGHT/2-7-1+10,1,2);
  }
  if(noS == 1){}
  else{
  gb.display.setColor(GRAY);
  gb.display.fillRect(LCDWIDTH/2-4-1+1-6,LCDHEIGHT/2+2-1,7,1);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-4-1+7-7,LCDHEIGHT/2-7-1+9);  
  }
  }
}

void obstacleDirection() {
        if(sameStart == 1){
        if(left == 1){obstacle_direction = 1; left = 0;}}
        //come from the left
        if(obstacle_direction == 1){
          ball_x = min(ball_x + ball_vx, LCDWIDTH/2-ball_size/2);
          ball_y = LCDHEIGHT/2-ball_size/2;
          if(ballStart == 1){
            ball_x = 0;
            ballStart = 0;
            }
          }
        //come from the top
        if(obstacle_direction == 2){
          ball_y = min(ball_y + ball_vy, LCDHEIGHT/2-ball_size/2);
          ball_x = LCDWIDTH/2-ball_size/2;
          if(ballStart == 1){
            ball_y = 0;
            ballStart = 0;
            }
          }
        //come from the right
        if(obstacle_direction == 3){
          ball_x = max(ball_x - ball_vx, LCDWIDTH/2-ball_size/2);
          ball_y = LCDHEIGHT/2-ball_size/2;
          if(ballStart == 1){
            ball_x = LCDWIDTH-ball_size;
            ballStart = 0;
            }
          }
        //come from the bottom
        if(obstacle_direction == 4){
          ball_y = max(ball_y - ball_vy, LCDHEIGHT/2-ball_size/2);
          ball_x = LCDWIDTH/2-ball_size/2;
          if(ballStart == 1){
            ball_y = LCDHEIGHT-ball_size;
            ballStart = 0;
            }
          }
  }

void bollDirection(){
        //come from the left
        if(boll_direction == 1){
          boll_x = min(boll_x + ball_vx, LCDWIDTH/2-ball_size/2);
          boll_y = LCDHEIGHT/2-ball_size/2;
          if(bollStart == 1){
            boll_x = 0;
            bollStart = 0;
            }
          }
        //come from the top
        if(boll_direction == 2){
          boll_y = min(boll_y + ball_vy, LCDHEIGHT/2-ball_size/2);
          boll_x = LCDWIDTH/2-ball_size/2;
          if(bollStart == 1){
            boll_y = 0;
            bollStart = 0;
            }
          }
        //come from the right
        if(boll_direction == 3){
          boll_x = max(boll_x - ball_vx, LCDWIDTH/2-ball_size/2);
          boll_y = LCDHEIGHT/2-ball_size/2;
          if(bollStart == 1){
            boll_x = LCDWIDTH-ball_size;
            bollStart = 0;
            }
          }
        //come from the bottom
        if(boll_direction == 4){
          boll_y = max(boll_y - ball_vy, LCDHEIGHT/2-ball_size/2);
          boll_x = LCDWIDTH/2-ball_size/2;
          if(bollStart == 1){
            boll_y = LCDHEIGHT-ball_size;
            bollStart = 0;
            }
          }
  }

void detectinRange(){
        //up detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-9, 7, 9)){
          if(player_direction == 1){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} reset();}
          }
        //right detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4-1+7+1,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 2){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} reset();}
          }
        //down detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-9+14+1-3, 7, 9)){
          if(player_direction == 3){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} reset();}
          }
        //left detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4-1-7,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 4){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} reset();}
          }
        if(dBall == 1){
        //extra ball up detection
        if(gb.collideRectRect(boll_x, boll_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-9, 7, 9)){
          if(player_direction == 1){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} roset();}
          }
        //extra ball right detection
        if(gb.collideRectRect(boll_x, boll_y, ball_size, ball_size, LCDWIDTH/2-4-1+7+1,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 2){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} roset();}
          }
        //extra ball down detection
        if(gb.collideRectRect(boll_x, boll_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-9+14+1-3, 7, 9)){
          if(player_direction == 3){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} roset();}
          }
        //extra ball left detection
        if(gb.collideRectRect(boll_x, boll_y, ball_size, ball_size, LCDWIDTH/2-4-1-7,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 4){if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick();} roset();}
          }  
        }
  }

void shurRange(){
        if(shur_direction == 0){}
        else{
        //up detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, shur_x,shur_y, 4, 4)){
          if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick(); shur_direction = 0;} reset();
          }
        //right detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, shur_x,shur_y, 4, 4)){
          if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick(); shur_direction = 0;} reset();
          }
        //down detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, shur_x,shur_y, 4, 4)){
          if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick(); shur_direction = 0;} reset();
          }
        //left detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, shur_x,shur_y, 4, 4)){
          if(ball_type == 1){score = score - 200; gb.sound.playCancel();} else {if(dBall == 1){score = score + 200;}else{score = score + 100;} gb.sound.playTick(); shur_direction = 0;} reset();
          }
        }
  }

void formatScore(){
    if(score == 0){gb.display.cursorX = LCDWIDTH-4;}
    else if(score < 0){gb.display.cursorX = LCDWIDTH-4;}
    else if(score < 1000){gb.display.cursorX = LCDWIDTH-12;}
    else if(score < 10000){gb.display.cursorX = LCDWIDTH-16;}
    else if(score < 32700){gb.display.cursorX = LCDWIDTH-20;}
    else{gb.display.cursorX = LCDWIDTH-20; win = 1; state = gameOver;}
    }

void countDown(){
  gb.display.fontSize = 3;
  unsigned long t = gb.frameCount - referenceTime;
  if(sameStart == 1){left = 1;}
  if (t < 15) {
  gb.display.cursorX = LCDWIDTH/2-4;
  gb.display.cursorY = LCDHEIGHT/2-8;
  gb.display.print("3");
  }
  else if (t < 30) {
  gb.display.cursorX = LCDWIDTH/2-4;
  gb.display.cursorY = LCDHEIGHT/2-8;
  gb.display.print("2");
  }
  else if (t < 45) {
  gb.display.cursorX = LCDWIDTH/2-4;
  gb.display.cursorY = LCDHEIGHT/2-8;
  gb.display.print("1");
  }
  else{
    state = game;
  }
 }
    
void checkMiddle(){
        if(obstacle_direction == 1|| obstacle_direction == 3){
        if(ball_x == LCDWIDTH/2-ball_size/2){
        if(ball_type == 1){if(player_direction == 0){gb.sound.playOK(); if(score == 32600){score = score + 100;}else{if(dBall == 1){score = score + 400;}else{score = score + 200;}}} reset();}
        else if(ball_type == 2){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 3){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 4){gb.sound.playCancel(); state = gameOver;}
          }
        }
        else{
        if(ball_y == LCDHEIGHT/2-ball_size/2){
        //reset();
        if(ball_type == 1){if(player_direction == 0){gb.sound.playOK(); if(score == 32600){score = score + 100;}else{if(dBall == 1){score = score + 400;}else{score = score + 200;}}} reset();}
        else if(ball_type == 2){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 3){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 4){gb.sound.playCancel(); state = gameOver;}
          }
         }
        if(dBall == 1){
        if(boll_direction == 1|| boll_direction == 3){
        if(boll_x == LCDWIDTH/2-ball_size/2){
        if(ball_type == 1){if(player_direction == 0){gb.sound.playOK(); if(score == 32600){score = score + 100;}else{if(dBall == 1){score = score + 400;}else{score = score + 200;}}} roset();}
        else if(ball_type == 2){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 3){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 4){gb.sound.playCancel(); state = gameOver;}
          }
        }
        else{
        if(boll_y == LCDHEIGHT/2-ball_size/2){
        //reset();
        if(ball_type == 1){if(player_direction == 0){gb.sound.playOK(); if(score == 32600){score = score + 100;}else{if(dBall == 1){score = score + 400;}else{score = score + 200;}}} roset();}
        else if(ball_type == 2){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 3){gb.sound.playCancel(); state = gameOver;}
        else if(ball_type == 4){gb.sound.playCancel(); state = gameOver;}
           }
          }
         }
        }
void pauseScreen(){
        gb.display.cursorY = 10;
        gb.display.cursorX = 2;
        gb.display.print("PAUSED");
        gb.display.cursorY = 20;
        gb.display.cursorX = 2;
        gb.display.print("Mode: ");
        if(difficulty == 0){gb.display.print("Easy");}
        if(difficulty == 1){gb.display.print("Medium");}
        if(difficulty == 2){gb.display.print("Hard");}
        if(difficulty == 3){gb.display.cursorY = 16.5; gb.display.print("Even"); gb.display.cursorX = 26; gb.display.cursorY = 23.5; gb.display.print("Harder");}
        gb.display.cursorY = 30;
        gb.display.cursorX = 2;
        gb.display.print("Score: ");
        gb.display.print(score);
        gb.display.fillRect(54.5, 12, 6, 25);
        gb.display.fillRect(67, 12, 6, 25);  
  }
void gameoverScreen(){
        if(win == 1){
        secret = 5;
        gb.display.cursorX = LCDWIDTH/2-17.5; 
        gb.display.cursorY = 10;
        gb.display.print("YOU WIN!");
        gb.display.drawBitmap(LCDWIDTH/2-5.5,40,smile,NOROT,NOFLIP);
        if(difficulty == 0){winE = 1;}
        else if(difficulty == 1){winM = 1;}
        else if(difficulty == 2){winH = 1;}
        else if(difficulty == 3){winEH = 1;}
          }
        else{
        gb.display.cursorX = LCDWIDTH/2-17.5; 
        gb.display.cursorY = 10;
        gb.display.print("GAME OVER");}
        if(difficulty == 3){gb.display.cursorX = LCDWIDTH/2-29.5;}
        else{gb.display.cursorX = LCDWIDTH/2-17.5;}
        gb.display.cursorY = 20;
        gb.display.print("Mode: ");
        if(difficulty == 0){gb.display.print("Easy");}
        if(difficulty == 1){gb.display.print("Medium");}
        if(difficulty == 2){gb.display.print("Hard");}
        if(difficulty == 3){gb.display.cursorX = LCDWIDTH/2-8; gb.display.print("Even Harder");}
        gb.display.cursorX = LCDWIDTH/2-17.5;
        gb.display.cursorY = 30;
        gb.display.print("Score: ");
        gb.display.print(score);  
        if(sOne == 1){
        gb.display.setColor(BLACK);
        gb.display.drawBitmap(7,12,sword,NOROT,NOFLIP);
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(7,5,No,NOROT,NOFLIP);
        }
        if(sTwo == 1){
        gb.display.setColor(BLACK);
        gb.display.drawBitmap(7,25,defaultN,NOROT,NOFLIP);
        gb.display.setColor(GRAY);
        gb.display.drawBitmap(7,25,No,NOROT,NOFLIP);
        }
        if(sThree == 1){
        gb.display.setColor(BLACK);
        gb.display.drawRect(69,10,4,4);
        gb.display.drawRect(74,10,4,4);}
        if(sFour == 1){
        gb.display.setColor(BLACK);
        gb.display.drawBitmap(70,30,shuriken,NOROT,NOFLIP);}
  }
void doTutorial() {
  ball_vx = 3;
  ball_vy = 2;
  if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B)) {
    referenceTime = gb.frameCount;
    state = gameMenu;
    gb.pickRandomSeed();
    return;
  }
  unsigned long t = gb.frameCount - referenceTime;
  gb.display.setColor(BLACK);
  if (t < 50) {
    gb.display.cursorX = 35;
    gb.display.cursorY = 8;
    gb.display.print(F("you"));
    chrSprite();}
  else if (t < 100) {
    gb.display.cursorX = 10;
    gb.display.cursorY = 8;
    gb.display.print(F("slice these"));
    ball_x = min(ball_x + ball_vx, LCDWIDTH/2-ball_size/2);
    ball_y = LCDHEIGHT/2-ball_size/2;
    if(ballStart == 1){
       ball_x = 0;
       ballStart = 0;} 
    if(ball_x == LCDWIDTH/2-ball_size/2){ballStart = 1;}
    if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4-1-7,LCDHEIGHT/2-7-1+4, 8, 10)){
    reset();}
    player_direction = 4;
    chrSprite();
    gb.display.setColor(BLACK);
    gb.display.fillRect(ball_x, ball_y, ball_size, ball_size); 
  }
  else if (t < 190) {
    if(gb.buttons.pressed(BTN_RIGHT)){
      if(gb.buttons.pressed(BTN_LEFT)){
      winE = 1;
      winM = 1;
      winH = 1;
      winEH = 1;
      }
     }
        
    gb.display.cursorX = 33;
    gb.display.cursorY = 8;
    gb.display.print(F("absorb these"));
    //come from the right
    ball_x = max(ball_x - ball_vx, LCDWIDTH/2-ball_size/2);
    ball_y = LCDHEIGHT/2-ball_size/2;
    if(ballStart == 1){
      ball_x = LCDWIDTH-ball_size;
      ballStart = 0;
    }
    if(ball_x < LCDWIDTH/2){ballStart = 1;}    
    player_direction = 0;
    chrSprite();
    gb.display.setColor(GRAY);
    gb.display.fillRect(ball_x, ball_y, ball_size, ball_size); 
  }
  else{
    referenceTime = gb.frameCount;
    state = gameMenu;
    gb.pickRandomSeed();
  }
}

void reset(){
  if(dBall == 1){ball_type = 2;}
  else{ball_type = random(1, 5);}
  obstacle_direction = random(1, 5);
  ballStart = 1;
  }

void roset(){
  boll_direction = random(1, 5);
  bollStart = 1;
  }

void initPrograme()
{
  goTitleScreen();
  state = gameMenu;
  gb.battery.show = false;
}

void goTitleScreen()
{
  gb.titleScreen(F("Slice & Dice"));
}
