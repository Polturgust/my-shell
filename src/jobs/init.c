/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** init.c - take control of the terminal when interactive
*/

#include <signal.h>
#include <unistd.h>
#include "shell.h"

static void ignore_job_signals(void)
{
    int sigs[] = {SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU, 0};

    for (int i = 0; sigs[i]; i++)
        signal(sigs[i], SIG_IGN);
}

void jobs_init(void)
{
    shell_t *s = sh();
    pid_t pgid = 0;

    if (!s->interactive)
        return;
    while (tcgetpgrp(STDIN_FILENO) != (pgid = getpgrp())) {
        if (tcgetpgrp(STDIN_FILENO) < 0)
            break;
        kill(-pgid, SIGTTIN);
    }
    ignore_job_signals();
    if (getpid() != getpgrp())
        setpgid(0, 0);
    s->pgid = getpgrp();
    s->job_control = tcsetpgrp(STDIN_FILENO, s->pgid) == 0;
    s->has_tmodes = tcgetattr(STDIN_FILENO, &s->tmodes) == 0;
}
