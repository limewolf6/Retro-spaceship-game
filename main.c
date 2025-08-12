#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define Rows 20
#define Colm 40
#define Player 'A'
#define Invader 'M'
#define Bullet '.'
#define Empty ' '

int invaderDirection = 1; // 1 for right, -1 for left
int moveCounter = 0;      // Control speed of invader movement
int moveThreshold = 3;   // Adjust for speed — higher = slower

char grid[Rows][Colm];
int playerPos = Colm / 2;
int score = 0;
int gameOver = 0;

void initGrid(){
    for (int i = 0; i < Rows; i++){  //for cleaning grid
        for (int j = 0; j < Colm; j++){
            grid[i][j] = Empty;
        }
    }

    for (int i = 0; i < 2; i++){  //for placing invaders
        for (int j = 5; j < Colm - 5; j += 2){
            grid[i][j] = Invader;
        }
    }

    grid[Rows - 1][playerPos] = Player;  //for placing player
}

void drawGrid(){
    system("cls");
    printf("Score: %d\n", score);  //gives score
    for (int i = 0; i < Rows; i++){
        for (int j = 0; j < Colm; j++){
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}
//player controls
void movePlayer(char key){
    grid[Rows - 1][playerPos] = Empty;

    if (key == 'a' && playerPos > 0)
        playerPos--;
    else if (key == 'd' && playerPos < Colm - 1)
        playerPos++;
    else if (key == ' ')
        grid[Rows - 2][playerPos] = Bullet;

    grid[Rows - 1][playerPos] = Player;
}

void updateBullets(){
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Colm; j++){
            if (grid[i][j] == Bullet){  // If bullet is at the top row, remove it (missed)
                if (i == 0){
                    grid[i][j] = Empty;
                    continue;
                }

                // Check if bullet hit an enemy
                if (grid[i - 1][j] == Invader){
                    grid[i - 1][j] = Empty;  // Delete enemy
                    grid[i][j] = Empty;      // Delete bullet
                    score += 10;
                    Beep(700, 50);
                }
                // Move bullet upward if space is empty
                else if (grid[i - 1][j] == Empty){
                    grid[i - 1][j] = Bullet;
                    grid[i][j] = Empty;
                }
                // If bullet hits anything else (like a wall), delete it
                else{
                    grid[i][j] = Empty;
                }
            }
        }
    }
}

void updateInvaders(){
    moveCounter++;
    if (moveCounter < moveThreshold) return; // Move only every few cycles
    moveCounter = 0;

    int shiftDown = 0;

    // Check if invaders hit wall
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Colm; j++) {
            if (grid[i][j] == Invader) {
                if ((invaderDirection == 1 && j == Colm - 1) ||
                    (invaderDirection == -1 && j == 0)) {
                    invaderDirection *= -1; // Change direction
                    shiftDown = 1;
                    break;
                }
            }
        }
        if (shiftDown) break;
    }

    // Move invaders
    if (shiftDown) {
        for (int i = Rows - 2; i >= 0; i--) {
            for (int j = 0; j < Colm; j++) {
                if (grid[i][j] == Invader) {
                    grid[i][j] = Empty;
                    if (i + 1 == Rows - 1) {
                        // Invader reached player row
                        gameOver = 1;
                        return;
                    }
                    grid[i + 1][j] = Invader;
                }
            }
        }
    } else {
        // Horizontal move
        if (invaderDirection == 1) {
            for (int i = Rows - 1; i >= 0; i--) {
                for (int j = Colm - 2; j >= 0; j--) {
                    if (grid[i][j] == Invader) {
                        grid[i][j] = Empty;
                        grid[i][j + 1] = Invader;
                    }
                }
            }
        } else {
            for (int i = Rows - 1; i >= 0; i--) {
                for (int j = 1; j < Colm; j++) {
                    if (grid[i][j] == Invader) {
                        grid[i][j] = Empty;
                        grid[i][j - 1] = Invader;
                    }
                }
            }
        }
    }
}

//checks for invaders
int invadersLeft() {
    for (int i = 0; i < Rows; i++)
        for (int j = 0; j < Colm; j++)
            if (grid[i][j] == Invader)
                return 1;
    return 0;
}

int main() {
    printf("Welcome to Space Invaders\n");
    while(1){
        int a;
        printf("1. Play\n2. Controls\n3. About\n4. Exit\n");
        scanf("%d",&a);

        if(a == 1){
            score = 0;
            gameOver = 0;
            playerPos = Colm/2;
            invaderDirection = 1;
            moveCounter = 0;
            initGrid();
            drawGrid();

            while (!gameOver){
                Sleep(100);

                if (_kbhit()){
                    char key = _getch();
                    movePlayer(key);
                }

                updateBullets();
                updateInvaders();
                drawGrid();

                if (!invadersLeft()){
                    printf("\nYou WIN! Final Score: %d\n", score);
                    gameOver = 1;
                }
                for (int j = 0; j < Colm; j++) {
                    if (grid[Rows - 2][j] == Invader) {
                        printf("\nGame Over! Final Score: %d\n", score);
                        gameOver = 1;
                        break;
                    }
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
            printf("Github Repository Link: https://github.com/limewolf6/Retro-spaceship-game\n\n");
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

