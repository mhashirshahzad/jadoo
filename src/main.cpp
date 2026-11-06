#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "game.hpp"


int main(int argc, char **argv) {
    gameLoop();
    return 0;
}
