#ifndef OS_H
#define OS_H

/*
 * loader()
 * Called by the shell and registers a new task under a fresh PID and, if a
 * processor is free, compiles and loads it straight away; otherwise the
 * task sits in the waiting queue until scheduler() frees one up. Returns
 * the new task's PID.
 */
int loader(const char *program_path, const char *data_path);

/*
 * scheduler()
 * One round-robin tick: gives every task currently holding a processor a
 * 10-instruction time slice, reclaims the processor and writes the final
 * data.byte for any task that just finished, hands a freed processor to a
 * waiting task if there is one, then lets the shell check for new input.
 */
void scheduler(void);

/*
 * shell()
 * Non-blocking check for a line typed at the terminal. A blank prompt
 * with nothing typed yet returns immediately without blocking the rest
 * of the OS.
 */
void shell(void);

/*
 * map_pid_proc_id()
 * Returns the processor a PID is currently running on, or -1 if that
 * task is still in the waiting queue or does not exist.
 */
int map_pid_proc_id(int pid);

/*
 * run_os()
 * Repeatedly calls scheduler() until the shell has been told to exit and
 * every task (ready or waiting) has finished.
 */
void run_os(void);

#endif
