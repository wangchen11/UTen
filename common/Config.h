#define SELECT_TIME_OUT_MS (5*1000)
#define RECYCLE_STEP_MS    (8*1000) // + [0-SELECT_TIME_OUT_MS]
#define RECYCLE_AFTER_SEC  10       // + [0-RECYCLE_AFTER_SEC]
#define MTU                1500
#define MAX_PKG_SIZE (MTU-8-20-8) // MTU - 36
#define BUFFER_MAX   (MAX_PKG_SIZE*2)