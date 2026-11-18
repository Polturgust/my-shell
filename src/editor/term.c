/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** term.c - raw terminal mode and terminal size
*/

#include <sys/ioctl.h>
#include <unistd.h>
#include "shell.h"

static struct termios *original(void)
{
    static struct termios saved;

    return &saved;
}

void term_raw(void)
{
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, original()) < 0)
        return;
    raw = *original();
    raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | INLCR);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &raw);
}

void term_restore(void)
{
    tcsetattr(STDIN_FILENO, TCSADRAIN, original());
}

int term_width(void)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
        return ws.ws_col;
    return 80;
}
