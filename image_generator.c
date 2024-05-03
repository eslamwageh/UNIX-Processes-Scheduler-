    #include "raylib.h"
    #include <stdio.h>
    #include <string.h>

    #define MAX_TEXT_LENGTH 5000

    int main(void)
    {
        // Read text from file
        FILE *file = fopen("input.txt", "r");
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

        return 0;
    }
