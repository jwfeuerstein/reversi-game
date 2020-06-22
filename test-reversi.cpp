#include <iostream>
#include <cstdlib>

#include "reversi.h"

using namespace std;

int main(int argc, char *argv[])
{
    size_t size = 4;
    if (argc >= 2)
    {
        size = atoi(argv[1]);
        if (((size % 2) == 1) ||
            ((size < 4) || (size > 26)))
        {
            cout << "Invalid size" << endl;
            return 1;
        }
        else
        {
            size = atoi(argv[1]);
        }
    }
    Reversi game(size);
    game.play();
    return 0;
}
