/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** job.c - a job is every process started by one pipeline
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

job_t **jobs_head(void)
{
    static job_t *head = NULL;

    return &head;
}

job_t *job_new(pid_t pgid, const char *text, bool background)
{
    job_t *job = xmalloc(sizeof(job_t));

    memset(job, 0, sizeof(job_t));
    job->pgid = pgid;
    job->text = xstrdup(text ? text : "");
    job->background = background;
    return job;
}

void job_add_proc(job_t *job, pid_t pid)
{
    job->procs = xrealloc(job->procs, sizeof(proc_t) * (job->nprocs + 1));
    job->procs[job->nprocs] = (proc_t){pid, false, false, 0};
    job->nprocs++;
}

void job_free(job_t *job)
{
    if (!job)
        return;
    free(job->procs);
    free(job->text);
    free(job);
}

void job_register(job_t *job)
{
    job_t **tail = jobs_head();
    int id = 1;

    if (job->id)
        return;
    for (job_t *j = *jobs_head(); j; j = j->next)
        id = j->id >= id ? j->id + 1 : id;
    job->id = id;
    while (*tail)
        tail = &(*tail)->next;
    job->next = NULL;
    *tail = job;
}
