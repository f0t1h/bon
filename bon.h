#ifndef BON_HEAD
#define BON_HEAD
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#define i32 int32_t 
#include <threads.h>


int print(void);
typedef struct batch_settings{
	i32 nodes;
	i32 time;
	char *partition;
	i32 ntasks;
	char *jobname;
	char *output;
} batch_settings;

typedef struct job {
	char *shell;
	bool completed;
	struct job *depends;
	size_t size;
	size_t cap;
} job;

char *stdup(const char *c){
	size_t len = strlen(c);
	char *buf = malloc(len+1);
	strncpy(buf,c,len+1);
	return buf;
}
#define INIT_DEP_CAP 4
#define MAKE_JOB(she) (job) {(she), false, malloc(sizeof(INIT_DEP_CAP) * sizeof(job)), 0, INIT_DEP_CAP}
#define MAKE_JOBR(she) &MAKE_JOB(stdup(she))
#define EXPAND(JOB) do{	\
	(JOB)->cap *=2;	\
	if(realloc((JOB)->depends, (JOB)->cap) == NULL){\
		exit(1);	\
	}			\
	}while(0)	
#define DEPENDS(X, Y) do{			\
	if((X)->size == (X)->cap ){		\
		EXPAND(X);			\
	}					\
	memcpy(&(X)->depends[(X)->size++], (Y), sizeof(struct job));\
}while (0)


#define CHAIN(X,Y) DEPENDS((X), (Y)); (X) = &(X)->depends[(X)->size-1]
#define CHAINS(X,sh) CHAIN(X,MAKE_JOBR(sh))
#define BEGIN_CHAIN(X) do{ \
	job *(X)
#define END_CHAIN(X) }while(0)

int runjob(job *);
#endif 
#ifdef BON_IMPL
#ifndef BON_IMPL_ONCE
#define BON_IMPL_ONCE
#include <stdio.h>
#include <stdlib.h>

int
print(void) {
	printf("Hellow, orld!\n");
	return 0;
}

int runjobv( void *v){
	return runjob(v);
}
int 
runjob( job *j){
	thrd_t *threads = malloc(sizeof(thrd_t) * j->size);
	for(int i =0; i < j->size;++i){
		thrd_create(&threads[i], runjobv, (&(j->depends[i])));
	}
	for(int i =0; i < j->size;++i){
		int result = 0;
		thrd_join(threads[i], &result);
		if(result != 0){
			exit (result);
		}
	}
	return system(j->shell);
}



#endif
#endif
