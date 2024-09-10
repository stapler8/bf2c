

#include <stdio.h>
#include <stdlib.h>
#include "loopstack.h"


char* openFile(char*);
void run(char*, long);


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("No file passed to compiler");
        exit(1);
    }

    char* fileBuffer = openFile(argv[1]);

    // determine how much memory to allocate for the program
    long cellCount;
    if (argc >= 3)
    {
        char* _;
        cellCount = strtol(fileBuffer, &_, 10);
    }

    // default to 1024 bytes
    else
        cellCount = 1024L;

    run(fileBuffer, cellCount);

    free(fileBuffer);
}

char* openFile(char* filename)
{
    // file variables
    FILE* file;
    long fileSize;
    char* fileBuffer;

    file = fopen(filename, "rb");

    // file not found
    if (!file)
    {
        printf("File not found");
        exit(1);
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // allocate memory for the file buffer
    fileBuffer = calloc(1, fileSize + 1);

    // handle calloc errors
    if (!fileBuffer)
    {
        fclose(file);
        printf("Memory allocation error");
        exit(1);
    }

    // handle read errors
    if (fread(fileBuffer, fileSize, 1, file) != 1)
    {
        fclose(file);
        free(fileBuffer);
        printf("File read error");
        exit(1);
    }

    return fileBuffer;
}

void run(char* buffer, long cellCount)
{
    // allocate memory for the program, cellCount amount of bytes
    char* cells = calloc(cellCount, sizeof(unsigned char));

    // pointers for the beginning and end of the program's memory
    char* start = cells;
    char* end = cells + cellCount - 1;

    // pointers to handle loops
    struct LoopStack* loopRoot = createRootNode();
    struct LoopStack* currentLoop = loopRoot;

    char* dataPointer = start;
    char* currentInstruction = &buffer[0];

    // start interpreting the program
    // break at EOF
    while (*currentInstruction != '\0')
    {
//        printf("DEBUG: %d\n", *currentInstruction);
        unsigned short inputBuffer = 0;
        switch (*currentInstruction)
        {
            // increment byte
            case '+':
                *dataPointer += 1;
                break;

            // decrement byte
            case '-':
                *dataPointer -= 1;
                break;

            // increment pointer
            case '>':
                dataPointer++;
                break;

            // decrement pointer
            case '<':
                dataPointer--;
                break;

            // print byte
            case '.':
                printf("%c", *dataPointer);
                break;

            // read byte
            case ',':
                scanf("%hd", &inputBuffer);
                *dataPointer = inputBuffer;
                break;

            // create a new loop
            case '[':
                // if conditional is unmet, skip past loop
                if (*dataPointer == 0)
                {
                    while (*currentInstruction != ']')
                        currentInstruction++;

                    continue;
                }

                // add new loop to the stack
                currentLoop = pushLoop(currentLoop, currentInstruction);
                break;

            // end of loop
            case ']':
                // finish loop if conditional is met
                if (*dataPointer == 0) {
                    popLoop(currentLoop);
                    break;
                }

                // jump back to start of loop
                currentInstruction = currentLoop->loopStart;
                break;
        }
        // finished interpreting instructions

        currentInstruction++;
    }

    free(cells);

}
