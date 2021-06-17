#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//Handling Different Sections
enum gameMode
{
  gameMenu,
  diffiMenu,
  game,
  gameOver,
  tutorial,
  credits
};
gameMode state = gameMenu;
int score = 0;
int player_direction = 0;
int obstacle_direction = 0;
int difficulty = 0;
int ball_size = 6;
int ball_x = 0;
int ball_y = 0;
int ball_vx = 3;
int ball_vy = 2;
int ballStart = 1;
int win = 0;
// 2,3,4 is a black ball you slice, 1 is a gray ball you absorb
int ball_type = 2;
char mode[] = "Easy";
unsigned long referenceTime;

//default menu declaration
const char strP[] PROGMEM = "Play!";
const char strT[] PROGMEM = "Tutorial";
const char* const mainMenu[2] PROGMEM = {
  strP,
  strT
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
    if(gb.buttons.pressed(BTN_C))
      {
        reset();
        score = 0;
        if(state == gameMenu){goTitleScreen();}
        else{state = gameMenu;}
      }
        switch(state)
        {
        case gameMenu :
        reset();
        score = 0;
        switch(gb.menu(mainMenu, 2)){
          case 0: 
          state = diffiMenu;
            break;
          case 1: 
          referenceTime = gb.frameCount;
          state = tutorial;          
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Ninja Game"));
          }
          default:
            state = gameMenu;
        }
        break;

        case diffiMenu :
        reset();
        score = 0;
        switch(gb.menu(difficultyMenu, 4)){
          case 0: 
          difficulty = 0;
          ball_vx = 1;
          ball_vy = 1;
          state = game;
            break;
          case 1: 
          difficulty = 1;
          ball_vx = 2;
          ball_vy = 1;
          state = game;
            break;
          case 2: 
          difficulty = 2;
          ball_vx = 3;
          ball_vy = 2;
          state = game;
            break;
          case 3: 
          difficulty = 3;
          ball_vx = 4.5;
          ball_vy = 3;
          state = game;
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Ninja Game"));
          }
          default:
            state = diffiMenu;
        }
        break;
        
        case game :
        gb.display.fontSize = 1;
        if(score < 0){score = 0;}
        //Checks if the player is pressing a button and changes 
        //the direction variable accordingly
        playerInput();
        
        //show which color the ball is based on if you can absorb it or not
        if(ball_type == 1){gb.display.setColor(GRAY);}
        else{gb.display.setColor(BLACK);}
        
        //sends the ball in the direction starting from the border
        //random direction indicated by the obstacle_direction variable
        obstacleDirection();
        
        //checks if the ball has reached the ninja and stops the game if so
        checkMiddle();
        
        //detect if the ball is in the sword slicing range and if the player slices
        detectinRange();
        
        //print where the ball is
        gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);
        //prints the character in the direction the player is pressing
        //print the score
        gb.display.setColor(BLACK);
        //change where it prints so the numbers don't go off the screen
        formatScore();
        gb.display.print(score);
        chrSprite();
        break;
        
        case gameOver :
        gb.display.cursorX = LCDWIDTH/2-17.5; 
        gb.display.cursorY = 10;
        gb.display.print("GAME OVER");
        gb.display.cursorX = LCDWIDTH/2-17.5;
        gb.display.cursorY = 20;
        gb.display.print("Mode: ");
        if(difficulty == 0){gb.display.print("Easy");}
        if(difficulty == 1){gb.display.print("Medium");}
        if(difficulty == 2){gb.display.print("Hard");}
        if(difficulty == 3){gb.display.cursorX = LCDWIDTH/2-17.5-10; gb.display.print("Even Harder");}
        gb.display.cursorX = LCDWIDTH/2-17.5;
        gb.display.cursorY = 30;
        gb.display.print("Score: ");
        gb.display.print(score);
        break;

        case tutorial :
        doTutorial();
        break;
        
        case credits :
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
        else{
        if(gb.buttons.repeat(BTN_RIGHT, 1)){player_direction = 2;}
        else{
        if(gb.buttons.repeat(BTN_DOWN, 1)){player_direction = 3;}
        else{
        if(gb.buttons.repeat(BTN_LEFT, 1)){player_direction = 4;}
        else{player_direction = 0;}
        //this else makes it so if no buttons are pressed it goes back to neutral
        }}}
    }

void chrSprite()
{
  if (player_direction == 0){
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,defaultN,NOROT,NOFLIP);
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,defaultSword,NOROT,NOFLIP);
  }
  if (player_direction == 1){
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-7-1,LCDHEIGHT/2-5-1,sideSword,NOROT,NOFLIP);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-1,LCDHEIGHT/2+1-1);  
  }
  if (player_direction == 2){
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.fillRect(LCDWIDTH/2-4-1+7-6,LCDHEIGHT/2-7-1+10,1,2);
  gb.display.setColor(GRAY);
  gb.display.fillRect(LCDWIDTH/2-4-1+1-6+12,LCDHEIGHT/2+2-1,7,1);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-4-1+7-7+8,LCDHEIGHT/2-7-1+9);  }
  if (player_direction == 3){
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.setColor(GRAY);
  gb.display.drawBitmap(LCDWIDTH/2-7-1,LCDHEIGHT/2+2-1,sideSword,NOROT,NOFLIP);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-1,LCDHEIGHT/2+3-1);  
  }
  if (player_direction == 4){
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(LCDWIDTH/2-4-1,LCDHEIGHT/2-7-1,holdA,NOROT,NOFLIP);
  gb.display.fillRect(LCDWIDTH/2-4-1+7,LCDHEIGHT/2-7-1+10,1,2);
  gb.display.setColor(GRAY);
  gb.display.fillRect(LCDWIDTH/2-4-1+1-6,LCDHEIGHT/2+2-1,7,1);
  gb.display.setColor(BLACK);
  gb.display.drawPixel(LCDWIDTH/2-4-1+7-7,LCDHEIGHT/2-7-1+9);  }
}

void obstacleDirection() {
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

void detectinRange(){
        //up detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-11, 7, 7)){
          if(player_direction == 1){if(ball_type == 1){score = score - 200;} else {score = score + 100;} reset();}
          }
        //right detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4-1+7+1,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 2){if(ball_type == 1){score = score - 200;} else {score = score + 100;} reset();}
          }
        //down detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4,LCDHEIGHT/2-9+14+1, 7, 7)){
          if(player_direction == 3){if(ball_type == 1){score = score - 200;} else {score = score + 100;} reset();}
          }
        //left detection
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, LCDWIDTH/2-4-1-7,LCDHEIGHT/2-7-1+4, 8, 10)){
          if(player_direction == 4){if(ball_type == 1){score = score - 200;} else {score = score + 100;} reset();}
          }
  }
  
void formatScore(){
    if(score == 0){gb.display.cursorX = LCDWIDTH-4;}
    else if(score < 1000){gb.display.cursorX = LCDWIDTH-12;}
    else if(score < 10000){gb.display.cursorX = LCDWIDTH-16;}
    else if(score < 32700){gb.display.cursorX = LCDWIDTH-20;}
    else{win = 1; state = gameOver;}
    }
    
void checkMiddle(){
        if(obstacle_direction == 1|| obstacle_direction == 3){
        if(ball_x == LCDWIDTH/2-ball_size/2){
        if(ball_type == 1){if(player_direction == 0){score = score + 200;} reset();}
        else{state = gameOver;}
          }
        }
        else{
        if(ball_y == LCDHEIGHT/2-ball_size/2){
        //reset();
        if(ball_type == 1){if(player_direction == 0){score = score + 200;} reset();}
        else{state = gameOver;}
          }
         }
        }
void doTutorial() {
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
    score = score + 100; reset();}
    player_direction = 4;
    chrSprite();
    gb.display.setColor(BLACK);
    gb.display.fillRect(ball_x, ball_y, ball_size, ball_size); 
  }
  else if (t < 190) {
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
  ball_type = random(1, 5);
  obstacle_direction = random(1, 5);
  ballStart = 1;
  }

void initPrograme()
{
  goTitleScreen();
  state = gameMenu;
  gb.battery.show = false;
}

void goTitleScreen()
{
  gb.titleScreen(F("Ninja Game"));
}
