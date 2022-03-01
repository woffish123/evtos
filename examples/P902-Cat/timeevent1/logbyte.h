#ifndef LOG_BYTE_ANNOUCE  // Log byte define 
#define LOG_BYTE_ANNOUCE
// reserve byte for differ the cmd and logbyte .

// header char and End char to check if a valid cmd .
#define CmdHeader   '<'
#define CmdEnder    '>'

//#define CmdRetHeader  '['
//#define CmdRetEnder   ']'

typedef enum LOGBYTE_
{
    log_nothing  = 0 ,  // no log .
    log_cfgdata_error,    
    log_tmdata_error,
    log_tmevent_no,
    log_proc_start,
    log_rtc_int,
    log_rf_coldup,
    log_rf_recv_data,
    log_rf_register,
    log_rf_regist_no_reply,
    log_rf_jioning,
    log_rf_jionfail,
    log_rf_asktime,
    log_rf_asktimefail,
    log_rsv1 = CmdHeader,
    
    log_rsv2 = CmdEnder,
    log_test = 0x80,
    log_test1,
    log_test2,
    log_test3,
    log_test4,
}LogByte ;
// logbyte  is a uint8_t var , max value is 255 ,not  bigger than it .


#endif
