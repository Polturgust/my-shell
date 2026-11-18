/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** jobs.c - jobs, fg, bg
*/

#include <stdio.h>
#include "shell.h"

int builtin_jobs(char **argv)
{
    job_t *last = job_find(NULL);

    (void)argv;
    jobs_reap(true);
    for (job_t *j = *jobs_head(); j; j = j->next)
        dprintf(1, "[%d]  %c %-29s %s\n", j->id, j == last ? '+' : ' ',
            job_is_stopped(j) ? "Suspended" : "Running", j->text);
    return 0;
}

static job_t *get_job(char **argv, const char *who)
{
    job_t *job = job_find(argv[1]);

    if (!job)
        print_error(argv[1] ? "%s: No such job.\n" : "%s: No current job.\n",
            argv[1] ? argv[1] : who);
    return job;
}

int builtin_fg(char **argv)
{
    job_t *job = get_job(argv, "fg");

    if (!job)
        return 1;
    if (!sh()->job_control)
        return print_error("fg: No job control in this shell.\n");
    dprintf(1, "%s\n", job->text);
    return job_foreground(job, true);
}

int builtin_bg(char **argv)
{
    job_t *job = get_job(argv, "bg");

    if (!job)
        return 1;
    if (!sh()->job_control)
        return print_error("bg: No job control in this shell.\n");
    dprintf(1, "[%d]    %s &\n", job->id, job->text);
    job_background(job, true);
    return 0;
}
