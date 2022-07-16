#include <iostream>
#include <conio.h>
#include <random>
#include <windows.h>
#include <time.h> 
#include <math.h>

const int width = 9, height = 9, winScore = 15;
const char snakeHeadChar = '0', snakeBodyChar = 'o', groundChar = ' ', borderChar = '#';
char field[width][height];//[x][y]
int snakeHeadCords[2], snake[winScore+3][2];//{x, y}
int score = 0;


struct Apple {
	const char appleView = '@';
	int x;
	int y;
	bool isAppleExsist;
};

Apple apple;

void startScreen() {
	Sleep(1000);
	for (int i = 3; i > 0; i--) {
		std::cout << i<<"\n";
		Sleep(500);
	}
	std::cout << "start\n";
	Sleep(500);
}
void createArr()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			field[i][j] = groundChar;
		}
	}
}

void createSnake() {
	snake[0][0] = snakeHeadCords[0]; snake[0][1] = snakeHeadCords[1];
	snake[1][0] = snakeHeadCords[0]; snake[1][1] = snakeHeadCords[1] + 1;
	snake[2][0] = snakeHeadCords[0]; snake[2][1] = snakeHeadCords[1] + 2;
	field[snakeHeadCords[0]][snakeHeadCords[1]] = snakeHeadChar;
	for (int i = 1; i < score + 3; i++) {
		field[snake[i][0]][snake[i][1]] = snakeBodyChar;
	}
}

void snakeBodyRender() {
	field[snake[score+2][0]][snake[score + 2][1]] = groundChar;
	for (int i = score + 3; i > 0; i--) { snake[i][0] = snake[i - 1][0]; snake[i][1] = snake[i - 1][1]; }
	snake[0][0] = snakeHeadCords[0]; snake[0][1] = snakeHeadCords[1];;
	field[snakeHeadCords[0]][snakeHeadCords[1]] = snakeHeadChar;
	for (int i = 1; i < score + 3; i++) {
		field[snake[i][0]][snake[i][1]] = snakeBodyChar;
	}
}

int border(int id, int max) {//проверка что значение находится в пределах поля
	if (id < 0) { return max - 1; }
	else if (id > max - 1) { return 0; }
	return id;
}

void move(char input) {
	field[snakeHeadCords[0]][snakeHeadCords[1]] = groundChar;
	if (input == 'a') { snakeHeadCords[0] -= 1; }
	else if (input == 'd') { snakeHeadCords[0] += 1; }
	else if (input == 'w') { snakeHeadCords[1] -= 1; }
	else if (input == 's') { snakeHeadCords[1] += 1; }
	snakeHeadCords[0] = border(snakeHeadCords[0], width); snakeHeadCords[1] = border(snakeHeadCords[1], height);
}

bool collision() {
	return (field[snakeHeadCords[0]][snakeHeadCords[1]] == groundChar || field[snakeHeadCords[0]][snakeHeadCords[1]] == apple.appleView);
}
void reset() {
	score = 0;
	snakeHeadCords[0] = width / 2; snakeHeadCords[1] = height / 2;
	apple.isAppleExsist = false;
	for (int i = 0; i < winScore + 3; i++) {
		snake[i][0] = width + 10; snake[i][1] = height + 10;
	}
}

bool AppleIsNotSnake(int x, int y){
	for (int i = 0; i < score + 3; i++) {
		if (snake[i][0] == x && snake[i][1] == y)return true;
	}
	return false;
}

void createApple() {
	bool isSnake=false;
	srand(static_cast<unsigned int>(time(0)));
	int randm = rand();
	apple.x = randm % width;
	apple.y = (int)pow(randm, 2) % height;
	isSnake = AppleIsNotSnake(apple.x, apple.y);
	if (isSnake == false)
	{
		field[apple.x][apple.y] = apple.appleView;
		apple.isAppleExsist = true;
	}
	else score += 1;
}

void eatApple() {
	if (snakeHeadCords[0] == apple.x && snakeHeadCords[1] == apple.y && apple.isAppleExsist) { apple.isAppleExsist = false; score+=1; }
}

void win() {
	system("cls");
	std::cout << "You win!\nYou score is: " << score << "." << std::endl;
	Sleep(1000);
}

void loose() {
	system("cls");
	std::cout << "You loose!\nYou score is: " << score << "." << std::endl;
	Sleep(1000);
}

void screenOutput() {
	for (int j = -1; j < height + 1; j++)
	{
		for (int i = -1; i < width + 1; i++)
		{
			if (j >= 0 && j < height && i >= 0 && i < width)std::cout << field[i][j];
			else std::cout << borderChar;
		}
		std::cout << "\n";
	}
}
void debug() {
	if (apple.isAppleExsist) std::cout << apple.x << apple.y << std::endl; else std::cout << "not exsist" << std::endl;
	std::cout << snakeHeadCords[0] << snakeHeadCords[1] << std::endl;
}
void render(bool debugMode) {
	char input = 'w';
	//startScreen();
	createArr();
	createSnake();
	while (input != ' ')
	{	
		system("cls");
		if (_kbhit())
		{
			input = _getch();
		}
		if (!apple.isAppleExsist)
		{ 
			createApple();
			eatApple(); 
		}
		if(debugMode)debug();
		screenOutput();
		std::cout<< "Score: " << score << std::endl;
		move(input);
		if (!collision()) {
			loose(); 
			break;
		}
		eatApple();	
		if (score == winScore) {
			win();
			break;	
		}
		snakeBodyRender();
		Sleep(100);
	}
}

bool restart() {
	char input2;
	std::cout << "Restart? y - Yes, n - Not\n";
	while (true) {
		std::cin >> input2;
		if (input2 != 'y' && input2 != 'n') { std::cout << "Only y or n input!\n"; continue; }
		else break;
	}
	return (input2 == 'y') ? true : false;
}

void snakeGame(bool debugMode) {
	reset();
	render(debugMode);
	while(restart()){reset(); render(debugMode);}
}

int main()
{
	snakeGame(false/*debugMode, false to off*/);
	return 0;
}