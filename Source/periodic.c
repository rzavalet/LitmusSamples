#include <stdio.h>
#include <litmus.h>

#define PERIOD        ms2ns(1000)
#define DEADLINE      ms2ns(1000)
#define EXEC_COST     ms2ns(50)

#define CALL( exp )                               \
  do {                                            \
    int ret;                                      \
    ret = exp;                                    \
    if (ret != 0) {                               \
      fprintf(stderr, "%s failed: %m\n", #exp);   \
    }                                             \
    else {                                        \
      fprintf(stderr, "%s ok: %m\n", #exp);       \
    }                                             \
  } while (0)

int i = 0;

int job()
{
  i ++;
  if (i >= 1000) {
    return 1;
  }

  return 0;
}

int main()
{
  int do_exit;
  struct rt_task params;

  CALL(init_litmus());

  init_rt_task_param(&params);
  params.exec_cost = EXEC_COST;
  params.period = PERIOD;
  params.relative_deadline = DEADLINE;

  CALL(set_rt_task_param(gettid(), &params));

  CALL(task_mode(LITMUS_RT_TASK));

  CALL(wait_for_ts_release());

  do {
    sleep_next_period();
    do_exit = job();
  } while(!do_exit);

  CALL(task_mode(BACKGROUND_TASK));

  return 0;

}
