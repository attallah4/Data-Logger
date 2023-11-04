#include "StdTypes.h"

#include "Data_Logger.h"







int main(void)
{
	
	Data_Logger_Init();
	while (1)
	{
		Data_Logger_Runnable();
	}
}

