#include "../process/process.h"

extern int next_sch_pid, curr_sch_pid;

extern process_t *next_process;

void scheduler_init();
process_t *get_next_process();
void scheduler(int, int, int, int, int, int, int, int, int);
void run_next_process();