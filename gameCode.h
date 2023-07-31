#pragma once

#define GAMECODE_API __declspec(dllexport)

#include <cinttypes>
#include "world.h"
#include "memory_arena.h"
#include "math.h"
#ifdef DEV_BUILD
#define Assert(expression) if(!(expression)){std::cout << "Assertion failure at " << __FUNCTION__ << "-" << __FILE__ << ":" << __LINE__ << std::endl; *(int*) 0 = 0;}
#else
#define Assert(expression)
#endif

#define LOG(p_string) std::cout << p_string << std::endl;

struct FileReadResult {
    uint64_t size;
    void* memory;
};

struct ThreadContext {
    int placeHolder;
};

#define READ_FILE(name) FileReadResult name(ThreadContext *thread, char* path)
typedef READ_FILE(readFile_t);

#define WRITE_FILE(name) bool name(ThreadContext *thread, char* path, void* content, uint64_t bytesToWrite)
typedef WRITE_FILE(writeFile_t);

#define APPEND_TO_FILE(name) bool name(ThreadContext *thread, char* path, void* content, uint64_t bytesToWrite)
typedef APPEND_TO_FILE(appendToFile_t);

#define FREE_FILE_MEMORY(name) void name(ThreadContext *thread, void* memory)
typedef FREE_FILE_MEMORY(freeFileMemory_t);

struct GameMemory{
    bool isinitialized;
    uint64_t transientStorageSize;
    void* transientStorage;
    uint64_t permanentStorageSize;
    void* permanentStorage;

    readFile_t *readFile;
    writeFile_t *writeFile;
    freeFileMemory_t *freeFileMemory;
    // TODO add append
};

struct Bitmap {
    uint32_t *startPixelPointer;
    int width;
    int height;
    Vector2 offset;
};

struct GameState{
    AbsoluteCoordinate playerCoord;
    Vector2 offsetInTile;
    Vector2 velocity;
    int orientation; // 0 back 1 front 2 left 3 right

    MemoryArena worldArena;
    World* world;

    Bitmap background;
    Bitmap guyHead[4];
    Bitmap guyCape[4];
    Bitmap guyTorso[4];
};

struct ButtonState{
    int halfTransitionCount;
    bool isDown;
};

struct AxisState{
    float xPosition;
    float yPosition;
};

struct GameInputState{
    float deltaTime;
    Vector2 mousePosition;
    union{
        ButtonState mouseButtons[2];
        ButtonState buttons[4];
        AxisState axis[2];
        struct {
            ButtonState Mouse_L;
            ButtonState Mouse_R;
            ButtonState A_Button;
            ButtonState B_Button;
            ButtonState X_Button;
            ButtonState Y_Button;
            AxisState Left_Stick;
            AxisState Right_Stick;
        } ;
    };
};

#define UPDATE_AND_RENDER(name) void name(ThreadContext *thread, GameMemory* gameMemory, uint32_t framesToWrite, void* bufferLocation, int samplesPerSec, void* bitmapMemory, int width, int height, GameInputState inputState)
typedef UPDATE_AND_RENDER(updateAndRender_t);

// TODO: handle multiple controllers:
/*
struct game_input {
  game_controller_input Controllers[4];
};
*/