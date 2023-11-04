


#ifndef DATA_LOGGER_H_
#define DATA_LOGGER_H_

void Data_Logger_Init(void);


void Data_Logger_Runnable(void);

void adc_read_chain(void);

void set_ldr(void);

void set_temp(void);

void LDR_LED(void);

#define TEMP_CH CH_1

#define LDR_CH  CH_0


#endif /* DATA_LOGGER_H_ */