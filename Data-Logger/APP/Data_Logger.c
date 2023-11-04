
#include "StdTypes.h"
#include "LCD_Interface.h"
#include "DIO_Interface.h"
#include "ADC_Interfaceh.h"
#include "EX_Interrupt_Interface.h"
#include "Data_Logger.h"
#include "Utils.h"
#include "Sensors_Interface.h"




volatile ADC_Channel_type channel =CH_0;

volatile u8 flag_adc_ldr=0;
volatile u8 flag_adc_lm35=1;

volatile u16 ADC_READ_ldr=0;
volatile u16 TEMP_READ=0;


void Data_Logger_Init(void)
{
	DIO_Init();
	LCD_Init();
	//KEYPAD_Init();
	ADC_SetCallback(adc_read_chain);
	GIE_voidEnable();
	EXI_SetCallBack(EX_INT0,set_ldr);
	EXI_SetCallBack(EX_INT1,set_temp);
	EXI_TriggerEdge(EX_INT0,RISING_EDGE);
	EXI_Enable(EX_INT0);
	EXI_TriggerEdge(EX_INT1,RISING_EDGE);
	EXI_Enable(EX_INT1);
	
	ADC_Init(VREF_AVCC,ADC_SCALER_128);
	ADC_Chain(LDR_CH);
	
	
}




void Data_Logger_Runnable(void)
{
	DIO_WritePin(PINC6,LOW);
	DIO_WritePin(PINC7,LOW);
	LCD_GoTo(3,0);
	LCD_WriteString("EX_INT Source:");
	LCD_WriteString("           ");
	LCD_GoTo(4,15);
	LCD_WriteString("           ");
	if ((TEMP_READ/10)>25)
	{
		DIO_WritePin(PIND2,HIGH);
		_delay_ms(1);
		DIO_WritePin(PIND2,LOW);
		LCD_GoTo(4,15);
		LCD_WriteString("TEMP");
		
	}
	if ((ADC_READ_ldr)>400)
	{
		DIO_WritePin(PIND3,HIGH);
		_delay_ms(1);
		DIO_WritePin(PIND3,LOW);
		LCD_GoTo(3,15);
		LCD_WriteString("LDR");
	}
	
	if (flag_adc_lm35)
	{
		
		
		LCD_GoTo(1,0);
		LCD_WriteString("Temp:");
		LCD_GoTo(1,5);
		LCD_WriteString("      ");
		LCD_GoTo(1,5);
		LCD_WriteNumber(TEMP_READ/10);
		LCD_WriteChar('.');
		LCD_WriteNumber(TEMP_READ%10);
		LCD_WriteString("      ");
		
		
		flag_adc_lm35=0;
	}
	
	if (flag_adc_ldr)
	{
		
		
		LCD_GoTo(2,0);
		LCD_WriteString("LDR:");
		LCD_WriteNumber(ADC_READ_ldr);
		LCD_WriteString("                ");
		LDR_LED();
		
		flag_adc_ldr=0;
	}
}

void LDR_LED(void)
{
	if(ADC_READ_ldr<1023&&ADC_READ_ldr>768)
	{
		DIO_WritePin(PINB7,HIGH);
		DIO_WritePin(PINA4,LOW);
		DIO_WritePin(PINA5,LOW);
		DIO_WritePin(PINA6,LOW);
	}
	else if(ADC_READ_ldr<768&&ADC_READ_ldr>512)
	{
		DIO_WritePin(PINB7,LOW);
		DIO_WritePin(PINA4,HIGH);
		DIO_WritePin(PINA5,LOW);
		DIO_WritePin(PINA6,LOW);
	}
	else if(ADC_READ_ldr<512&&ADC_READ_ldr>256)
	{
		DIO_WritePin(PINB7,LOW);
		DIO_WritePin(PINA4,LOW);
		DIO_WritePin(PINA5,HIGH);
		DIO_WritePin(PINA6,LOW);
	}
	else if(ADC_READ_ldr>0)
	{
		DIO_WritePin(PINB7,LOW);
		DIO_WritePin(PINA4,LOW);
		DIO_WritePin(PINA5,LOW);
		DIO_WritePin(PINA6,HIGH);
	}
}



void adc_read_chain(void)
{
	if (flag_adc_ldr==0)
	{
		ADC_READ_ldr=ADC_16ReadADC();
		flag_adc_ldr=1;
		channel=TEMP_CH;
		ADC_voidStartConversion(channel);
	}
	else if (flag_adc_lm35==0)
	{
		TEMP_READ=Temp_Read();
		flag_adc_lm35=1;
		channel=LDR_CH;
		ADC_voidStartConversion(channel);
	}
}

void set_ldr(void)
{
	DIO_WritePin(PINC7,HIGH);
}

void set_temp(void)
{
	DIO_WritePin(PINC6,HIGH);
}