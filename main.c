#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    int size;
    int dificulty;
}Sudoku;

// Function declarations
void clearScreen();
int** createBoard(int size);
void freeBoard(int** board, int size);
void printBoard(int** board, int size);
bool isNumberValid(int** board, int size, int row, int col, int num);
bool findEmptyCell(int** board, int size, int* row, int* col);
bool isBoardSolved(int** board, int size);
bool solveBoard(int** board, int size);
void removeCells(int** board, int size, int percentage);
void playGame(int** board, int size);
void saveGame(int** board, int size);
void loadGame();
void showInstructions();
int getBoardSize();
int getDifficultyLevel();

int** board = NULL;

//method for clear cmd
void clear() {
    for (int i = 0; i < 100; i++) printf("\n");
}

// method for saving current board to the file
void savegame(int** board, int size) {
    printf("Saving this game...\n");
    FILE* file = fopen("game.txt", "w");
    if (!file) {
        printf("Error after saving game.\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved finished!\n");
    printf("Click Enter to continue...");
    while ((getchar()) != '\n');
    getchar();
}

// method for creating board of given size
int** createboard(int size) {
    int** board = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        board[i] = calloc(size, sizeof(int));
    }
    return board;
}

// frees memory allocated for the board
void freeboard(int** board, int size) {
    for (int i = 0; i < size; i++) {
        free(board[i]);
    }
    free(board);
}

//printing board with | and -- around and . wheres no values
void print_board(int** board, int size) {
    int box_size = sqrt(size);

    for (int i = 0; i < size; i++) {
        if (i % box_size == 0) {
            for (int k = 0; k < size + box_size; k++) {
                printf("--");
            }
            printf("\n");
        }

        for (int j = 0; j < size; j++) {
            if (j % box_size == 0) {
                printf("| ");
            }
            if (board[i][j] == 0)
                printf(". ");
            else
                printf("%d ", board[i][j]);
        }
        printf("|\n");
    }

    for (int k = 0; k < size + box_size; k++) {
        printf("--");
    }
    printf("\n");
}

// Checks if placing a number is available (row, column, and cube)
int available_place(int** board, int size,int row,int col,int num)
{
    int cubesize = (int)sqrt(size);
    
    // checking row 
    for(int i=0;i<size;i++)
    {
        if(board[row][i] == num)
            return 0;
    }
    
    // checking col 
    for(int i=0;i<size;i++)
    {
        if(board[i][col] == num)
            return 0;
    }
    
    //checking cube
    int startingRow = (row / cubesize) * cubesize;
    int startingCol = (col / cubesize) * cubesize;
    for(int i=0;i<cubesize;i++)
    {
        for(int j=0;j<cubesize;j++)
        {
            if (board[startingRow + i][startingCol + j] == num)
            return 0;
        }
    }
    return 1;
}


int findempty(int** board, int size,int *row, int*col)
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            if (board[i][j] == 0){
                *row=i;
                *col=j;
                return 1;
            }
        }
    }
    return 0;
}

// method for check if the current board is finished and correctly solved
bool issolved(int** board, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int num = board[row][col];
            if (num == 0) 
                return false;

            board[row][col] = 0;
            if (!available_place(board, size, row, col, num)) {
                board[row][col] = num;
                return false;
            }
            board[row][col] = num;
        }
    }
    return true;
}

// method for making correct board
int solvedboard(int** board, int size)
{
    int row,col;
    
    int* numbers = malloc(size * sizeof(int));
    if(!findempty(board,size, &row, &col))  
    {
        return 1; // Solved
    }
    
    for (int i = 0; i < size; i++) {
        numbers[i] = i + 1;
    }
    
     for (int i = 0; i < size; i++) {
        int j = rand() % size;
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
    
    for(int i=0;i<size;i++)
    {
        if(available_place(board ,size,row,col,numbers[i]))
        {
            board[row][col] = numbers[i];

            if (solvedboard(board,size)){
                free(numbers);
                return 1;
            }
            
            board[row][col] = 0;
        }
    }
    free(numbers);
    return 0;
}

// method for removing a % of filled cells based on difficulty 35 for easy , 55 for medium and 70 for hard
void removecells(int** board,int size, int percentage) {
    int totalcells = size * size;
    int cellstoremove = (percentage * totalcells) / 100;

    while (cellstoremove > 0) {
        int row = rand() % size;
        int col = rand() % size;

        if (board[row][col] != 0) {
            board[row][col] = 0;
            cellstoremove--;
        }
    }
}

//method for player input in loop +  save ( 0 0 0) and quit ( -1 -2 -3) 
void userinput(int** board, int size)
{
    int row,col,num;
    while(1){
        clear();
        print_board(board, size);
        printf("Write row col and num (0 0 0 to quit or -1 -2 -3 to save): ");
        scanf("%d %d %d", &row, &col, &num);
        
        if (row == -1 && col == -2 && num == -3) {
            savegame(board, size);
            continue;
        }
        
        if (row == 0 || col == 0 && num == 0)
            break;
        if(row>size || row<1 || col>size || col<1 || num>size || num<1)
        {
            printf("\nWrong number. Write again. ");
            printf("Click Enter, to continue...");
            while ((getchar()) != '\n');
            getchar();
            continue;
        }
        if (board[row - 1][col - 1] != 0) {
            printf("You cant write here!\n");
            printf("Click Enter, to continue...");
            while ((getchar()) != '\n');
            getchar();
            continue;
        }

        if (available_place(board,size, row - 1, col - 1, num)) {
            board[row - 1][col - 1] = num;

            if (issolved(board, size)) {
                clear();
                print_board(board, size);
                printf("Congratulations! You won!\n");
                printf("Click Enter to exit...");
                while ((getchar()) != '\n');
                getchar();
                return;
            }
            continue;
        }   
        
        else {
            printf("Wrong move!\n");
            printf("Click Enter, to continue...");
            while ((getchar()) != '\n');
            getchar();
        }
    }
}


// method for load old board from file
void loadgame()
{
    printf("Loading old game...\n");
    FILE* file = fopen("game.txt", "r");
    if (!file) {
        printf("No saved game found.\n");
        printf("Click Enter to continue...");
        while ((getchar()) != '\n');
        getchar();
        return;
    }

    int sizefromfile;
    fscanf(file, "%d", &sizefromfile);
    board = createboard(sizefromfile);

    for (int i = 0; i < sizefromfile; i++) {
        for (int j = 0; j < sizefromfile; j++) {
            fscanf(file, "%d", &board[i][j]);
        }
    }
    
    fclose(file);
    printf("Game loaded successfully!\n");
    userinput(board, sizefromfile);
    
}

//menu
int main()
{
    bool end = false;
    Sudoku* game = NULL;
    int size;
    int dificulty;
    int row,col,num;
while(!end)
    {
        int choice;
        clear();
        printf("\n-- Welcome to Sudoku --\n");
        printf("1. Start Game\n");
        printf("2. Load Game\n");
        printf("3. Instruction\n");
        printf("4. Exit Game\n");
        printf("\nPlease choose option: ");
        scanf("%d", &choice);
        printf("\n");
    
        switch(choice){
            case 1:
                int boardsize;
                
                clear();
                printf("-- Choose your board size: --\n");
                printf("\n1. 4x4 | 2. 9x9 | 3. 16x16\n");
                printf("Select: ");
                
                do {
                    scanf("%d", &boardsize);
                    if (boardsize==1){
                        size=4;
                        break; 
                    }
                    else if (boardsize==2){
                        printf("\nSelected: 9x9\n");
                        size=9;
                        break; 
                    }
                    else if (boardsize==3){
                        printf("\nSelected: 16x16\n");
                        size=16;
                        break; 
                    }
                    else
                    {
                        printf("You choose wrong number. Choose again ");
                    }
                } while(boardsize<1 || boardsize>3);
                board = createboard(size);
                
                int dificultylevel;
                clear();
                printf("-- Choose your level: -- \n");
                printf("\n1. Easy | 2. Medium | 3. Hard\n");
                printf("Select: ");
                do {
                    scanf("%d", &dificultylevel);
                    if (dificultylevel==1){
                        printf("\nSelected: Easy\n");
                        dificulty = 35;
                        break; 
                    }
                    else if (dificultylevel==2){
                        printf("\nSelected: Medium\n");
                        dificulty = 55;
                        break; 
                    }
                    else if (dificultylevel==3){
                        printf("\nSelected: Hard\n");
                        dificulty = 70;
                        break; 
                    }
                    else
                    {
                        printf("You choose wrong number. Choose again: ");
                    }
                }while(dificultylevel<1 || dificultylevel>3);
                srand(time(NULL));
                solvedboard(board,size);
                removecells(board, size, dificulty);
                print_board(board, size);
                userinput(board, size);
                break;
            case 2:
                loadgame();
                printf("\nClick Enter, to continue...");
                while ((getchar()) != '\n');
                getchar();
                break;
            case 3:
                printf("-- Instruction for Sudoku --\n");
                printf("Sudoku is a logic puzzle where you fill in a grid so that every row, column, and marked region contains all the required numbers without repeating. I offer three different grid sizes and difficulty levels, making it fun for both beginners and experienced players. Just choose your preferred size and difficulty, and start solving! \n");
                printf("1. Write 1 to 'Start Game' \n");
                printf("2. Choose your board size. 1 is for 4x4 2 is for 9x9 and 3 is for 16x16 \n");
                printf("3. Choose your level. 1 is for easy 25% 2 is for medium 55% and 3 is for hard% \n");
                printf("4. Board will generate for you\n");
                printf("5. Write row + lap + number where you want\n");
                printf("6. Program will automaticaly check if you did it wrong\n");
                printf("7. write '-1 -2 -3' if you want save game to file\n");
                printf("7. write '0 0 0' if you want leave game\n");
                printf("8. Have fun :)\n");
                printf("Click Enter, to continue...");
                while ((getchar()) != '\n');
                getchar();
                break;
            case 4:
               end = true;
               printf("\nThank you for game! :)");
               break;
            default:
                printf("You choose wrong number, choose again. ");
                printf("Click Enter, to continue...");
                while ((getchar()) != '\n');
                getchar();
                break;
    }
}
    freeboard(board, size);
    return 0;
}