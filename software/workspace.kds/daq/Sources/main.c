/* ###################################################################
**     Filename    : main.c
**     Project     : daq
**     Processor   : MK64FN1M0VLL12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-31, 13:40, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "UART_SDA.h"
#include "UART_ESP8266.h"
#include "Pot.h"
#include "I2C0.h"
#include "RTC.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */


//Dados de calibracao
short int AC1 = 0;
short int AC2 = 0;
short int AC3 = 0;
unsigned short int AC4 = 0;
unsigned short int AC5 = 0;
unsigned short int AC6 = 0;
short int B1 = 0;
short int B2 = 0;
short int MB = 0;
short int MC = 0;
short int MD = 0;

char response[64];
unsigned int expectedResponseSize = 0;

//Calcula o delay necessario baseado na quantidade de caracteres a serem enviados
//Delay em ms ~= numero de caracteres * 0.1
void delay(unsigned short int charNum){
	unsigned long int i;
	unsigned long int max = charNum*500;
	for(i=0;i<max;i++){}
}

void sendAndGetResponseSerialESP8266(char cmd[]){
	UART_ESP8266_SendBlock(UART_ESP8266_DeviceData, cmd, strlen(cmd));
	delay(strlen(cmd));//Espera uma resposta durante esse delay
}

void sendConsoleMessage(char msg[]){
	UART_SDA_SendBlock(UART_SDA_DeviceData, msg, strlen(msg));
	delay(strlen(msg));
}

//Envia um AT e verifica se responde OK, apenas para testar a comunicacao
void simpleAT(){
	expectedResponseSize=6;
	sendAndGetResponseSerialESP8266("AT\r\n");
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage("[INFO] - AT command returned OK\r\n");
	}
	else {
		sendConsoleMessage("   [ERROR] - AT command not OK\r\n");
	}
}

//Verifica se esta conectado em algum ponto de acesso
void amIConnected(){
	expectedResponseSize=11;
	sendAndGetResponseSerialESP8266("AT+CWJAP?\r\n");
	if(strstr(response, "No AP")){
		sendConsoleMessage("   [ERROR] - Not connected to a station\r\n");
	}
	else {
		sendConsoleMessage("[INFO] - Module is connected to a station\r\n");
	}
}

//Configura entre os modos de operacao: Station, Access Point ou Both
void setWiFiMode(char mode[]){
	expectedResponseSize=6;
	char text[] = "[INFO] - Operation mode is set to: \0\0\0\0\0 ";
	if(mode == "sta") sendAndGetResponseSerialESP8266("AT+CWMODE=1\r\n");
	else if(mode == "ap") sendAndGetResponseSerialESP8266("AT+CWMODE=2\r\n");
	else if(mode == "both") sendAndGetResponseSerialESP8266("AT+CWMODE=3\r\n");
	else {
		sendConsoleMessage("   [ERROR] - Problem at setWiFiMode function\r\n");
		return;
	}
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage(strcat(strcat(text, mode), "\r\n"));
	}
	else {
		sendConsoleMessage("   [ERROR] - Could not set operation mode\r\n");
	}
}

//Define se o modulo ira aceitar multiplas conexoes ou somente uma por vez
void setNumberOfConnections(char mode[]){
	expectedResponseSize=6;
	char text[] = "[INFO] - Number of connections set to: \0\0\0\0\0\0\0\0";
	if(mode == "single") sendAndGetResponseSerialESP8266("AT+CIPMUX=0\r\n");
	if(mode == "multiple") sendAndGetResponseSerialESP8266("AT+CIPMUX=1\r\n");
	else {
		sendConsoleMessage("   [ERROR] - Problem at SetNumberOfConnections function\r\n");
		return;
	}
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage(strcat(strcat(text, mode), "\r\n"));
	}
	else {
		sendConsoleMessage("   [ERROR] - Could not set number of connections\r\n");
	}
}

//Desabilita a repeticao de comandos
void disableCmdEcho(){
	expectedResponseSize=6;
	sendAndGetResponseSerialESP8266("ATE0\r\n");
	if(strstr(response, "\r\nOK\r\n") || strstr(response, "ATE0")){
		sendConsoleMessage("[INFO] - Commands echo disabled \r\n");
	}
	else {
		sendConsoleMessage("   [ERROR] - Problem at disabling commands echo\r\n");
	}
}

void setupModule(){
	disableCmdEcho();
	simpleAT();
	setNumberOfConnections("multiple");
}

//Reinicia o modulo e espera um tempo antes de anviar comandos
void restartModule(){
	expectedResponseSize=6;
	sendAndGetResponseSerialESP8266("AT+RST\r\n");
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage("[INFO] - Module restarted, need to disable commands echo\r\n");
		//Apos reiniciar, espera antes de enviar outros comandos
		delay(10000);
	}
	else {
		sendConsoleMessage("   [ERROR] - Fail to restart module\r\n");
	}
}

//Abre um servidor TCP na porta escolhida
void startTCPServer(char port[]){
	expectedResponseSize=6;
	char text[] = "[INFO] - TCP server opened at port \0\0\0\0\0";
	char cmd[] = "AT+CIPSERVER=1,\0\0\0\0\0";
	//Para trocar a porta, precisa reiniciar, colocar para multiplas conexoes e ai sim trocar a porta
	sendConsoleMessage("[INFO] - Creating TCP server, module will be restarted\r\n");
	restartModule();
	setupModule();
	sendAndGetResponseSerialESP8266(strcat(strcat(cmd, port), "\r\n"));
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage(strcat(strcat(text, port), "\r\n"));
	}
	else {
		sendConsoleMessage("   [ERROR] - Could not start TCP server\r\n");
	}
}


//Recebe uma string e a envia na porta configurada pela funcao startTCPServer()
void sendTCPMsg(char msg[]){
	char cmd[] = ("AT+CIPSEND=0,\0\0\0\0\0\0");
	char numCharInMsg[]="\0\0\0\0\0\0";

	expectedResponseSize=4;
	sprintf(numCharInMsg,"%d", strlen(msg)); //Determina o tamanho da mensagem e transforma em string
	sendAndGetResponseSerialESP8266(strcat(strcat(cmd,numCharInMsg), "\r\n"));
	if(strstr(response, "\r\nOK")){
		sendConsoleMessage("[INFO] - Sending TCP message...\r\n");
		expectedResponseSize=6;
		sendAndGetResponseSerialESP8266(msg);
		if(strstr(response, "\r\nRecv")){
			sendConsoleMessage("[INFO] - TCP message sent\r\n");
		}
		else if (strstr(response, "\r\nbusy")){
			sendTCPMsg(msg); //Se retornar busy, tenta mandar de novo, ate ir
		}
		else {
			sendConsoleMessage("   [ERROR] - Someone is listening, but something happened\r\n");
		}
	}
	else if(strstr(response, "link")){
		sendConsoleMessage("   [ERROR] - Problem sending TCP message (Is there anybody out there?)\r\n");
	}
}

//Configura um ponto de acesso com nome "DAQ" e senha "123987"
void createAP(){
	setWiFiMode("ap");
	expectedResponseSize=6;
	sendAndGetResponseSerialESP8266("AT+CWSAP=\"DAQ\",\"123987\",3,0\r\n");
	if(strstr(response, "\r\nOK\r\n")){
		sendConsoleMessage("[INFO] - Access point created with name \"DAQ\"\r\n");
	}
	else {
		sendConsoleMessage("   [ERROR] - Fail creating access point\r\n");
	}
}

void connectAP(){
	setWiFiMode("sta");
}

char waiting(){
	memset(response, 0x0, 64);
	UART_ESP8266_ReceiveBlock(UART_ESP8266_DeviceData, &response, 9);
	while(response[8] == 0x0){}
	if(strstr(response, "0,CONNE")){
		sendConsoleMessage("[INFO] - Port connected\r\n");
		return 1;
	}
	else if(strstr(response, "0,CLOSE")){
		sendConsoleMessage("[INFO] - Port disconnected\r\n");
		return 2;
	}
	else if(strstr(response, "WIFI CON")){
		sendConsoleMessage("[INFO] - Connected to an access point\r\n");
		return 3;
	}
	else if (strstr(response, "WIFI GOT")){
		sendConsoleMessage("[INFO] - Got IP\r\n");
		return 4;
	}
	else if (strstr(response, "WIFI DIS")){
		sendConsoleMessage("[INFO] - Disconnected from access point\r\n");
		return 5;
	}
	else if (strstr(response, "\r\n+IPD,0,")){
		sendConsoleMessage("[INFO] - Request received\r\n");
		return 6;
	}
	else{
		sendConsoleMessage("   [ERROR] - Problem at function waiting (multi connections?)\r\n");
	}
}


//2.3ms
void smallDelay(){
  unsigned int i;
  for(i=0;i<5000;i++){}
}

//~7.5ms
void bigDelay(){
  unsigned int i;
  for(i=0;i<15000;i++){}
}

//A FRDM K64 le o resultado que chega invertido (da pra mudar isso?)
//por isso e preciso essas operacoes, para destrocar o MSB e o LSB
short int swapNibble(short int number){
  short int numberRealMsb = number & 0b11111111;
  short int numberRealLsb = (number >> 8) & 0b11111111;
  number = (numberRealMsb << 8) | numberRealLsb;
  return number;
}

unsigned int readMemory(char addr){
  short int data = 0;
  I2C0_MasterSendBlock(I2C0_DeviceData, &addr, 1, LDD_I2C_SEND_STOP);
  smallDelay();
  I2C0_MasterReceiveBlock(I2C0_DeviceData, &data, 2, LDD_I2C_SEND_STOP);
  bigDelay();
  return swapNibble(data);
}

char readId(){
  //O ID so interessa o primeiro byte que chega,mas a funcao de leitura le sempre dois bytes
  //por isso essa operacao, para ignorar o primeiro byte
  return ((readMemory(0xD0) & 0xFF00) >> 8);
}

short int readRawTemp(){
  //A FRDM K64 envia primeiro o LSB (talvez de para mudar isso...), por isso a variavel esta invertida
  short tempRequestCodeAndCtrlMeasRegAddr = 0x2EF4;
  I2C0_MasterSendBlock(I2C0_DeviceData, &tempRequestCodeAndCtrlMeasRegAddr, 2, LDD_I2C_SEND_STOP);
  smallDelay(); //Esse delay deve ser de no minimo 5ms segundo o datasheet
  return readMemory(0xF6);
}

long readRawPres(){
  //A FRDM K64 envia primeiro o LSB (talvez de para mudar isso...), por isso a variavel esta invertida
  short presRequestCodeAndCtrlMeasRegAddr = 0x34F4;
  I2C0_MasterSendBlock(I2C0_DeviceData, &presRequestCodeAndCtrlMeasRegAddr, 2, LDD_I2C_SEND_STOP);
  smallDelay(); //Esse delay deve ser de no minimo 5ms segundo o datasheet
  return (readMemory(0xF6) & 0x0000FFFF);
}

void readCalibrationData(){
  AC1 = readMemory(0xAA);
  AC2 = readMemory(0xAC);
  AC3 = readMemory(0xAE);
  AC4 =  readMemory(0xB0);
  AC5 = readMemory(0xB2);
  AC6 = readMemory(0xB4);
  B1 = readMemory(0xB6);
  B2 = readMemory(0xB8);
  MB = readMemory(0xBA);
  MC = readMemory(0xBC);
  MD = readMemory(0xBE);
}

float getProcessedTemp(){
  long UT = readRawTemp();
  long X1 = (UT - AC6)*AC5/32768;
  long X2 = MC*2048/(X1+MD);
  long B5 = X1+X2;
  float temp = (B5+8)/16 * 0.1;
  return temp; //in C
}

float getProcessedPres(){
  long UP = readRawPres();
  long UT = readRawTemp();
  long X1 = (UT - AC6)*AC5/32768;
  long X2 = MC*2048/(X1+MD);
  long B5 = X1+X2;
  long B6 = B5 - 4000;
  X1 = (B2 * (B6 * B6/4096))/2048;
  X2 = AC2 * B6/2048;
  long X3 = X1 + X2;
  long B3 = ((AC1 * 4 + X3) + 2)/4;
  X1 = AC3 * B6/8192;
  X2 = (B1* (B6 * B6/4096))/65536;
  X3 = ((X1+X2)+2)/4;
  unsigned long B4 = AC4 * (unsigned long)(X3 + 32768)/32768;
  unsigned long B7 = ((unsigned long)UP - B3)*(50000);
  float p = 0;
  if(B7 < 0x80000000){
    p = (B7*2)/B4;
  }
  else{
    p = (B7/B4)*2;
  }
  X1 = (p/256) * (p/256);
  X1 = (X1 * 3038)/65536;
  X2 = (-7357 * p)/65536;
  p = p + (X1 + X2 + 3791)/16; //in Pa
  p = p/100; //in hPa
  return p;
}

float getAltitude(){
	float pres = getProcessedPres();
	return 44330 * (1 - pow(pres/1013.25,0.19029495718363465)); // in m
}

unsigned int getRawData(){
  unsigned int rawData = 0;
  //Dispara uma aqusiscao
  Pot_StartSingleMeasurement(Pot_DeviceData);
  smallDelay();
  //Recupera o valor lido
  Pot_GetMeasuredValues(Pot_DeviceData, &rawData);
  return rawData;
}

float getVoltage(){
  unsigned int rawData = 0;
  Pot_StartSingleMeasurement(Pot_DeviceData);
  smallDelay();
  Pot_GetMeasuredValues(Pot_DeviceData, &rawData);
  return rawData * (3.3/(0xFFFF));
}

float getPercentage(){
  unsigned int rawData = 0;
  Pot_StartSingleMeasurement(Pot_DeviceData);
  smallDelay();
  Pot_GetMeasuredValues(Pot_DeviceData, &rawData);
  return (rawData*100.0)/0xFFFF;
}

//Recebe os valores separador e como numeros..
//Os transforma em string e concatena
//O separador é uma ,
// Retorna na seguinte formato:
// DD/MM/AA - HH:MM:SS, temp, pres, alt, pot
char *buildMsg(float temp, float pres, float alt, float percent, LDD_RTC_TTime time){
	static char msg[50];
	memset(msg, 0x0, 30);
	char aux[8];
	sprintf(msg, "%d", time.Day);
	strcat(msg, "/");
	sprintf(aux, "%d", time.Month);
	strcat(msg, aux);
	strcat(msg, "/");
	sprintf(aux, "%d", time.Year);
    strcat(msg, aux);
    strcat(msg, ",");
    sprintf(aux, "%d", time.Hour);
    strcat(msg, aux);
    strcat(msg, ":");
    sprintf(aux, "%d", time.Minute);
    strcat(msg, aux);
    strcat(msg, ":");
    sprintf(aux, "%d", time.Second);
    strcat(msg, aux);
    strcat(msg, ",");
	sprintf(aux, "%.2f", pres);
	strcat(msg, aux);
	strcat(msg, ",");
	sprintf(aux, "%.2f", temp);
	strcat(msg, aux);
	strcat(msg, ",");
	sprintf(aux, "%.2f", alt);
	strcat(msg, aux);
	strcat(msg, ",");
	sprintf(aux, "%.2f", percent);
	strcat(msg, aux);
	return msg;
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	sendConsoleMessage("[INFO] Starting....\r\n");

	LDD_RTC_TTime time;
	LDD_TDeviceData *RTCPtr;

	//Inicializacao soft para não perder os dados que ja estão lá
	RTCPtr = RTC_Init((LDD_TUserData *)NULL, TRUE);

	//Delay para os componentes ligarem, antes de enviar os comandos de configuração
	delay(10000);

	setupModule();
	startTCPServer("80");
	connectAP();
	//createAP();


	//Endereco do BMP180 no baramento I2C
	I2C0_SelectSlaveDevice(I2C0_DeviceData, LDD_I2C_ADDRTYPE_7BITS, 0b1110111);
	//Le da memoria EEPROM os dados de calibracao
	readCalibrationData();

	//Seleciona o grupo 0, que tem somente um AD (o que esta ligado o potenciometro)
	Pot_SelectSampleGroup(Pot_DeviceData, 0);

	float temp = 0;
	float pres = 0;
	float alt = 0;
	float percent = 0;
	char flag = 0;

	while(1){
		flag = waiting();
		if(flag == 6) {
			temp = getProcessedTemp();
			pres = getProcessedPres();
			alt = getAltitude();
			percent = getPercentage();
			RTC_GetTime(RTCPtr, &time);
			sendTCPMsg(buildMsg(temp, pres, alt, percent, time));
		}
	}

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
