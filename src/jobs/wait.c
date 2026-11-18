/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** wait.c - wait for jobs, hand them the terminal, reap finished ones
*/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static void update_proc(proc_t *proc, int wstatus)
{
    if (WIFSTOPPED(wstatus)) {
        proc->stopped = true;
        return;
    }
    if (WIFCONTINUED(wstatus)) {
        proc->stopped = false;
        return;
    }
    proc->done = true;
    proc->stopped = false;
    proc->wstatus = wstatus;
}

static int job_status(job_t *job)
{
    int status = 0;
    int one = 0;
    int ws = 0;

    for (size_t i = 0; i < job->nprocs; i++) {
        ws = job->procs[i].wstatus;
        one = status_from_wait(ws, !(WIFSIGNALED(ws)
            && WTERMSIG(ws) == SIGPIPE && i + 1 < job->nprocs));
        status = one ? one : status;
    }
    return status;
}

int job_wait(job_t *job)
{
    int wstatus = 0;
    pid_t ret = 0;

    for (size_t i = 0; i < job->nprocs; i++) {
        if (job->procs[i].done)
            continue;
        do
            ret = waitpid(job->procs[i].pid, &wstatus, WUNTRACED);
        while (ret < 0 && errno == EINTR);
        if (ret < 0)
            job->procs[i].done = true;
        else
            update_proc(&job->procs[i], wstatus);
    }
    if (job_is_stopped(job))
        return 1;
    return job_status(job);
}

int job_foreground(job_t *job, bool cont)
{
    int status = 0;

    if (sh()->job_control && job->pgid)
        tcsetpgrp(STDIN_FILENO, job->pgid);
    if (cont) {
        for (size_t i = 0; i < job->nprocs; i++)
            job->procs[i].stopped = false;
        kill(-job->pgid, SIGCONT);
    }
    status = job_wait(job);
    if (sh()->job_control) {
        tcsetpgrp(STDIN_FILENO, sh()->pgid);
        if (sh()->has_tmodes)
            tcsetattr(STDIN_FILENO, TCSADRAIN, &sh()->tmodes);
    }
    if (job_is_stopped(job)) {
        job->background = false;
        job_register(job);
        dprintf(STDOUT_FILENO, "\nSuspended\n");
        return status;
    }
    if (job->id)
        job_remove(job);
    job_free(job);
    return status;
}

void job_background(job_t *job, bool cont)
{
    job->background = true;
    if (cont) {
        for (size_t i = 0; i < job->nprocs; i++)
            job->procs[i].stopped = false;
        kill(-job->pgid, SIGCONT);
    }
}

void jobs_reap(bool report)
{
    job_t *next = NULL;
    int wstatus = 0;

    for (job_t *j = *jobs_head(); j; j = next) {
        next = j->next;
        for (size_t i = 0; i < j->nprocs; i++)
            if (!j->procs[i].done && waitpid(j->procs[i].pid, &wstatus,
                WNOHANG | WUNTRACED | WCONTINUED) > 0)
                update_proc(&j->procs[i], wstatus);
        if (!job_is_done(j))
            continue;
        if (report)
            dprintf(STDOUT_FILENO, "[%d]    Done                          %s\n",
                j->id, j->text);
        job_remove(j);
        job_free(j);
    }
}
