// ffmpeg-samples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FileObject.h"

void print_usage()
{
    printf("Usage: ffmpeg-samples.exe [file_name] [type].\n");
}

int main(int argc, char** argv)
{
    try
    {
        std::string type;
        const char* url{};
        if (argc >= 2) {
            url = argv[1];
            if (argc >= 3) {                
                type = argv[2];
                if (!(type == "v" ||
                    type == "video" ||
                    type == "a" ||
                    type == "audio"))
                {
                    print_usage();
                    return 0;
                }
            }
        }
        else
        {
            print_usage();
            return 0;
        }
        FileObject obj(url, type);
        obj.ShowStreamInfo();
        obj.ReadFrames();
    }
    catch (const std::exception& e)
    {
        printf("%s\n", e.what());
    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
