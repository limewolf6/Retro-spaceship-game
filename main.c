#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 40
#define PLAYER '^'
#define ENEMY 'M'
#define BULLET '.'
#define EMPTY ' '

char grid[ROWS][COLS];
int playerPos = COLS / 2;
int score = 0;
int gameOver = 0;

void initGrid(){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            grid[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < 2; i++){
        for (int j = 5; j < COLS - 5; j += 2){
            grid[i][j] = ENEMY;
        }
    }

    grid[ROWS - 1][playerPos] = PLAYER;
}

void drawGrid(){
    system("cls");
    printf("Score: %d\n", score);
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

void movePlayer(char key){
    grid[ROWS - 1][playerPos] = EMPTY;

    if (key == 'a' && playerPos > 0)
        playerPos--;
    else if (key == 'd' && playerPos < COLS - 1)
        playerPos++;
    else if (key == ' ')
        grid[ROWS - 2][playerPos] = BULLET;

    grid[ROWS - 1][playerPos] = PLAYER;
}

void updateBullets(){
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++){
            if (grid[i][j] == BULLET){
                // If bullet is at the top row, remove it (missed)
                if (i == 0){
                    grid[i][j] = EMPTY;
                    continue;
                }

                // Check if bullet hit an enemy
                if (grid[i - 1][j] == ENEMY){
                    grid[i - 1][j] = EMPTY;  // Delete enemy
                    grid[i][j] = EMPTY;      // Delete bullet
                    score += 10;
                    Beep(700, 50);
                }
                // Move bullet upward if space is empty
                else if (grid[i - 1][j] == EMPTY){
                    grid[i - 1][j] = BULLET;
                    grid[i][j] = EMPTY;
                }
                // If bullet hits anything else (like a wall), delete it
                else{
                    grid[i][j] = EMPTY;
                }
            }
        }
    }
}

int enemiesLeft() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (grid[i][j] == ENEMY)
                return 1;
    return 0;
}

int main() {
    printf("Welcome to Space Invaders\n");
    while(1){
        int a;
        printf("1. Play\n2. Controls\n3. About Us\n4. Exit\n");
        scanf("%d",&a);

        if(a == 1){
            initGrid();
            drawGrid();

            while (!gameOver){
                Sleep(100);

                if (_kbhit()){
                    char key = _getch();
                    movePlayer(key);
                }

                updateBullets();
                drawGrid();

                if (!enemiesLeft()){
                    printf("\nYou WIN! Final Score: %d\n", score);
                    gameOver = 1;
                }
            }
            continue;
        }
        else if(a == 2){
            printf("To play the game use:\n -A/D to move left or right\n -Spacebar to shoot\n\n");
        }
        else if(a == 3){
            printf("This is a CLI based game made by Group 8 for CSE115.4\n\n");
            printf("Instructor: Mohammad Shifat-E-Rabbi\n\nGroup 8 Members\n 1.Radit Rifah Rahman\n 2.Seendeed Islam\n 3.Afrida Hossain Khan\n\n");
            printf("Github Repository Link: https://github.com/limewolf6/Retro-spaceship-game-CSE-grp-8\n\n");
            continue;
        }
        else if(a == 4){
            printf("Goodbye\n");
            break;
        }
        else{
            printf("Invalid\n\n");
            continue;
        }
    }
    return 0;
}

