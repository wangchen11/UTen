#ifndef SELECT_TIME_OUT_MS
    #define SELECT_TIME_OUT_MS (2*1000) // ALL INTERVAL/STEP/TIMEOUT is multiple of this in runtime.
#endif

#define MTU                1500
#define MAX_PKG_SIZE (MTU-8-20-8) // MTU - 36
#define BUFFER_MAX   (MAX_PKG_SIZE*2)

#define KEEP_CONNECTION_INTERVAL_MS (10*1000)
#define KEEP_CONNECTION_TIME_OUT_MS (KEEP_CONNECTION_INTERVAL_MS*4)

#define RECYCLE_AFTER_MS (KEEP_CONNECTION_TIME_OUT_MS)
#define RECYCLE_STEP_MS  (RECYCLE_AFTER_MS/2)