/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** shell.h - every type and prototype shared by the shell
*/

#ifndef SHELL_H_
    #define SHELL_H_
    #include <stdbool.h>
    #include <stddef.h>
    #include <sys/types.h>
    #include <termios.h>
    #include <time.h>
    #include "variables.h"

    #define SHELL_NAME "42sh"
    #define DEFAULT_PATH "/usr/local/bin:/usr/bin:/bin"
    #define HISTORY_FILE ".42sh_history"
    #define HISTORY_MAX 1000
    #define ALIAS_DEPTH_MAX 20

/* ---------------------------------------------------------------- utils */

typedef struct str_s {
    char *data;
    size_t len;
    size_t cap;
} str_t;

typedef struct vec_s {
    char **items;
    size_t len;
    size_t cap;
} vec_t;

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
char *xstrdup(const char *s);
char *xstrndup(const char *s, size_t n);
void str_init(str_t *s);
void str_push(str_t *s, char c);
void str_append(str_t *s, const char *text);
void str_appendn(str_t *s, const char *text, size_t n);
void str_insert(str_t *s, size_t pos, const char *text, size_t n);
void str_erase(str_t *s, size_t pos, size_t n);
void str_set(str_t *s, const char *text);
char *str_take(str_t *s);
void str_free(str_t *s);
void vec_init(vec_t *v);
void vec_push(vec_t *v, char *item);
void vec_free(vec_t *v);
void vec_sort(vec_t *v);
bool is_name_char(char c, bool first);
char *path_join(const char *dir, const char *name);
int print_error(const char *fmt, ...);
int print_notice(const char *fmt, ...);
bool should_abort(void);

/* ------------------------------------------------------------- environment */

void env_init(char **environ_src);
const char *env_get(const char *key);
void env_set(const char *key, const char *value);
void env_unset(const char *key);
char **env_array(void);
void env_free(void);

/* ---------------------------------------------------------------- lexer */

typedef enum tok_type_e {
    T_WORD,
    T_PIPE,
    T_AND,
    T_OR,
    T_SEMI,
    T_BG,
    T_LPAREN,
    T_RPAREN,
    T_REDIR,
    T_END
} tok_type_t;

typedef enum redir_type_e {
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC,
    R_OUT_ERR,
    R_APPEND_ERR,
    R_ERR,
    R_ERR_APPEND,
    R_ERR_TO_OUT
} redir_type_t;

typedef struct token_s {
    tok_type_t type;
    redir_type_t redir;
    char *text;
    bool quoted;
    int alias_depth;
} token_t;

typedef struct tokens_s {
    token_t *items;
    size_t len;
    size_t cap;
} tokens_t;

int lex_line(const char *line, tokens_t *out);
void tokens_push(tokens_t *t, token_t tok);
void tokens_free(tokens_t *t);
int scan_word_end(const char *line, size_t *i, bool *quoted);

/* ------------------------------------------------------------------ AST */

typedef enum node_type_e {
    N_CMD,
    N_SUBSHELL,
    N_PIPE,
    N_AND,
    N_OR,
    N_SEQ,
    N_BG
} node_type_t;

typedef struct redir_s {
    redir_type_t type;
    char *target;
    char *body;
    bool quoted;
    struct redir_s *next;
} redir_t;

typedef struct node_s {
    node_type_t type;
    vec_t argv;
    redir_t *redirs;
    struct node_s *left;
    struct node_s *right;
    struct node_s **kids;
    size_t nkids;
    char *text;
} node_t;

typedef struct parser_s {
    tokens_t *toks;
    size_t pos;
    bool error;
} parser_t;

node_t *parse_line(const char *line, tokens_t *toks, bool *error);
node_t *parse_list(parser_t *p);
node_t *parse_command(parser_t *p);
node_t *parse_error(parser_t *p, const char *msg, node_t *partial);
token_t *peek(parser_t *p);
node_t *node_new(node_type_t type);
void node_free(node_t *node);
char *node_to_text(const node_t *node);
int read_heredocs(node_t *node);
int expand_alias_at(parser_t *p);

/* ------------------------------------------------------------ expansion */

int expand_word(const char *word, vec_t *out);
int expand_words(const vec_t *words, vec_t *out);
char *expand_single(const char *word);
char *expand_heredoc(const char *body);
int expand_dollar(const char *s, size_t *i, str_t *out);
char *command_substitution(const char *cmd);
char *expand_tilde(const char *word, size_t *consumed);

/* ------------------------------------------------------------ execution */

typedef struct shell_s {
    bool interactive;
    bool job_control;
    bool in_child;
    bool do_exit;
    bool warned_jobs;
    int status;
    pid_t pgid;
    struct termios tmodes;
    bool has_tmodes;
    var_t *vars;
    void *input;
    bool abort_on_error;
    bool error_raised;
} shell_t;

shell_t *sh(void);
int run_line(const char *line);
int run_file(const char *path, bool quiet_missing);
int exec_node(node_t *node);
int exec_pipeline(node_t **cmds, size_t n, const char *text, bool background);
void exec_in_child(node_t *cmd);
int exec_simple(node_t *cmd);
int launch_single(node_t *cmd, char **argv, char *path, const char *text);
void exec_failed(const char *name);
char *find_command(const char *name);
int apply_redirs(redir_t *redirs);
int save_fds(int saved[3]);
void restore_fds(int saved[3]);
int status_from_wait(int wstatus, bool report);
void reset_child_signals(void);
void set_status(int status);

/* ----------------------------------------------------------------- jobs */

typedef struct proc_s {
    pid_t pid;
    bool done;
    bool stopped;
    int wstatus;
} proc_t;

typedef struct job_s {
    int id;
    pid_t pgid;
    proc_t *procs;
    size_t nprocs;
    char *text;
    bool background;
    struct job_s *next;
} job_t;

job_t *job_new(pid_t pgid, const char *text, bool background);
void job_add_proc(job_t *job, pid_t pid);
int job_wait(job_t *job);
int job_foreground(job_t *job, bool cont);
void job_background(job_t *job, bool cont);
void job_register(job_t *job);
void job_remove(job_t *job);
void job_free(job_t *job);
void jobs_reap(bool report);
bool job_is_stopped(job_t *job);
bool job_is_done(job_t *job);
job_t *job_find(const char *spec);
job_t **jobs_head(void);
bool jobs_any_stopped(void);
void jobs_init(void);

/* ------------------------------------------------------------- builtins */

typedef int (*builtin_fn_t)(char **argv);

builtin_fn_t builtin_find(const char *name);
const char **builtin_names(void);
int builtin_cd(char **argv);
int builtin_echo(char **argv);
int builtin_env(char **argv);
int builtin_setenv(char **argv);
int builtin_unsetenv(char **argv);
int builtin_exit(char **argv);
int builtin_alias(char **argv);
int builtin_unalias(char **argv);
int builtin_history(char **argv);
int builtin_set(char **argv);
int builtin_unset(char **argv);
int builtin_source(char **argv);
int builtin_which(char **argv);
int builtin_where(char **argv);
int builtin_jobs(char **argv);
int builtin_fg(char **argv);
int builtin_bg(char **argv);
int builtin_builtins(char **argv);
int builtin_repeat(char **argv);
void run_alias_hook(const char *name);

/* -------------------------------------------------------------- aliases */

const char *alias_get(const char *name);
void alias_set(const char *name, const char *value);
void alias_unset(const char *name);
vec_t *alias_names(void);
void alias_free(void);

/* -------------------------------------------------------------- history */

typedef struct hist_entry_s {
    int id;
    time_t time;
    char *cmd;
} hist_entry_t;

void history_add(const char *cmd);
size_t history_len(void);
const hist_entry_t *history_at(size_t index);
void history_clear(void);
void history_load(void);
void history_save(void);
int history_expand(const char *line, char **out);

/* --------------------------------------------------------------- editor */

char *read_input_line(const char *prompt, bool *eof);
char *editor_read(const char *prompt, bool *eof);
void print_prompt_header(void);
void term_raw(void);
void term_restore(void);
int term_width(void);

#endif /* SHELL_H_ */
