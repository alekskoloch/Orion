#pragma once

#include "../pch.h"

class ProceduralGenerationSystem
{
public:
    //initialize procedural generator system with seed as parameter
    static void Initialize(int seed);
    //get number of any range
    static int GetRandomNumber(int min, int max);
private:
    //private static variables for procedural generator
    
};