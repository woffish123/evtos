#ifndef LOG_BYTE_ANNOUCE  // Log byte define 
#define LOG_BYTE_ANNOUCE
typedef enum LOGBYTE_
{
    log_nothing  = 0 ,  // no log .
    log_cfgdata_error,    
    log_tmdata_error,
    log_tmevent_no,
    log_rtc_int,

}LogByte ;
// logbyte  is a uint8_t var , max value is 255 ,not  bigger than it .


#endif
