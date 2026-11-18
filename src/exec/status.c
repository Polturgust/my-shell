/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** status.c - exit statuses, signal messages and child signal setup
*/

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static const char *signal_message(int sig)
{
    switch (sig) {
    case SIGSEGV:
        return "Segmentation fault";
    case SIGFPE:
        return "Floating exception";
    case SIGBUS:
        return "Bus error";
    case SIGABRT:
        return "Abort";
    case SIGKILL:
        return "Killed";
    case SIGTERM:
        return "Terminated";
    case SIGQUIT:
        return "Quit";
    case SIGILL:
        return "Illegal instruction";
    default:
        return NULL;
    }
}

int status_from_wait(int wstatus, bool report)
{
    int sig = 0;
    const char *msg = NULL;

    if (WIFEXITED(wstatus))
        return WEXITSTATUS(wstatus);
    if (!WIFSIGNALED(wstatus))
        return 1;
    sig = WTERMSIG(wstatus);
    msg = signal_message(sig);
    if (report && msg)
        print_error("%s%s\n", msg, WCOREDUMP(wstatus) ? " (core dumped)" : "");
    if (report && sig == SIGINT && sh()->interactive)
        write(STDOUT_FILENO, "\n", 1);
    return 128 + sig;
}

void reset_child_signals(void)
{
    int sigs[] = {SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU, SIGCHLD, 0};

    for (int i = 0; sigs[i]; i++)
        signal(sigs[i], SIG_DFL);
}

void set_status(int status)
{
    char text[16];

    sh()->status = status;
    snprintf(text, sizeof(text), "%d", status);
    set_variable(&sh()->vars, "status", text);
}
