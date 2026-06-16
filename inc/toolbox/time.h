#ifndef TOOLKIT_TIME_H
#define TOOLKIT_TIME_H




///////////////
// time.c
int64_t micros(void);
void micros_block_for(int64_t u);
void micros_busy_for(int64_t u);
void micros_test(void);




#endif
