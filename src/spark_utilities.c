#include "spark_utilities.h"
#include "socket.h"
#include "netapp.h"
#include "string.h"

long sparkSocket;
sockaddr tSocketAddr;

timeval timeout;
fd_set readSet;

// Spark Messages
const char Device_Secret[] = "secret";
const char Device_Name[] = "willisawesome";
const char Device_Ok[] = "OK ";
const char Device_Fail[] = "FAIL ";
const char Device_CRLF[] = "\n";
const char API_Who[] = "who";
const char API_UserFunc[] = "USERFUNC ";
const char API_Callback[] = "CALLBACK ";
char High_Dx[] = "HIGH D ";
char Low_Dx[] = "LOW D ";

char digits[] = "0123456789";

static int Spark_Send_Device_Message(long socket, char * cmd, char * cmdparam, char * cmdvalue);
static unsigned char itoa(int cNum, char *cString);
static uint8_t atoc(char data);

char recvBuff[SPARK_BUF_LEN];
int total_bytes_received = 0;

/*
static uint16_t atoshort(char b1, char b2);
static unsigned char ascii_to_char(char b1, char b2);

static void str_cpy(char dest[], char src[]);
static int str_cmp(char str1[], char str2[]);
static int str_len(char str[]);
static void sub_str(char dest[], char src[], int offset, int len);
*/

int Spark_Connect(void)
{
	int retVal = 0;

    sparkSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sparkSocket < 0)
    {
        //wlan_stop();
        return -1;
    }

	// the family is always AF_INET
    tSocketAddr.sa_family = AF_INET;

	// the destination port
    tSocketAddr.sa_data[0] = (SPARK_SERVER_PORT & 0xFF00) >> 8;
    tSocketAddr.sa_data[1] = (SPARK_SERVER_PORT & 0x00FF);

	// the destination IP address
	tSocketAddr.sa_data[2] = 192;	// First Octet of destination IP
	tSocketAddr.sa_data[3] = 168;	// Second Octet of destination IP
	tSocketAddr.sa_data[4] = 0; 	// Third Octet of destination IP
	tSocketAddr.sa_data[5] = 47;	// Fourth Octet of destination IP

	retVal = connect(sparkSocket, &tSocketAddr, sizeof(tSocketAddr));

	if (retVal < 0)
	{
		// Unable to connect
		return -1;
	}
	else
	{
//		retVal = Spark_Send_Device_Message(sparkSocket, (char *)Device_Secret, NULL, NULL);
		retVal = send(sparkSocket, "A", 1, 0);
	}

    return retVal;
}

int Spark_Disconnect(void)
{
    int retVal = 0;

    retVal = closesocket(sparkSocket);

    if(retVal == 0)
    	sparkSocket = 0xFFFFFFFF;

    return retVal;
}

// receive from socket until we either find a newline or fill the buffer
// called repeatedly from an interrupt handler, so DO NOT BLOCK
// returns: -1 on error, signifying socket disconnected
//          0 if we have not yet received a full line
//          the number of bytes received when we have received a full line
int receive_line()
{
	if (0 == total_bytes_received)
	{
		memset(recvBuff, 0, SPARK_BUF_LEN);

	    // reset the fd_set structure
	    FD_ZERO(&readSet);
	    FD_SET(sparkSocket, &readSet);
	}

    int buffer_bytes_available = SPARK_BUF_LEN - 1 - total_bytes_received;
    char *newline = NULL;

    // tell select to timeout after 500 microseconds
    timeout.tv_sec = 0;
    timeout.tv_usec = 500;

	int num_fds_ready = select(sparkSocket+1, &readSet, NULL, NULL, &timeout);
	if (0 < num_fds_ready)
	{
		if (FD_ISSET(sparkSocket, &readSet))
		{
			char *buffer_ptr = recvBuff + total_bytes_received;
			int bytes_received_once = recv(sparkSocket, buffer_ptr, buffer_bytes_available, 0);
			if (0 > bytes_received_once)
				return bytes_received_once;

			total_bytes_received += bytes_received_once;
			newline = strchr(recvBuff, '\n');
		}
	}

//    if (NULL == newline && 0 < buffer_bytes_available)
//    {
//    	return 0;
//    }
//    else
//    {
    	int retVal = total_bytes_received;
    	total_bytes_received = 0;
    	return retVal;
//    }
}

// process the contents of recvBuff
// returns number of bytes transmitted or -1 on error
int process_command()
{
	int bytes_sent;

	// who
	if (0 == strncmp(recvBuff, API_Who, strlen(API_Who)))
	{
		bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Name, NULL, NULL);
	}

	// device id echoed by server
	else if (0 == strncmp(recvBuff, Device_Name, strlen(Device_Name)))
    {
    	LED_On(LED1);
    	bytes_sent = 0;
    }

	// command to set a pin high
	else if (0 == strncmp(recvBuff, High_Dx, 6))
	{
		High_Dx[6] = recvBuff[6];

		if (OK == DIO_SetState(atoc(High_Dx[6]), HIGH))
			bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Ok, (char *)High_Dx, NULL);
		else
			bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Fail, (char *)High_Dx, NULL);
	}

	// command to set a pin low
	else if (0 == strncmp(recvBuff, Low_Dx, 5))
	{
		Low_Dx[5] = recvBuff[5];

		if (OK == DIO_SetState(atoc(Low_Dx[5]), LOW))
			bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Ok, (char *)Low_Dx, NULL);
		else
			bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Fail, (char *)Low_Dx, NULL);
	}

	// command to call the user-defined function
	else if (0 == strncmp(recvBuff, API_UserFunc, strlen(API_UserFunc)))
	{
		if (NULL != userFunction)
		{
			char *user_arg = &recvBuff[strlen(API_UserFunc)];
			char *newline = strchr(user_arg, '\n');
			if (NULL != newline)
			{
				if ('\r' == *(newline - 1))
					newline--;
				*newline = '\0';
			}

			char retStr[11];
			int userResult = userFunction(user_arg);
			int retLen = itoa(userResult, retStr);
			retStr[retLen] = '\0';
			bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Ok, (char *)API_UserFunc, (char *)retStr);
		}
	}

	// Do nothing for new line returned
	else if(strcmp(recvBuff, Device_CRLF) == 0)
	{
		bytes_sent = 0;
	}
	else
	{
//		bytes_sent = Spark_Send_Device_Message(sparkSocket, (char *)Device_Fail, (char *)recvBuff, NULL);
		char one_char[2] = {0, 0};
		one_char[0] = recvBuff[0] + 1;
		bytes_sent = send(sparkSocket, one_char, 1, 0);
	}

	return bytes_sent;
}

int Spark_Process_API_Response(void)
{
	int retVal = receive_line();

	if (0 < retVal)
		retVal = process_command();
//	else
//		Spark_Send_Device_Message(sparkSocket, (char *)Device_Name, NULL, NULL); // keepalive

	return retVal;
}

void userCallback(char *callback_name)
{
	Spark_Send_Device_Message(sparkSocket, (char *)API_Callback, (char *)callback_name, NULL);
}

void userCallbackWithData(char *callback_name, char *callback_data, long data_length)
{
	char len, lenStr[11];
	len = itoa(data_length, &lenStr[0]);
	lenStr[len] = '\0';
	Spark_Send_Device_Message(sparkSocket, (char *)API_Callback, (char *)callback_name, (char *)lenStr);
}

// returns number of bytes transmitted or -1 on error
static int Spark_Send_Device_Message(long socket, char * cmd, char * cmdparam, char * cmdvalue)
{
    char cmdBuf[SPARK_BUF_LEN];
    int sendLen = 0;
    int retVal = 0;

    memset(cmdBuf, 0, SPARK_BUF_LEN);

    if(cmd != NULL)
    {
        sendLen = strlen(cmd);
        memcpy(cmdBuf, cmd, strlen(cmd));
    }

    if(cmdparam != NULL)
    {
        memcpy(&cmdBuf[sendLen], cmdparam, strlen(cmdparam));
        sendLen += strlen(cmdparam);
    }

    if(cmdvalue != NULL)
    {
        memcpy(&cmdBuf[sendLen], cmdvalue, strlen(cmdvalue));
        sendLen += strlen(cmdvalue);
    }

    memcpy(&cmdBuf[sendLen], Device_CRLF, strlen(Device_CRLF));
    sendLen += strlen(Device_CRLF);

    retVal = send(socket, cmdBuf, sendLen, 0);

    return retVal;
}

// brief  Convert integer to ASCII in decimal base
static unsigned char itoa(int cNum, char *cString)
{
    char* ptr;
    int uTemp = cNum;
    unsigned char length;

    // value 0 is a special case
    if (cNum == 0)
    {
        length = 1;
        *cString = '0';

        return length;
    }

    // Find out the length of the number, in decimal base
    length = 0;
    while (uTemp > 0)
    {
        uTemp /= 10;
        length++;
    }

    // Do the actual formatting, right to left
    uTemp = cNum;
    ptr = cString + length;
    while (uTemp > 0)
    {
        --ptr;
        *ptr = digits[uTemp % 10];
        uTemp /= 10;
    }

    return length;
}

//Convert nibble to hexdecimal from ASCII
static uint8_t atoc(char data)
{
	unsigned char ucRes;

	if ((data >= 0x30) && (data <= 0x39))
	{
		ucRes = data - 0x30;
	}
	else
	{
		if (data == 'a')
		{
			ucRes = 0x0a;;
		}
		else if (data == 'b')
		{
			ucRes = 0x0b;
		}
		else if (data == 'c')
		{
			ucRes = 0x0c;
		}
		else if (data == 'd')
		{
			ucRes = 0x0d;
		}
		else if (data == 'e')
		{
			ucRes = 0x0e;
		}
		else if (data == 'f')
		{
			ucRes = 0x0f;
		}
	}
	return ucRes;
}

/*
// Convert 2 nibbles in ASCII into a short number
static uint16_t atoshort(char b1, char b2)
{
	uint16_t usRes;
	usRes = (atoc(b1)) * 16 | atoc(b2);
	return usRes;
}

// Convert 2 bytes in ASCII into one character
static unsigned char ascii_to_char(char b1, char b2)
{
	unsigned char ucRes;

	ucRes = (atoc(b1)) << 4 | (atoc(b2));

	return ucRes;
}

// Various String Functions
static void str_cpy(char dest[], char src[])
{
	int i = 0;
	for(i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
}

static int str_cmp(char str1[], char str2[])
{
	int i = 0;
	while(1)
	{
		if(str1[i] != str2[i])
			return str1[i] - str2[i];
		if(str1[i] == '\0' || str2[i] == '\0')
			return 0;
		i++;
	}
}

static int str_len(char str[])
{
	int i;
	for(i = 0; str[i] != '\0'; i++);
	return i;
}

static void sub_str(char dest[], char src[], int offset, int len)
{
	int i;
	for(i = 0; i < len && src[offset + i] != '\0'; i++)
		dest[i] = src[i + offset];
	dest[i] = '\0';
}

*/