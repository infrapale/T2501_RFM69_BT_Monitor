#ifndef __LOGGER_H__
#define __LOGGER_H__

#define LOGGER_DATA_LEN  80

typedef struct
{
    char msg_buff[LOGGER_DATA_LEN];
    bool new_msg_in_buff;
    char file_name[32];
    bool sd_ok;
} logger_st;


void logger_initialize(void);

void logger_add_msg(char *new_msg);

void logger_directory(void);

#endif