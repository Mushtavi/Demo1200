#include "iGraphics.h"
#include <cstdlib>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

// Dinosaur variables
int dinoX = 50, dinoY = 100, dinoWidth = 40, dinoHeight = 40;
bool isJumping = false;
int jumpSpeed = 10, gravity = -2, jumpHeight = 150, currentJumpHeight = 0;

// Obstacle variables
int obsX = SCREEN_WIDTH, obsY = 100, obsWidth = 20, obsHeight = 40;
int gameSpeed = 5;

// Score variables
int score = 0;
char scoreText[20];

// Function declarations
void jump();
void resetObstacle();
bool detectCollision();
void iTimer();  // Updated function declaration to match the correct signature


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void iDraw()
{
	iClear();
	// Background
	iSetColor(255, 255, 255);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Ground
	iSetColor(0, 255, 0);
	iFilledRectangle(0, 0, SCREEN_WIDTH, 100);

	// Dinosaur
	iSetColor(0, 0, 255);
	iFilledRectangle(dinoX, dinoY, dinoWidth, dinoHeight);

	// Obstacle
	iSetColor(255, 0, 0);
	iFilledRectangle(obsX, obsY, obsWidth, obsHeight);

	// Score
	sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);  // Use sprintf_s for safety
	iSetColor(0, 0, 0);
	iText(10, SCREEN_HEIGHT - 20, scoreText, GLUT_BITMAP_HELVETICA_18);

	// Collision Detection
	if (detectCollision()) {
		iSetColor(255, 0, 0);
		iText(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "Game Over!", GLUT_BITMAP_TIMES_ROMAN_24);
		iPauseTimer(0); // Stop the game after collision
	}
}





/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{
	
}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		
	}
	
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}


// Function to simulate jumping
void jump()
{
	if (isJumping) {
		dinoY += jumpSpeed;
		currentJumpHeight += jumpSpeed;

		if (currentJumpHeight >= jumpHeight) {
			isJumping = false;
		}
	}
	else {
		if (dinoY > 100) {
			dinoY += gravity;
		}
		else {
			currentJumpHeight = 0;
			dinoY = 100;  // Ensure it lands back on the ground
		}
	}
}

// Function to reset obstacle position and increase score
void resetObstacle()
{
	if (obsX + obsWidth < 0) {
		obsX = SCREEN_WIDTH + rand() % 200; // Random spacing
		score++; // Increment score when obstacle moves off screen
	}
}

// Function to detect collision between dinosaur and obstacle
bool detectCollision()
{
	return (dinoX < obsX + obsWidth && dinoX + dinoWidth > obsX &&
		dinoY < obsY + obsHeight && dinoY + dinoHeight > obsY);
}

// Timer function (no parameters)
void iTimer()
{
	obsX -= gameSpeed; // Move the obstacle
	resetObstacle();   // Reset obstacle position when it goes off-screen
	jump();            // Simulate jumping
}


/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{
	
	if (key == ' ') {
		if (dinoY == 100) { // Jump only if on the ground
			isJumping = true;
		}
	}
	
	
	
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

	
	if (key == GLUT_KEY_RIGHT)
	{
				
	}
	if (key == GLUT_KEY_LEFT)
	{
		
	}
	
	if (key == GLUT_KEY_HOME)
	{
		
	}
	
}


int main()
{
	
	srand((unsigned)time(NULL));
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Dinosaur Endless Runner");

	// Set the game loop timer (no parameters for iTimer)
	iSetTimer(25, iTimer);

	
	
	///updated see the documentations
	iStart();
	return 0;
}