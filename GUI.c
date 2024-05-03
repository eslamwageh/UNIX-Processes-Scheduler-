#include "raylib.h"
#include <string.h>
#include "unistd.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_TEXT_LENGTH 5000
#define MAX_INPUT_CHARS 20
int pid;
typedef enum { STATE_INPUT, STATE_SECOND_WINDOW, STATE_ROUND_ROBIN, STATE_STRN, STATE_HPF, STATE_ROUND_ROBIN_INPUT } State;
char inputText[] = "\0";



void SIGINT_HANDLER(int signum)
{
        generate_schedlog();
}


int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    signal(SIGINT, SIGINT_HANDLER);
     // Input text buffer
    char quantumText[MAX_INPUT_CHARS + 1] = "\0"; // Quantum time input buffer
    bool showMessageBox = false; // Flag to show message box
    State currentState = STATE_INPUT;
    State nextState = STATE_INPUT; // State transition after making a choice

    InitWindow(screenWidth, screenHeight, "Raylib String Input Example");
    char filepath[100], algo[100], quantum[100];
    strcpy(quantum, "-1");
    int a = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update
        if (currentState == STATE_INPUT) {
            // Handle input textLAESLAESLAESL
             int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (strlen(inputText) < MAX_INPUT_CHARS)) {
                inputText[strlen(inputText)] = (char)key;
                inputText[strlen(inputText) + 1] = '\0'; // Ensure null-termination
            } else if (IsKeyPressed(KEY_BACKSPACE) && (strlen(inputText) > 0)) {
                inputText[strlen(inputText) - 1] = '\0';
            } else if (IsKeyPressed(KEY_ENTER)) {
                // Change state to show the second window
                nextState = STATE_SECOND_WINDOW;
            }
            //strcpy(filepath, inputText);
            //printf("%s",inputText);
            
            

        } else if (currentState == STATE_SECOND_WINDOW && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Change state based on mouse click position
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, (Rectangle){100, 100, 200, 50})) {
                nextState = STATE_ROUND_ROBIN_INPUT;
                strcpy(algo, "3");
            } else if (CheckCollisionPointRec(mousePos, (Rectangle){100, 160, 200, 50})) {
                nextState = STATE_STRN;
                strcpy(algo, "2");
                break;
            } else if (CheckCollisionPointRec(mousePos, (Rectangle){100, 220, 200, 50})) {
                nextState = STATE_HPF;
                strcpy(algo, "1");
                break;
            }
            printf(algo);
        } else if (currentState == STATE_ROUND_ROBIN_INPUT) {
            // Handle input text for quantum time
            int key = GetKeyPressed();
            if ((key >= 48 && key <= 57) && (strlen(quantumText) < MAX_INPUT_CHARS)) { // Only allow numbers
                quantumText[strlen(quantumText)] = (char)key;
            } else if (IsKeyPressed(KEY_BACKSPACE) && (strlen(quantumText) > 0)) {
                quantumText[strlen(quantumText) - 1] = '\0';
            } else if (IsKeyPressed(KEY_ENTER)) {
                // Change state to show Round Robin window after entering quantum time
                nextState = STATE_ROUND_ROBIN;
                break;
            }
            strcpy(quantum, quantumText);
            printf(quantum);
            
        }

        // State transition
        if (nextState != currentState) {
            currentState = nextState;
            // Reset input text buffers when changing states
            // inputText[0] = '\0';
            // quantumText[0] = '\0';
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (currentState == STATE_INPUT) {
            DrawText("Enter FILE PATH:", 10, 10, 20, BLACK);
            DrawRectangle(10, 40, 200, 40, LIGHTGRAY);
            DrawText(inputText, 20, 50, 20, MAROON);
        } else if (currentState == STATE_SECOND_WINDOW) {
            DrawText("Choose an option:", 10, 10, 20, BLACK);
            DrawRectangle(100, 100, 200, 50, LIGHTGRAY);
            DrawText("1- Round Robin", 110, 110, 20, MAROON);
            DrawRectangle(100, 160, 200, 50, LIGHTGRAY);
            DrawText("2- STRN", 110, 170, 20, MAROON);
            DrawRectangle(100, 220, 200, 50, LIGHTGRAY);
            DrawText("3- HPF", 110, 230, 20, MAROON);
        } else if (currentState == STATE_ROUND_ROBIN_INPUT) {
            DrawText("Enter quantum time:", 10, 10, 20, BLACK);
            DrawRectangle(10, 40, 200, 40, LIGHTGRAY);
            DrawText(quantumText, 20, 50, 20, MAROON);
        } else if (currentState == STATE_ROUND_ROBIN) {
            DrawText("Round Robin Window", 10, 10, 20, BLACK);
        } else if (currentState == STATE_STRN) {
            DrawText("STRN Window", 10, 10, 20, BLACK);
        } else if (currentState == STATE_HPF) {
            DrawText("HPF Window", 10, 10, 20, BLACK);
            break;
        }
        
        // {
        //     printf("%s \n",inputText);
        //     printf("fkflsdlkfsdfsdfafsdf\n");
        //     printf("%s\n", quantum);
        //     a++;
        // }
        
        EndDrawing();
    }

    
    printf("\n %s \n",inputText);
    CloseWindow();

    pid = fork();
    if(pid == 0)
    {
        printf("iam the child\n");
        execl("./process_generator.out", "process_generator", inputText, algo, quantum, NULL);
    }
    else {
            //we moved them to the sigint handler
            int statlock;
        int cid = waitpid(pid, &statlock, 0);
        if (WIFEXITED(statlock))
        {
            printf("GUI terminated successfully with status %d.", WEXITSTATUS(statlock));
            //generate image
        }
        else
            printf("Something went wrong with the GUI.\n");
        }

    return 0;
}

void generate_schedlog()
{
            // Read text from file
        FILE *file = fopen("scheduler.log", "r");
        if (file == NULL)
        {
            printf("Error: Unable to open file.\n");
            return 1;
        }

        char text[MAX_TEXT_LENGTH] = "";
        char line[MAX_TEXT_LENGTH];
        while (fgets(line, MAX_TEXT_LENGTH, file) != NULL)
        {
            strcat(text, line);
        }
        for (int i = 0; i < 10; i++)
        {
            strcat(text, "\n");
        }
        
        fclose(file);

        // Initialize Raylib
        InitWindow(800, 600, "Text to Image");

        // Load font
        Font font = LoadFont("resources/font.ttf");
        SetTargetFPS(60);

        // Measure text size
        Vector2 textSize = MeasureTextEx(font, text, 20, 1);

        // Create an image to draw text onto
        Image image = GenImageColor((int)textSize.x + 40, (int)textSize.y + 40, BLACK);

        // Draw the text onto the image
        ImageDrawText(&image,text, 20, 20, 20, WHITE); // Offset by 20 to center text

        // Save the image to a file
        ExportImage(image, "output.png");

        // Unload image
        UnloadImage(image);

        // Unload font
        UnloadFont(font);

        // Close window
        CloseWindow();

}

