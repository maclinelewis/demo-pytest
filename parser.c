#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "calc.c"
#include <stdlib.h>

HANDLE hComm;                          	// Handle to the Serial port
char  ComPortName[] = "COM4";  			// Name of the Serial port(May Change) to be opened,
BOOL  Status_R, Status_W, MATCH_FOUND ;	// Status of the various operations 
DWORD dwEventMask;                     	// Event mask to trigger
char  TempChar;                        	// Temperory Character
char  SerialBuffer[256];               	// Buffer Containing Rxed Data
char SerialBuffer_W[256];				// Buffer Containing data to be Txed
DWORD NoBytesRead; 	                   	// Bytes read by ReadFile()


void com_reception_handler(void);
void send_string(void);
void rxv_string(void);
void com_config_init(void);
void strip_special_chars (char *);


void main(void)
		{
			com_config_init();
			while(1)
				{
					com_reception_handler();			
				}	
		}//End of Main()


void com_reception_handler()
{
	/*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/
	printf("\n\n    Waiting for Data Reception");

	Status_R = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

	/*-------------------------- Program will Wait here till a Character is received ------------------------*/				
	if (Status_R == FALSE)
		{
			printf("\n    Error! in Setting WaitCommEvent()");
			CloseHandle(hComm);//Closing the Serial Port
			printf("\n +==========================================+\n");
			getchar();
		}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
		{
			rxv_string();			
		}
}


void send_string()
{
	//from uart, string will be received in format "test000_10_20"
	int num1, num2 = 0;
	char sub_str1[10] = {0};
	char sub_str2[10] = {0};
	char sub_str3[10] = {0};
	char * token = strtok(SerialBuffer, "_");
	MATCH_FOUND = FALSE;
	int i = 0;

	while( token != NULL) 
		{
		if (i == 0)
			strcpy(sub_str1,token); //printing each token
		else if (i == 1)
			strcpy(sub_str2,token);
		else if (i == 2)
			strcpy(sub_str3,token);
		token = strtok(NULL, "_");
		i++;
		}

	if(strcmp(sub_str1,"test000") == 0)

		{	
			MATCH_FOUND = TRUE;
			if(sub_str2)
				num1 = atoi(sub_str2);
			if(sub_str3)
				num2 = atoi(sub_str3);
			printf("\nnum1 and num2 = %d %d",num1,num2);
			sprintf(SerialBuffer_W,"%d,%d,%d,%d\n",add_c(num1,num2),add_c(-num1,-num2),
									add_c(-num1,num2),add_c(num1,-num2));
		}
	else if(strcmp(sub_str1,"test001") == 0)
		{	
			MATCH_FOUND = TRUE;
			if(sub_str2)
				num1 = atoi(sub_str2);
			if(sub_str3)
				num2 = atoi(sub_str3);
			printf("\nnum1 and num2 = %d %d",num1,num2); 
			sprintf(SerialBuffer_W,"%d,%d,%d,%d\n",sub_c(num1,num2),sub_c(-num1,-num2),
									sub_c(-num1,num2),sub_c(num1,-num2));
		}
	else if(strcmp(sub_str1,"test002") == 0)	
		{	
			MATCH_FOUND = TRUE;
			if(sub_str2)
				num1 = atoi(sub_str2);
			if(sub_str3)
				num2 = atoi(sub_str3);
			printf("\nnum1 and num2 = %d %d",num1,num2);
			sprintf(SerialBuffer_W,"%0.2f,%0.2f,%0.2f,%0.2f\n",div_c(num1,num2),div_c(-num1,-num2),
									div_c(-num1,num2),div_c(num1,-num2));
		}
	else if(strcmp(sub_str1,"test003") == 0)
		
		{	
			MATCH_FOUND = TRUE;
			if(sub_str2)
				num1 = atoi(sub_str2);
			if(sub_str3)
				num2 = atoi(sub_str3);
			printf("\nnum1 and num2 = %d %d",num1,num2);
			sprintf(SerialBuffer_W,"%d,%d,%d,%d\n",mul_c(num1,num2),mul_c(-num1,-num2),
								mul_c(-num1,num2),mul_c(num1,-num2));
		}
	else
		{
			MATCH_FOUND = FALSE;
		}


	if(MATCH_FOUND)
	{
	DWORD  dNoOFBytestoWrite;              		// No of bytes to write into the port
	DWORD  dNoOfBytesWritten = 0;          		// No of bytes written to the port

	dNoOFBytestoWrite = sizeof(SerialBuffer_W); // Calculating the no of bytes to write into the port

	Status_W = WriteFile(hComm,               	// Handle to the Serialport
					   SerialBuffer_W,          // Data to be written to the port 
					   dNoOFBytestoWrite,   	// No of bytes to write into the port
					   &dNoOfBytesWritten,  	// No of bytes written to the port
					   NULL);

	if (Status_W == TRUE)
		{	
			printf("\n\n    %s - Written to %s", SerialBuffer_W, ComPortName);
			for(;i!=0;i--)
			SerialBuffer[i] = 0;
		}
		else
			printf("\n\n   Error %d in Writing to Serial Port",GetLastError());
		}
}


void rxv_string()
{
	int i = 0;
	do
		{
			Status_R = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			SerialBuffer[i] = TempChar;
			i++;
		}
	while (NoBytesRead > 0);

	SerialBuffer[i-1] = '\0';
	strip_special_chars(SerialBuffer);
	send_string();
	for(;i!=0;i--)
	SerialBuffer[i] = 0;
}


void com_config_init()
{
	printf("\n\n +==========================================+");
	printf("\n |    Serial Port  Reception (Win32 API)    |");
	printf("\n +==========================================+\n");

	/*---------------------------------- Opening the Serial Port -------------------------------------------*/
	hComm = CreateFile( ComPortName,                  // Name of the Port to be Opened
						GENERIC_READ | GENERIC_WRITE, // Read/Write Access
						0,                            // No Sharing, ports cant be shared
						NULL,                         // No Security
						OPEN_EXISTING,                // Open existing port only
						0,                            // Non Overlapped I/O
						NULL);                        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("\n    Error! - Port %s can't be opened\n", ComPortName);
	else
		printf("\n    Port %s Opened\n ", ComPortName);

	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/
	DCB dcbSerialParams = { 0 };                         	// Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status_R = GetCommState(hComm, &dcbSerialParams);      	//retreives  the current settings

	if (Status_R == FALSE)
		printf("\n    Error! in GetCommState()");

	dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

	Status_R = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

	if (Status_R == FALSE)
		{
			printf("\n    Error! in Setting DCB Structure");
		}
	else //If Successfull display the contents of the DCB Structure
		{
			printf("\n\n    Setting DCB Structure Successfull\n");
			printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
			printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
			printf("\n       StopBits = %d", dcbSerialParams.StopBits);
			printf("\n       Parity   = %d", dcbSerialParams.Parity);
		}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
		printf("\n\n    Error! in Setting Time Outs");
	else
		printf("\n\n    Setting Serial Port Timeouts Successfull");

	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/
	Status_R = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (Status_R == FALSE)
		printf("\n\n    Error! in Setting CommMask");
	else
		printf("\n\n    Setting CommMask successfull");
}


void strip_special_chars(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n' && *s != '\r') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}
