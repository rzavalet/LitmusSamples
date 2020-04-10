#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <litmus.h>

#define PERIOD        ms2ns(50)
#define DEADLINE      ms2ns(20)
#define EXEC_COST     ms2ns(10)
#define MAJOR_CYCLE   ms2ns(200)

#define NUM_THREADS   (4)

struct thread_params
{
  unsigned short thr_num;

  int core;
  int rid;

	lt_t phase;
};

int I[NUM_THREADS] = {0};
struct thread_params tparams[NUM_THREADS] = {0};
pthread_t threads[NUM_THREADS];
pthread_attr_t attr;


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

int job(unsigned short thr_num)
{
#define MYSIZE  1000
#define LOOPS   1

  int j;
  int k;
  float A[MYSIZE];
  float B[MYSIZE];

  I[thr_num] ++;

  if (I[thr_num] >= 10000) {
    return 1;
  }

  
  fprintf(stderr, "Starting job: %d\n", thr_num); 

  for (j=0; j<MYSIZE; j++) {
    A[j] = 73.9 * (float) random() / 7.1;
    B[j] = 13.7 * (float) random() / 7.1;
  }

  for (k=0; k<LOOPS; k++) 
    for (j=0; j<MYSIZE; j++) {
      int k = MYSIZE * random();

      A[k] += A[k] * B[k] + A[(k + 8192) % MYSIZE] * B[(k + 8192) % MYSIZE];
    }

  return 0;
}


void *runThread(void *arg)
{

  int do_exit;
  lt_t cycle_length;
  lt_t slot_offset;
  lt_t now;
  lt_t next_cycle_start;
  struct rt_task params;
  struct thread_params *thr_paramP = arg;

  fprintf(stderr, "Starting thread: %u\n", thr_paramP->thr_num);

  CALL(init_rt_thread());

  init_rt_task_param(&params);
  params.exec_cost = EXEC_COST;
  params.period = PERIOD;
  params.relative_deadline = DEADLINE;
  params.cpu = thr_paramP->rid;
	params.phase = thr_paramP->phase;

  cycle_length = MAJOR_CYCLE;
  slot_offset = params.phase;

  CALL(set_rt_task_param(gettid(), &params));
  show_rt_param(&params);

  CALL(be_migrate_to_domain(thr_paramP->core));

  CALL(task_mode(LITMUS_RT_TASK));

  CALL(wait_for_ts_release());

  do {

    now = litmus_clock();
    next_cycle_start = ((now / cycle_length) + 1) * cycle_length;
    lt_sleep_until(next_cycle_start + slot_offset);

    //sleep_next_period();
    do_exit = job(thr_paramP->thr_num);

  } while(!do_exit);

  CALL(task_mode(BACKGROUND_TASK));

  pthread_exit(NULL);
}

int main()
{
  int i;
  int rc;
	void *status;

  CALL(init_litmus());

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // Run job here
  for (i=0; i<NUM_THREADS; i++) {

    struct thread_params *tparamP = &tparams[i];
    tparamP->thr_num = i;
    tparamP->core = i / 2;
    tparamP->rid = ((1 + tparamP->core) * 1000) + (i % 2);
		tparamP->phase = ms2ns(50 * (i % 2));

    rc = pthread_create(&threads[i], NULL, runThread, (void *)tparamP);
    if (rc != 0) {
      fprintf(stderr, "Failed to start thread %d\n", i);
    }
  }

	pthread_attr_destroy(&attr);
	for(i=0; i<NUM_THREADS; i++) {
		rc = pthread_join(threads[i], &status);
		if (rc != 0) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %d having a status of %ld\n", i, (long)status);
	}

  exit_litmus();

  return 0;

}
