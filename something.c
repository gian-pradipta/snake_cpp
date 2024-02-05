#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>

int get(char *tmp)
{
    char buf[20];
    ssize_t bytes;
    if ((bytes = read(STDIN_FILENO, buf, 20)) > 0) {
        *tmp = buf[0];
        return 0;
    }
    return -1;
}

int main(int argc, char const *argv[]) {
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    struct termios attr;
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    attr = old;
    attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
    int tmp;
    char input;
    while (1) {
        if ((tmp = get(&input)) == 0) {
            printf("%c\n", input);
            break;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;
}
