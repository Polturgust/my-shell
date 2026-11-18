/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** job2.c - job list queries
*/

#include <stdlib.h>
#include <string.h>
#include "shell.h"

void job_remove(job_t *job)
{
    job_t **cur = jobs_head();

    while (*cur && *cur != job)
        cur = &(*cur)->next;
    if (*cur)
        *cur = job->next;
    job->next = NULL;
    job->id = 0;
}

bool job_is_stopped(job_t *job)
{
    bool any_stopped = false;

    for (size_t i = 0; i < job->nprocs; i++) {
        if (!job->procs[i].done && !job->procs[i].stopped)
            return false;
        any_stopped = any_stopped || job->procs[i].stopped;
    }
    return any_stopped;
}

bool job_is_done(job_t *job)
{
    for (size_t i = 0; i < job->nprocs; i++)
        if (!job->procs[i].done)
            return false;
    return true;
}

bool jobs_any_stopped(void)
{
    for (job_t *j = *jobs_head(); j; j = j->next)
        if (job_is_stopped(j))
            return true;
    return false;
}

job_t *job_find(const char *spec)
{
    job_t *last = NULL;
    int id = 0;

    if (spec && spec[0] == '%')
        spec++;
    if (spec && *spec && strcmp(spec, "+") != 0 && strcmp(spec, "%") != 0) {
        id = atoi(spec);
        for (job_t *j = *jobs_head(); j; j = j->next)
            if (j->id == id || (id == 0 && strncmp(j->text, spec,
                strlen(spec)) == 0))
                return j;
        return NULL;
    }
    for (job_t *j = *jobs_head(); j; j = j->next)
        last = j;
    return last;
}
