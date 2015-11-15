/* ###################################################################
**     Filename    : Events.h
**     Project     : daq
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-10-31, 13:40, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Pins1.h"
#include "UART_SDA.h"
#include "UART_ESP8266.h"
#include "Pot.h"
#include "I2C0.h"
#include "RTC.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MK64FN1M0LL12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void);


/*
** ===================================================================
**     Event       :  I2C0_OnMasterBlockSent (module Events)
**
**     Component   :  I2C0 [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C in master mode finishes the
**         transmission of the data successfully. This event is not
**         available for the SLAVE mode and if MasterSendBlock is
**         disabled. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void I2C0_OnMasterBlockSent(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  I2C0_OnMasterBlockReceived (module Events)
**
**     Component   :  I2C0 [I2C_LDD]
*/
/*!
**     @brief
**         This event is called when I2C is in master mode and finishes
**         the reception of the data successfully. This event is not
**         available for the SLAVE mode and if MasterReceiveBlock is
**         disabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void I2C0_OnMasterBlockReceived(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  UART_ESP8266_OnBlockReceived (module Events)
**
**     Component   :  UART_ESP8266 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void UART_ESP8266_OnBlockReceived(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  UART_ESP8266_OnBlockSent (module Events)
**
**     Component   :  UART_ESP8266 [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void UART_ESP8266_OnBlockSent(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  UART_SDA_OnBlockReceived (module Events)
**
**     Component   :  UART_SDA [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void UART_SDA_OnBlockReceived(LDD_TUserData *UserDataPtr);

/*
** ===================================================================
**     Event       :  UART_SDA_OnBlockSent (module Events)
**
**     Component   :  UART_SDA [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. 
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void UART_SDA_OnBlockSent(LDD_TUserData *UserDataPtr);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
