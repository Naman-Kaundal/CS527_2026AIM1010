#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <sys/select.h>
#include <unistd.h>
#endif
#include "os.h"
#include "compiler.h"
#include "memory.h"
#include "processor.h"

#define MAX_TASKS    64
#define TIME_SLICE   10   /* instructions given to a task per scheduler tick */

typedef struct {
    int  pid;
    int  proc_id;              /* -1 while the task is in the waiting queue */
    int  finished;
    char program_path[128];
    char data_path[128];
} Task;

static Task tasks[MAX_TASKS];
static int  task_count = 0;
static int  next_pid = 1;

static int  proc_busy[NP];
static int  shell_exit_requested = 0;

static void print_prompt(void)
{
    printf("minios> ");
    fflush(stdout);
}

static int find_free_processor(void)
{
    for (int p = 0; p < NP; p++) {
        if (!proc_busy[p]) return p;
    }
    return -1;
}

static void start_task_on_processor(Task *t, int proc_id)
{
    char bytecode_path[64];
    snprintf(bytecode_path, sizeof(bytecode_path), "compiled_%d.byte", proc_id);

    int valid_instructions = compile(t->program_path, bytecode_path);
    if (valid_instructions == 0) {
        printf("Error: '%s' has no recognizable instructions - task pid=%d was not started, and '%s' was left untouched.\n",
               t->program_path, t->pid, t->data_path);
        t->finished = 1;
        t->proc_id = -1;
        return;
    }

    initialize(proc_id, bytecode_path, t->data_path);
    reset(proc_id);

    proc_busy[proc_id] = 1;
    t->proc_id = proc_id;

    printf("Task pid=%d loaded on processor %d\n", t->pid, proc_id);
}

/* gives any task still waiting in the queue a processor, if one is free */
static void promote_waiting_tasks(void)
{
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].finished || tasks[i].proc_id != -1) continue;

        int p = find_free_processor();
        if (p == -1) return;

        start_task_on_processor(&tasks[i], p);
    }
}

int loader(const char *program_path, const char *data_path)
{
    if (task_count >= MAX_TASKS) {
        printf("Error: task table is full, cannot start %s\n", program_path);
        return -1;
    }

    Task *t = &tasks[task_count++];
    t->pid = next_pid++;
    t->finished = 0;
    t->proc_id = -1;
    strncpy(t->program_path, program_path, sizeof(t->program_path) - 1);
    t->program_path[sizeof(t->program_path) - 1] = '\0';
    strncpy(t->data_path, data_path, sizeof(t->data_path) - 1);
    t->data_path[sizeof(t->data_path) - 1] = '\0';

    int p = find_free_processor();
    if (p != -1) {
        start_task_on_processor(t, p);
    } else {
        printf("Task pid=%d queued, waiting for a free processor\n", t->pid);
    }

    return t->pid;
}

int map_pid_proc_id(int pid)
{
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].pid == pid && !tasks[i].finished) return tasks[i].proc_id;
    }
    return -1;
}

void scheduler(void)
{
    for (int i = 0; i < task_count; i++) {
        Task *t = &tasks[i];
        if (t->finished || t->proc_id == -1) continue;

        process_instructions(t->proc_id, TIME_SLICE);

        if (end_of_simulation[t->proc_id]) {
            finalize(t->proc_id, t->data_path);
            printf("Task pid=%d finished on processor %d\n", t->pid, t->proc_id);
            proc_busy[t->proc_id] = 0;
            t->finished = 1;
            t->proc_id = -1;
        }
    }

    promote_waiting_tasks();
    shell();
}

static int stdin_ready(void)
{
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) return 1;
    return _kbhit() != 0;
#else
    fd_set fds;
    struct timeval tv = { 0, 0 };

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}

void shell(void)
{
    if (shell_exit_requested) return;
    if (!stdin_ready()) return;

    char line[256];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        /* stdin closed, treat the same as typing exit */
        shell_exit_requested = 1;
        return;
    }

    line[strcspn(line, "\r\n")] = '\0';

    if (strcmp(line, "exit") == 0) {
        shell_exit_requested = 1;
        printf("Shell exiting, remaining tasks will keep running to completion.\n");
        return;
    }

    char program_path[128], data_path[128];
    if (sscanf(line, "%127s %127s", program_path, data_path) == 2) {
        loader(program_path, data_path);
    } else if (line[0] != '\0') {
        printf("Usage: <program.txt> <data.byte>\n");
    }

    print_prompt();
}

static int all_tasks_finished(void)
{
    for (int i = 0; i < task_count; i++) {
        if (!tasks[i].finished) return 0;
    }
    return 1;
}

void run_os(void)
{
    printf("Mini-OS is running. This is NOT your regular shell - only two things work here:\n");
    printf("  <program.txt> <data.byte>   start a new task\n");
    printf("  exit                        stop accepting new tasks (running ones finish normally)\n");
    print_prompt();

    while (!(shell_exit_requested && all_tasks_finished())) {
        scheduler();
    }

    printf("All tasks finished, shutting down.\n");
}
