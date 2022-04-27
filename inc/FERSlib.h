/******************************************************************************
* 
* CAEN SpA - Front End Division
* Via Vetraia, 11 - 55049 - Viareggio ITALY
* +390594388398 - www.caen.it
*
***************************************************************************//**
* \note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
******************************************************************************/

#ifndef _FERSLIB_H
#define _FERSLIB_H				// Protect against multiple inclusion

#include <stdint.h>

#define FERSLIB_MAX_NCNC			4		// Max number of concentrators
#define FERSLIB_MAX_NBRD			16		// Max number of connected boards
#define FERSLIB_MAX_NCH				64		// Max number of channels per board

#define ENABLE_FERSLIB_LOGMSG		0		// Enable FERSlib to print log messages to console
#define SAVE_RAW_DATA_TO_FILE		0		// Enable read data function to dump event data (32 bit words) into a text file

#define FERS_CONNECT_TIMEOUT		3		// timeout connection in seconds

#define CLK_PERIOD					8		// clock period in ns (for reg settings)

#define MAX_WAVEFORM_LENGTH			2048
#define MAX_LIST_SIZE				128
#define MAX_TEST_NWORDS				4

// Error Codes
#define FERSLIB_ERR_GENERIC				-1
#define FERSLIB_ERR_COMMUNICATION		-2
#define FERSLIB_ERR_MAX_NBOARD_REACHED	-3
#define FERSLIB_ERR_INVALID_PATH		-4
#define FERSLIB_ERR_INVALID_HANDLE		-5
#define FERSLIB_ERR_READOUT_ERROR		-6
#define FERSLIB_ERR_MALLOC_BUFFERS		-7
#define FERSLIB_ERR_INVALID_BIC			-8
#define FERSLIB_ERR_READOUT_NOT_INIT	-9
#define FERSLIB_ERR_PEDCALIB_NOT_FOUND	-10
#define FERSLIB_ERR_INVALID_FWFILE		-11
#define FERSLIB_ERR_UPGRADE_ERROR		-12
#define FERSLIB_ERR_INVLID_PARAM		-13
#define FERSLIB_ERR_NOT_APPLICABLE		-14

// Acquisition Modes
#define ACQMODE_SPECT				0x01  // Spectroscopy Mode (Energy)
#define ACQMODE_TSPECT				0x03  // Spectroscopy + Timing Mode (Energy + Tstamp)
#define ACQMODE_TIMING_CSTART		0x02  // Timing Mode - Common Start (List)
#define ACQMODE_TIMING_CSTOP		0x12  // Timing Mode - Common Stop (List)
#define ACQMODE_TIMING_STREAMING	0x22  // Timing Mode - Streaming (List)
#define ACQMODE_COUNT				0x04  // Counting Mode (MCS)
#define ACQMODE_WAVE				0x08  // Waveform Mode

// Data Qualifier
#define DTQ_SPECT					0x01  // Spectroscopy Mode (Energy)
#define DTQ_TIMING					0x02  // Timing Mode 
#define DTQ_COUNT 					0x04  // Counting Mode (MCS)
#define DTQ_WAVE					0x08  // Waveform Mode
#define DTQ_TSPECT					0x03  // Spectroscopy + Timing Mode (Energy + Tstamp)
#define DTQ_TEST					0xFF  // Test Mode 

// Data width for Energy, ToA and ToT
#define ENERGY_NBIT					14	
#define TOA_NBIT					16
#define TOT_NBIT					9

#define FLASH_PAGE_SIZE				528	// flash page size for AT54DB321
#define FLASH_BIC_PAGE				0	// flash page with Board IDcard
#define FLASH_PEDCALIB_PAGE			4	// flash page with Pedestal calibration
#define FLASH_PEDCALIB_BCK_PAGE		5	// flash page with Pedestal calibration (backup)

// Handles and indexing
#define FERS_INDEX(handle)				((handle) & 0xFF)		// Board Index
#define FERS_CONNECTIONTYPE(handle)		((handle) & 0xF0000)	// Connection Type
#define FERS_CONNECTIONTYPE_ETH			0x00000
#define FERS_CONNECTIONTYPE_USB			0x10000
#define FERS_CONNECTIONTYPE_TDL			0x20000
#define FERS_CONNECTIONTYPE_CNC			0x80000
#define FERS_NODE(handle)				((handle >> 20) & 0xF)
#define FERS_CHAIN(handle)				((handle >> 24) & 0xF)
#define FERS_CNCINDEX(handle)			((handle >> 30) & 0xF)


//******************************************************************
// Board Info Struct
//******************************************************************
typedef struct {
	uint32_t pid;			// Board PID (5 decimal digits)
	uint16_t FERSCode;	// e.g. 5202
	uint8_t PCBrevision;	// PCB revision 
	char ModelCode[16];		// e.g. WA5202XAAAAA
	char ModelName[16];		// e.g. A5202
	uint8_t FormFactor;		// 0=naked version (A52XX), 1=boxed version (DT52XX)
	uint16_t NumCh;			// Number of channels
	uint32_t FPGA_FWrev;	// FPGA FW revision 
	uint32_t uC_FWrev;		// uC FW revision
} FERS_BoardInfo_t;

//******************************************************************
// Event Data Structures
//******************************************************************
// Spectroscopy Event (with or without timing)
typedef struct {
	double tstamp_us;
	uint64_t trigger_id;
	uint64_t chmask;
	uint64_t qdmask;
	uint16_t energyHG[64];
	uint16_t energyLG[64];
	uint32_t tstamp[64];	// used in TSPEC mode only
	uint16_t ToT[64];		// used in TSPEC mode only
} SpectEvent_t;

// Counting Event
typedef struct {
	double tstamp_us;
	uint64_t trigger_id;
	uint64_t chmask;
	uint32_t counts[64];
	uint32_t t_or_counts;
	uint32_t q_or_counts;
} CountingEvent_t;

// Waveform Event
typedef struct {
	double tstamp_us;
	uint64_t trigger_id;
	uint16_t ns;
	uint16_t *wave_hg;
	uint16_t *wave_lg;
	uint8_t *dig_probes;
} WaveEvent_t;

// List Event (timing mode only)
typedef struct {
	uint16_t nhits;
	uint8_t  channel[MAX_LIST_SIZE];
	uint32_t tstamp[MAX_LIST_SIZE];
	uint16_t ToT[MAX_LIST_SIZE];
} ListEvent_t;

// Test Mode Event (fixed data patterns)
typedef struct {
	double tstamp_us;
	uint64_t trigger_id;
	uint16_t nwords;
	uint32_t test_data[MAX_TEST_NWORDS];
} TestEvent_t;


// *****************************************************************
// Global Variables
// *****************************************************************
extern FERS_BoardInfo_t *FERS_BoardInfo[FERSLIB_MAX_NBRD];	// pointers to the board info structs 

// Macros for getting main parameters of the FERS units 
#define FERS_Model(handle)			((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->Model : 0)
#define FERS_pid(handle)			((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->pid : 0)
#define FERS_Code(handle)			((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->FERSCode : 0)
#define FERS_ModelName(handle)		((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->ModelName : "")
#define FERS_FPGA_FWrev(handle)		((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->FPGA_FWrev : 0)
#define FERS_uC_FWrev(handle)		((FERS_INDEX(handle) >= 0) ? FERS_BoardInfo[FERS_INDEX(handle)]->uC_FWrev : 0)

/*
// *****************************************************************
// Messaging and errors
// *****************************************************************
int FERS_LibMsg(char *fmt, ...);

// *****************************************************************
// Open/Close
// *****************************************************************
int FERS_OpenDevice(char *path, int *handle);
int FERS_CloseDevice(int handle);

// *****************************************************************
// Register Read/Write
// *****************************************************************
int FERS_ReadRegister(int handle, uint32_t address, uint32_t *data);
int FERS_WriteRegister(int handle, uint32_t address, uint32_t data);
int FERS_WriteRegisterSlice(int handle, uint32_t address, uint32_t start_bit, uint32_t stop_bit, uint32_t data);
int FERS_SendCommand(int handle, uint32_t cmd);
int FERS_SendCommandBroadcast(int *handle, uint32_t cmd, uint32_t delay);
int FERS_ReadBoardInfo(int handle, FERS_BoardInfo_t *binfo);
int FERS_WriteBoardInfo(int handle, FERS_BoardInfo_t binfo);
int FERS_WritePedestals(int handle, uint16_t *PedLG, uint16_t *PedHG, uint16_t *dco);
int FERS_ReadPedestalsFromFlash(int handle, char *date, uint16_t *PedLG, uint16_t *PedHG, uint16_t *dco);
int FERS_PedestalBackupPage(int handle, int EnBckPage);
int FERS_SetCommonPedestal(int handle, uint16_t Pedestal);
int FERS_EnablePedestalCalibration(int handle, int enable);
int FERS_GetChannelPedestalBeforeCalib(int handle, int ch, uint16_t *PedLG, uint16_t *PedHG);
int FERS_Get_FPGA_Temp(int handle, float *temp);

// *****************************************************************
// Flash Read/Write
// *****************************************************************
int FERS_ReadFlashPage(int handle, int pagenum, int size, uint8_t *data);
int FERS_WriteFlashPage(int handle, int pagenum, int size, uint8_t *data);

// *****************************************************************
// High Voltage Control
// *****************************************************************
int HV_Init(int handle);
int HV_WriteReg(int handle, uint32_t reg_addr, uint32_t dtype, uint32_t reg_data);
int HV_ReadReg(int handle, uint32_t reg_addr, uint32_t dtype, uint32_t *reg_data);
int HV_Set_OnOff(int handle, int OnOff);
int HV_Get_Status(int handle, int *OnOff, int *Ramping, int *OvC, int *OvV);
int HV_Set_Vbias(int handle, float vbias);
int HV_Get_Vbias(int handle, float *vbias);
int HV_Get_Vmon(int handle, float *vmon);
int HV_Set_Imax(int handle, float imax);
int HV_Get_Imax(int handle, float *imax);
int HV_Get_Imon(int handle, float *imon);
int HV_Get_IntTemp(int handle, float *temp);
int HV_Get_DetectorTemp(int handle, float *temp);
int HV_Set_Tsens_Coeff(int handle, float Tsens_coeff[3]);
int HV_Set_TempFeedback(int handle, int enable, float Tsens_coeff);


// *****************************************************************
// TDC Config and readout (for test)
// *****************************************************************
int TDC_WriteReg(int handle, int tdc_id, uint32_t addr, uint32_t data);
int TDC_ReadReg(int handle, int tdc_id, uint32_t addr, uint32_t *data);
int TDC_Config(int handle, int tdc_id, int StartSrc, int StopSrc);
int TDC_DoStartStopMeasurement(int handle, int tdc_id, double *tof_ns);


// *****************************************************************
// Data Readout
// *****************************************************************
int FERS_InitReadout(int handle, int *AllocatedSize);
int FERS_CloseReadout(int handle);
int FERS_FlushData(int handle);
int FERS_GetEvent(int *handle, int *bindex, int *DataQualifier, double *tstamp_us, void **Event, int *nb);
int FERS_GetEventFromBoard(int handle, int *DataQualifier, double *tstamp_us, void **Event, int *nb);

// *****************************************************************
// Firmware Upgrade
// *****************************************************************
int FERS_FirmwareUpgrade(int handle, FILE *fp, void(*ptr)(char *msg, int progress));
*/

#endif
