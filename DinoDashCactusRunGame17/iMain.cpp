#include "iGraphics.h"
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>

std::string intToString(int value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

int loadImageWithCheck(const char* filename);
void iLoadMainMenuImages();
void iDrawMainMenu();
void iMouseMainMenu(int button, int state, int mx, int my);
void loadGameImages();
void loadSpriteFrames();
void updateAnimation();
void spawnShield();
void resetShield();
void spawnCoin();
void resetCoins();
void iDraw();
void jump();
void resetObstacle();
void resetBird();
void resetGame();
void increaseGameSpeed();
void iTimer();
void iMouse(int button, int state, int mx, int my);
void iKeyboard(unsigned char key);
void iSpecialKeyboard(unsigned char key);
bool detectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void playBackgroundMusic();
void applyAsteroidGravity();
void updateAsteroidTrajectory();
void saveHighScore();
void loadHighScore();
void showScoreMenu();
void showIntroduction();
void showHelp();
void showAboutUs();
void showNextHelpImage();

int bgImage, buttonImage, dinoMenuImage, scoreMenuImage, backscoreImage;
int windowWidth = 1500, windowHeight = 600;
int buttonX = 100, buttonY = 550;
bool gameStarted = false;

const int SCREEN_WIDTH = 1500, SCREEN_HEIGHT = 600;
int dinoX = 150, dinoY = 50, dinoWidth = 100, dinoHeight = 100;
bool isJumping = false;
int jumpSpeed = 20, gravity = -7, jumpHeight = 350, currentJumpHeight = 0;
int fireX, fireY, fireWidth = 120, fireHeight = 120;
int fireSpeed = 20;
bool isFireActive = false;
int obsX = SCREEN_WIDTH + 500, obsY = 30, obsWidth = 70, obsHeight = 70;
int birdX = SCREEN_WIDTH + 800, birdY = 300, birdWidth = 70, birdHeight = 70;
int initialGameSpeed = 5;
int gameSpeed = initialGameSpeed;
int bgX = 0, bgSpeed = 5;
int score = 0, highScore = 0;
char scoreText[20], highScoreText[20];
bool isGameOver = false;
int lives = 3;
int backgroundImage, birdImage, obsImage, heartImage, fireImage, shieldImage, shield2Image, coinImage, asteroidImage;

int shieldX = -100, shieldY = 100, shieldWidth = 60, shieldHeight = 60;
bool isShieldActive = false;
bool hasShield = false;
const int shieldDuration = 15000;
int shieldStartTime = 0;

const int SHIELD_BAR_WIDTH = 100, SHIELD_BAR_HEIGHT = 20;
int shieldBarX = SCREEN_WIDTH - 150, shieldBarY = SCREEN_HEIGHT - 120;
float shieldBarWidth = SHIELD_BAR_WIDTH;

enum DinoState { IDLE, RUN, JUMP, DEAD };
DinoState dinoState = IDLE;
int currentFrame = 0;
int frameDelay = 5;
int deadFrameDelay = 15;
int frameCounter = 0;

std::vector<int> idleFrames;
std::vector<int> runFrames;
std::vector<int> jumpFrames;
std::vector<int> deadFrames;

bool isPaused = false;
int pauseDuration = 1000;
int pauseStartTime = 0;

struct Asteroid {
	int x, y;
	bool active;
};
std::vector<Asteroid> asteroids;
const int ASTEROID_WIDTH = 60, ASTEROID_HEIGHT = 60;
int asteroidSpeed = 10;
const int ASTEROID_SPAWN_INTERVAL = 50;
int asteroidSpawnCounter = 0;
const int ASTEROID_FALLING_SPACE_X = 1000;
bool isAsteroidFallingSpaceActive = false;

struct Coin {
	int x, y;
	bool active;
};
std::vector<Coin> coins;
const int COIN_WIDTH = 40, COIN_HEIGHT = 40;
const int COIN_SPAWN_INTERVAL = 100;
int coinSpawnCounter = 0;

enum GameMode { EASY, MEDIUM, HARD };
GameMode currentGameMode = EASY;
bool showGameModeOptions = false;

bool showScoreMenuScreen = false;

bool showIntroductionScreen = false;
bool showHelpScreen = false;
bool showAboutUsScreen = false;
int currentHelpImage = 0;
int introductionImage, helpImages[5], aboutUsImage;

int loadImageWithCheck(const char* filename) {
	char file[256];
	strcpy_s(file, filename);
	int image = iLoadImage(file);
	if (image == -1) {
		printf("Error: Failed to load image: %s\n", filename);
		exit(1);
	}
	return image;
}

void iLoadMainMenuImages() {
	bgImage = loadImageWithCheck("images/dino_back_2.bmp");
	buttonImage = loadImageWithCheck("images/back.bmp");
	dinoMenuImage = loadImageWithCheck("images/dino1.bmp");
	scoreMenuImage = loadImageWithCheck("images/back.bmp");
	backscoreImage = loadImageWithCheck("images/backscore.bmp");
	introductionImage = loadImageWithCheck("images/introduction.bmp");
	helpImages[0] = loadImageWithCheck("images/Help1.bmp");
	helpImages[1] = loadImageWithCheck("images/Help2.bmp");
	helpImages[2] = loadImageWithCheck("images/Help3.bmp");
	helpImages[3] = loadImageWithCheck("images/Help4.bmp");
	helpImages[4] = loadImageWithCheck("images/Help5.bmp");
	aboutUsImage = loadImageWithCheck("images/AboutUs.bmp");
}

void iDrawMainMenu() {
	iClear();
	iShowImage(0, 0, windowWidth, windowHeight, bgImage);
	iShowImage(510, -80, 600, 600, dinoMenuImage);
	iSetColor(0, 128, 0);
	iText(580, 550, "Dino Dash: Cactus Run", GLUT_BITMAP_TIMES_ROMAN_24);
	iShowImage(buttonX, buttonY, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 90, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 180, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 270, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 360, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 450, 250, 60, buttonImage);
	iShowImage(buttonX, buttonY - 540, 250, 60, buttonImage);
	iSetColor(225, 225, 225);
	iText(buttonX + 100, buttonY + 20, "Play", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 70, buttonY - 65, "Game Mode", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 90, buttonY - 155, "Score", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 70, buttonY - 245, "Introduction", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 100, buttonY - 335, "Help", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 90, buttonY - 425, "About Us", GLUT_BITMAP_HELVETICA_18);
	iText(buttonX + 100, buttonY - 515, "Exit", GLUT_BITMAP_HELVETICA_18);

	if (showGameModeOptions) {
		iShowImage(buttonX, buttonY - 180, 250, 60, buttonImage);
		iShowImage(buttonX, buttonY - 270, 250, 60, buttonImage);
		iShowImage(buttonX, buttonY - 360, 250, 60, buttonImage);
		iSetColor(225, 225, 225);
		iText(buttonX + 100, buttonY - 155, "Easy", GLUT_BITMAP_HELVETICA_18);
		iText(buttonX + 80, buttonY - 245, "Medium", GLUT_BITMAP_HELVETICA_18);
		iText(buttonX + 100, buttonY - 335, "Hard", GLUT_BITMAP_HELVETICA_18);

		switch (currentGameMode) {
		case EASY:
			iSetColor(0, 255, 0);
			iText(buttonX + 100, buttonY - 155, "Easy", GLUT_BITMAP_HELVETICA_18);
			break;
		case MEDIUM:
			iSetColor(0, 255, 0);
			iText(buttonX + 80, buttonY - 245, "Medium", GLUT_BITMAP_HELVETICA_18);
			break;
		case HARD:
			iSetColor(0, 255, 0);
			iText(buttonX + 100, buttonY - 335, "Hard", GLUT_BITMAP_HELVETICA_18);
			break;
		}
	}
}

void iMouseMainMenu(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (mx >= buttonX && mx <= buttonX + 250) {
			if (my >= buttonY && my <= buttonY + 60) {
				gameStarted = true;
				resetGame();
			}
			else if (my >= buttonY - 90 && my <= buttonY - 30) {
				showGameModeOptions = !showGameModeOptions;
			}
			else if (showGameModeOptions) {
				if (my >= buttonY - 180 && my <= buttonY - 120) {
					currentGameMode = EASY;
					showGameModeOptions = false;
				}
				else if (my >= buttonY - 270 && my <= buttonY - 210) {
					currentGameMode = MEDIUM;
					showGameModeOptions = false;
				}
				else if (my >= buttonY - 360 && my <= buttonY - 300) {
					currentGameMode = HARD;
					showGameModeOptions = false;
				}
			}
			else if (my >= buttonY - 180 && my <= buttonY - 120) {
				showScoreMenuScreen = true;
			}
			else if (my >= buttonY - 270 && my <= buttonY - 210) {
				showIntroductionScreen = true;
			}
			else if (my >= buttonY - 360 && my <= buttonY - 300) {
				showHelpScreen = true;
				currentHelpImage = 0;
			}
			else if (my >= buttonY - 450 && my <= buttonY - 390) {
				showAboutUsScreen = true;
			}
			else if (my >= buttonY - 540 && my <= buttonY - 480) {
				exit(0);
			}
		}
	}
}

void loadGameImages() {
	backgroundImage = loadImageWithCheck("images/background.bmp");
	birdImage = loadImageWithCheck("images/enemy.png");
	obsImage = loadImageWithCheck("images/cactus.bmp");
	heartImage = loadImageWithCheck("images/heart.bmp");
	fireImage = loadImageWithCheck("images/fire.bmp");
	shieldImage = loadImageWithCheck("images/shield.png");
	shield2Image = loadImageWithCheck("images/shield_2.png");
	coinImage = loadImageWithCheck("images/coin.png");
	asteroidImage = loadImageWithCheck("images/asteroid.png");
	loadSpriteFrames();
}

void loadSpriteFrames() {
	for (int i = 1; i <= 10; i++) {
		std::string filename = "images/Walk_" + intToString(i) + ".png";
		idleFrames.push_back(loadImageWithCheck(filename.c_str()));
	}
	for (int i = 1; i <= 8; i++) {
		std::string filename = "images/Run_" + intToString(i) + ".png";
		runFrames.push_back(loadImageWithCheck(filename.c_str()));
	}
	for (int i = 1; i <= 12; i++) {
		std::string filename = "images/Jump_" + intToString(i) + ".png";
		jumpFrames.push_back(loadImageWithCheck(filename.c_str()));
	}
	for (int i = 1; i <= 8; i++) {
		std::string filename = "images/Dead_" + intToString(i) + ".png";
		deadFrames.push_back(loadImageWithCheck(filename.c_str()));
	}
}

void updateAnimation() {
	frameCounter++;
	int currentFrameDelay = (dinoState == DEAD) ? deadFrameDelay : frameDelay;
	if (frameCounter >= currentFrameDelay) {
		frameCounter = 0;
		currentFrame++;
		switch (dinoState) {
		case IDLE:
			if (currentFrame >= (int)idleFrames.size()) currentFrame = 0;
			break;
		case RUN:
			if (currentFrame >= (int)runFrames.size()) currentFrame = 0;
			break;
		case JUMP:
			if (currentFrame >= (int)jumpFrames.size()) currentFrame = 0;
			break;
		case DEAD:
			if (currentFrame >= (int)deadFrames.size()) {
				currentFrame = deadFrames.size() - 1;
				if (dinoY <= 30) {
					isGameOver = true;
				}
			}
			break;
		}
	}
}

void spawnShield() {
	if (!isShieldActive) {
		shieldX = SCREEN_WIDTH + rand() % 500;
		shieldY = 100 + rand() % 300;
		isShieldActive = true;
	}
}

void resetShield() {
	isShieldActive = false;
	shieldX = -100;
	shieldY = 100;
	hasShield = false;
	shieldStartTime = 0;
	shieldBarWidth = SHIELD_BAR_WIDTH;
}

void spawnCoin() {
	if (coinSpawnCounter >= COIN_SPAWN_INTERVAL) {
		int numCoins = 2 + rand() % 4;
		for (int i = 0; i < numCoins; i++) {
			Coin newCoin;
			newCoin.x = SCREEN_WIDTH + rand() % 500;
			newCoin.y = 40;
			newCoin.active = true;
			coins.push_back(newCoin);
		}
		coinSpawnCounter = 0;
	}
	else {
		coinSpawnCounter++;
	}
}

void resetCoins() {
	coins.clear();
}

void spawnAsteroid() {
	Asteroid newAsteroid;
	newAsteroid.x = ASTEROID_FALLING_SPACE_X + rand() % 200;
	newAsteroid.y = SCREEN_HEIGHT;
	newAsteroid.active = true;
	asteroids.push_back(newAsteroid);
}

void resetAsteroids() {
	asteroids.clear();
}

bool isScoreInAsteroidRange() {
	return (score >= 1500 && score <= 2000) || (score >= 3000 && score <= 3500) || (score >= 4500 && score <= 5000);
}

void applyAsteroidGravity() {
	const double GRAVITY_CONSTANT = 0.1;
	for (size_t i = 0; i < asteroids.size(); i++) {
		if (asteroids[i].active) {
			double dx = asteroids[i].x - dinoX;
			double dy = asteroids[i].y - dinoY;
			double distanceSquared = dx * dx + dy * dy;
			if (distanceSquared < 200 * 200) {
				double force = GRAVITY_CONSTANT / distanceSquared;
				dinoX += static_cast<int>(force * dx);
				dinoY += static_cast<int>(force * dy);
			}
		}
	}
}

void updateAsteroidTrajectory() {
	for (size_t i = 0; i < asteroids.size(); i++) {
		if (asteroids[i].active) {
			double dx = dinoX - asteroids[i].x;
			double dy = dinoY - asteroids[i].y;
			double distance = sqrt(dx * dx + dy * dy);
			if (distance > 0) {
				dx /= distance;
				dy /= distance;
			}
			asteroids[i].x += static_cast<int>(dx * asteroidSpeed);
			asteroids[i].y += static_cast<int>(dy * asteroidSpeed);
			asteroids[i].x += rand() % 5 - 2;
			asteroids[i].y += rand() % 5 - 2;
		}
	}
}

void iDraw() {
	if (gameStarted) {
		iClear();
		iShowImage(bgX, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundImage);
		iShowImage(bgX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundImage);
		iSetColor(139, 69, 19);
		iFilledRectangle(0, 30, SCREEN_WIDTH, 30);

		int currentDinoImage = -1;
		switch (dinoState) {
		case IDLE:
			if (!idleFrames.empty() && currentFrame >= 0 && currentFrame < (int)idleFrames.size()) {
				currentDinoImage = idleFrames[currentFrame];
			}
			break;
		case RUN:
			if (!runFrames.empty() && currentFrame >= 0 && currentFrame < (int)runFrames.size()) {
				currentDinoImage = runFrames[currentFrame];
			}
			break;
		case JUMP:
			if (!jumpFrames.empty() && currentFrame >= 0 && currentFrame < (int)jumpFrames.size()) {
				currentDinoImage = jumpFrames[currentFrame];
			}
			break;
		case DEAD:
			if (!deadFrames.empty() && currentFrame >= 0 && currentFrame < (int)deadFrames.size()) {
				currentDinoImage = deadFrames[currentFrame];
			}
			break;
		}

		if (currentDinoImage != -1) {
			iShowImage(dinoX, dinoY, dinoWidth, dinoHeight, currentDinoImage);
		}

		if (hasShield && GetTickCount() - shieldStartTime < shieldDuration) {
			iShowImage(dinoX - 100, dinoY - 100, dinoWidth + 200, dinoHeight + 200, shield2Image);

			int shieldTimeLeft = shieldDuration - (GetTickCount() - shieldStartTime);
			float depletionFactor = 1.0f + (score / 1000.0f);
			shieldBarWidth = SHIELD_BAR_WIDTH * pow((shieldTimeLeft / (float)shieldDuration), depletionFactor);

			iSetColor(0, 0, 255);
			iFilledRectangle(shieldBarX, shieldBarY, shieldBarWidth, SHIELD_BAR_HEIGHT);

			iSetColor(0, 0, 0);
			iRectangle(shieldBarX, shieldBarY, SHIELD_BAR_WIDTH, SHIELD_BAR_HEIGHT);

			iShowImage(shieldBarX - 30, shieldBarY - 5, 45, 45, shieldImage);
		}

		if (isFireActive) {
			iShowImage(fireX, fireY, fireWidth, fireHeight, fireImage);
		}

		iShowImage(obsX, obsY, obsWidth, obsHeight, obsImage);

		if (currentGameMode != EASY) {
			iShowImage(birdX, birdY, birdWidth, birdHeight, birdImage);
		}

		if (isShieldActive) {
			iShowImage(shieldX, shieldY, shieldWidth, shieldHeight, shieldImage);
		}

		if (currentGameMode == HARD) {
			for (size_t i = 0; i < asteroids.size(); i++) {
				if (asteroids[i].active) {
					iShowImage(asteroids[i].x, asteroids[i].y, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroidImage);
				}
			}
		}

		for (size_t i = 0; i < coins.size(); i++) {
			if (coins[i].active) {
				iShowImage(coins[i].x, coins[i].y, COIN_WIDTH, COIN_HEIGHT, coinImage);
			}
		}

		sprintf_s(scoreText, sizeof(scoreText), "Score: %d", score);
		iSetColor(0, 0, 0);
		iText(20, SCREEN_HEIGHT - 30, scoreText, GLUT_BITMAP_HELVETICA_18);
		sprintf_s(highScoreText, sizeof(highScoreText), "High Score: %d", highScore);
		iText(20, SCREEN_HEIGHT - 50, highScoreText, GLUT_BITMAP_HELVETICA_18);

		for (int i = 0; i < lives; i++) {
			iShowImage(SCREEN_WIDTH - 150 + (i * 40), SCREEN_HEIGHT - 50, 30, 30, heartImage);
		}

		iSetColor(0, 0, 0);
		switch (currentGameMode) {
		case EASY:
			iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, "Easy Mode", GLUT_BITMAP_HELVETICA_18);
			break;
		case MEDIUM:
			iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, "Medium Mode", GLUT_BITMAP_HELVETICA_18);
			break;
		case HARD:
			iText(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 80, "Hard Mode", GLUT_BITMAP_HELVETICA_18);
			break;
		}

		if (isGameOver) {
			iSetColor(255, 0, 0);
			iText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, "Game Over! Press 'R' to Restart", GLUT_BITMAP_TIMES_ROMAN_24);
			iPauseTimer(0);
		}
	}
	else if (showScoreMenuScreen) {
		showScoreMenu();
	}
	else if (showIntroductionScreen) {
		showIntroduction();
	}
	else if (showHelpScreen) {
		showHelp();
	}
	else if (showAboutUsScreen) {
		showAboutUs();
	}
	else {
		iDrawMainMenu();
	}
}

void jump() {
	if (isJumping) {
		dinoY += jumpSpeed;
		currentJumpHeight += jumpSpeed;
		if (currentJumpHeight >= jumpHeight) {
			isJumping = false;
		}
	}
	else if (dinoY > 30) {
		dinoY += gravity;
	}
	if (dinoY < 30) {
		dinoY = 30;
	}
}

void resetObstacle() {
	obsX = SCREEN_WIDTH + rand() % 500;
}

void resetBird() {
	birdX = SCREEN_WIDTH + rand() % 800;
}

void resetGame() {
	dinoY = 30;
	isJumping = false;
	isFireActive = false;
	obsX = SCREEN_WIDTH + 500;
	birdX = SCREEN_WIDTH + 800;
	score = 0;
	bgX = 0;
	isGameOver = false;
	lives = 3;
	gameSpeed = initialGameSpeed;
	resetShield();
	resetCoins();
	resetAsteroids();
	isAsteroidFallingSpaceActive = false;
	dinoState = IDLE;
	currentFrame = 0;
	frameCounter = 0;
	iResumeTimer(0);
}

void increaseGameSpeed() {
	if (score % 100 == 0 && gameSpeed < 25) {
		gameSpeed++;
	}
}

void iTimer() {
	if (gameStarted && !isGameOver) {
		if (isPaused) {
			if (GetTickCount() - pauseStartTime >= pauseDuration) {
				isPaused = false;
				resetObstacle();
				resetBird();
				resetAsteroids();
				resetCoins();
			}
			return;
		}

		bgX -= bgSpeed;
		if (bgX <= -SCREEN_WIDTH) {
			bgX = 0;
		}

		if (currentGameMode == HARD && isScoreInAsteroidRange()) {
			isAsteroidFallingSpaceActive = true;
		}
		else {
			isAsteroidFallingSpaceActive = false;
		}

		if (currentGameMode == HARD && isAsteroidFallingSpaceActive) {
			if (asteroidSpawnCounter >= ASTEROID_SPAWN_INTERVAL) {
				spawnAsteroid();
				asteroidSpawnCounter = 0;
			}
			else {
				asteroidSpawnCounter++;
			}
		}

		if (currentGameMode == HARD) {
			updateAsteroidTrajectory();
		}

		if (currentGameMode == HARD) {
			for (size_t i = 0; i < asteroids.size(); i++) {
				if (asteroids[i].active) {
					asteroids[i].x -= asteroidSpeed;
					asteroids[i].y -= asteroidSpeed;
					if (asteroids[i].y < 0 || asteroids[i].x < 0) {
						asteroids[i].active = false;
					}
				}
			}
		}

		if (currentGameMode == HARD) {
			applyAsteroidGravity();
		}

		obsX -= gameSpeed;

		if (currentGameMode != EASY) {
			birdX -= gameSpeed;
		}

		shieldX -= gameSpeed;

		for (size_t i = 0; i < coins.size(); i++) {
			if (coins[i].active) {
				coins[i].x -= gameSpeed;
				if (coins[i].x < 0) {
					coins[i].active = false;
				}
			}
		}

		if (isFireActive) {
			fireX += fireSpeed;
			if (fireX > SCREEN_WIDTH || detectCollision(fireX, fireY, fireWidth, fireHeight, birdX, birdY, birdWidth, birdHeight)) {
				resetBird();
				isFireActive = false;
			}
			if (currentGameMode == HARD) {
				for (size_t i = 0; i < asteroids.size(); i++) {
					if (asteroids[i].active && detectCollision(fireX, fireY, fireWidth, fireHeight, asteroids[i].x, asteroids[i].y, ASTEROID_WIDTH, ASTEROID_HEIGHT)) {
						asteroids[i].active = false;
						isFireActive = false;
					}
				}
			}
		}
		if (obsX < 0) resetObstacle();
		if (birdX < 0 && currentGameMode != EASY) resetBird();
		if (shieldX < 0) resetShield();

		if (currentGameMode == HARD) {
			for (size_t i = 0; i < asteroids.size(); i++) {
				if (asteroids[i].active && detectCollision(dinoX, dinoY, dinoWidth, dinoHeight, asteroids[i].x, asteroids[i].y, ASTEROID_WIDTH, ASTEROID_HEIGHT)) {
					if (hasShield && GetTickCount() - shieldStartTime < shieldDuration) {
						asteroids[i].active = false;
					}
					else {
						lives--;
						if (lives > 0) {
							isPaused = true;
							pauseStartTime = GetTickCount();
						}
						else {
							dinoState = DEAD;
						}
					}
				}
			}
		}

		if (detectCollision(dinoX, dinoY, dinoWidth, dinoHeight, obsX, obsY, obsWidth, obsHeight)) {
			if (hasShield && GetTickCount() - shieldStartTime < shieldDuration) {
			}
			else {
				lives--;
				if (lives > 0) {
					isPaused = true;
					pauseStartTime = GetTickCount();
				}
				else {
					dinoState = DEAD;
				}
			}
		}

		if (currentGameMode != EASY && detectCollision(dinoX, dinoY, dinoWidth, dinoHeight, birdX, birdY, birdWidth, birdHeight)) {
			if (hasShield && GetTickCount() - shieldStartTime < shieldDuration) {
			}
			else {
				lives--;
				if (lives > 0) {
					isPaused = true;
					pauseStartTime = GetTickCount();
				}
				else {
					dinoState = DEAD;
				}
			}
		}

		if (isShieldActive && detectCollision(dinoX, dinoY, dinoWidth, dinoHeight, shieldX, shieldY, shieldWidth, shieldHeight)) {
			hasShield = true;
			isShieldActive = false;
			shieldStartTime = GetTickCount();
		}

		for (size_t i = 0; i < coins.size(); i++) {
			if (coins[i].active && detectCollision(dinoX, dinoY, dinoWidth, dinoHeight, coins[i].x, coins[i].y, COIN_WIDTH, COIN_HEIGHT)) {
				score += 100;
				coins[i].active = false;
			}
		}

		spawnShield();
		spawnCoin();

		if (hasShield && GetTickCount() - shieldStartTime >= shieldDuration) {
			hasShield = false;
		}

		score++;
		increaseGameSpeed();
	}
	jump();
	updateAnimation();
}

void iMouse(int button, int state, int mx, int my) {
	if (gameStarted) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			if (!isGameOver && dinoY == 30) {
				isJumping = true;
				currentJumpHeight = 0;
				dinoState = JUMP;
			}
			else if ((mx >= dinoX && mx <= dinoX + dinoWidth && my >= dinoY && my <= dinoY + dinoHeight)) {
				if (!isFireActive) {
					isFireActive = true;
					fireX = dinoX + dinoWidth / 2;
					fireY = dinoY;
				}
			}
		}
	}
	else {
		iMouseMainMenu(button, state, mx, my);
	}
}

void iKeyboard(unsigned char key) {
	if (gameStarted) {
		if (key == 'r' || key == 'R') {
			resetGame();
		}
		if (key == ' ') {
			if (!isGameOver && dinoY == 30) {
				isJumping = true;
				currentJumpHeight = 0;
				dinoState = JUMP;
			}
		}
		if (key == 'b' || key == 'B') {
			gameStarted = false;
			showScoreMenuScreen = false;
			resetGame();
		}
	}
	else if (showScoreMenuScreen) {
		if (key == 27) {
			showScoreMenuScreen = false;
		}
	}
	else if (showIntroductionScreen) {
		if (key == 27) {
			showIntroductionScreen = false;
		}
	}
	else if (showHelpScreen) {
		if (key == 27) {
			showHelpScreen = false;
		}
		else if (key == 'n' || key == 'N') {
			showNextHelpImage();
		}
	}
	else if (showAboutUsScreen) {
		if (key == 27) {
			showAboutUsScreen = false;
		}
	}
}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_UP) {
		if (!isFireActive) {
			isFireActive = true;
			fireX = dinoX + dinoWidth / 2;
			fireY = dinoY;
		}
	}
}

bool detectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return !(x1 + w1 < x2 || x1 > x2 + w2 || y1 + h1 < y2 || y1 > y2 + h2);
}

void playBackgroundMusic() {
	PlaySound("dino_music.wav", NULL, SND_LOOP | SND_ASYNC);
}

void saveHighScore() {
	std::ofstream file("scores.txt");
	if (file.is_open()) {
		file << highScore;
		file.close();
	}
}

void loadHighScore() {
	std::ifstream file("scores.txt");
	if (file.is_open()) {
		file >> highScore;
		file.close();
	}
	else {
		highScore = 0;
	}
}

void showScoreMenu() {
	iClear();
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backscoreImage);
	iSetColor(0, 128, 0);
	iText(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 100, "High Score:", GLUT_BITMAP_TIMES_ROMAN_24);
	sprintf_s(highScoreText, sizeof(highScoreText), "%d", highScore);
	iText(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2, highScoreText, GLUT_BITMAP_TIMES_ROMAN_24);
	iText(20, 20, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
}

void showIntroduction() {
	iClear();
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, introductionImage);
	iSetColor(0, 128, 0);
	iText(SCREEN_WIDTH - 400, 20, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
}

void showHelp() {
	iClear();
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, helpImages[currentHelpImage]);
	iSetColor(0, 128, 0);
	iText(SCREEN_WIDTH - 400, 50, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
	iText(SCREEN_WIDTH - 400, 20, "Press 'N' to see the next image", GLUT_BITMAP_HELVETICA_18);
}

void showAboutUs() {
	iClear();
	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, aboutUsImage);
	iSetColor(0, 128, 0);
	iText(SCREEN_WIDTH - 400, 20, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
}

void showNextHelpImage() {
	currentHelpImage++;
	if (currentHelpImage >= 5) {
		currentHelpImage = 0;
	}
}

void iMouseMove(int mx, int my) {
}

void iPassiveMouseMove(int mx, int my) {
}

int main() {
	iInitialize(windowWidth, windowHeight, "Dino Dash: Cactus Run");
	srand(time(0));
	iLoadMainMenuImages();
	loadGameImages();
	playBackgroundMusic();
	loadHighScore();
	iSetTimer(30, iTimer);
	iStart();
	return 0;
}
