/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** pipeline.c - fork every stage of a pipeline into one job
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

typedef struct stage_s {
    node_t *node;
    char **argv;
    char *path;
} stage_t;

static void child_setup(job_t *job, bool background)
{
    if (sh()->job_control) {
        setpgid(0, job->pgid);
        if (!background)
            tcsetpgrp(STDIN_FILENO, getpgrp());
    }
    reset_child_signals();
    sh()->in_child = true;
    sh()->job_control = false;
    sh()->interactive = false;
    sh()->abort_on_error = true;
    sh()->error_raised = false;
}

static void run_stage(stage_t *stage)
{
    if (!stage->argv)
        exec_in_child(stage->node);
    if (apply_redirs(stage->node->redirs) < 0)
        exit(1);
    execve(stage->path, stage->argv, env_array());
    exec_failed(stage->argv[0]);
}

static void fork_stage(stage_t *stage, job_t *job, int io[3], bool bg)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        child_setup(job, bg);
        if (io[0] != -1 && dup2(io[0], STDIN_FILENO) >= 0)
            close(io[0]);
        if (io[1] != -1) {
            close(io[2]);
            dup2(io[1], STDOUT_FILENO);
            close(io[1]);
        }
        run_stage(stage);
    }
    if (sh()->job_control) {
        job->pgid = job->pgid ? job->pgid : pid;
        setpgid(pid, job->pgid);
    }
    job_add_proc(job, pid);
}

static int launch(stage_t *stages, size_t n, const char *text, bool bg)
{
    job_t *job = job_new(0, text, bg);
    int prev = -1;
    int pipefd[2];

    for (size_t i = 0; i < n; i++) {
        pipefd[0] = -1;
        pipefd[1] = -1;
        if (i + 1 < n && pipe(pipefd) < 0)
            perror("pipe");
        fork_stage(&stages[i], job, (int[3]){prev, pipefd[1], pipefd[0]}, bg);
        if (prev != -1)
            close(prev);
        if (pipefd[1] != -1)
            close(pipefd[1]);
        prev = pipefd[0];
    }
    if (!bg)
        return job_foreground(job, false);
    job_register(job);
    if (sh()->interactive)
        dprintf(STDOUT_FILENO, "[%d] %d\n", job->id, (int)job->procs[0].pid);
    return 0;
}

int exec_pipeline(node_t **cmds, size_t n, const char *text, bool background)
{
    stage_t *stages = NULL;
    int status = 0;

    if (n == 1 && cmds[0]->type == N_CMD && !background)
        return exec_simple(cmds[0]);
    stages = xmalloc(sizeof(stage_t) * n);
    for (size_t i = 0; i < n; i++)
        stages[i] = (stage_t){cmds[i], NULL, NULL};
    status = launch(stages, n, text, background);
    free(stages);
    return status;
}

int launch_single(node_t *cmd, char **argv, char *path, const char *text)
{
    stage_t stage = {cmd, argv, path};

    return launch(&stage, 1, text, false);
}
