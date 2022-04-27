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

#ifndef _REGISTERS_H
#define _REGISTERS_H				// Protect against multiple inclusion


// *****************************************************************
// Individual Channel and Broadcast address converter
// *****************************************************************
#define INDIV_ADDR(addr, ch)		(0x02000000 | ((addr) & 0xFFFF) | ((ch)<<16))
#define BCAST_ADDR(addr)			(0x03000000 | ((addr) & 0xFFFF))

// *****************************************************************
// Register Address Map
// *****************************************************************
#define a_acq_ctrl         0x01000000   //  Acquisition Control Register
#define a_run_mask         0x01000004   //  Run mask: bit[0]=SW, bit[1]=T0-IN
#define a_trg_mask         0x01000008   //  Global trigger mask: bit[0]=SW, bit[1]=T1-IN, bit[2]=Q-OR, bit[3]=Maj, bit[4]=Periodic
#define a_tref_mask        0x0100000C   //  Tref mask: bit[0]=T0-IN, bit[1]=T1-IN, bit[2]=Q-OR, bit[3]=T-OR, bit[5]=Periodic
#define a_validation_mask  0x01000010   //  Validation mask: bit[0]=Cmd from TDL, bit[1]=T0-IN, bit[2]=T1-IN
#define a_t0_out_mask      0x01000014   //  T0 out mask
#define a_t1_out_mask      0x01000018   //  T1 out mask
#define a_veto_mask        0x0100001C   //  Veto mask: bit[0]=Cmd from TDL, bit[1]=T0-IN, bit[2]=T1-IN, 
#define a_tref_delay       0x01000048   //  Delay of the time reference for the coincidences
#define a_tref_window      0x0100004C   //  Tref coincidence window (for list mode)
#define a_dwell_time       0x01000050   //  Dwell time (periodic trigger) in clk cyclces. 0 => OFF
#define a_list_size        0x01000054   //  Number of 32 bit words in a packet in timing mode (list mode)
#define a_channel_mask_0   0x01000100   //  Input Channel Mask (ch  0 to 31)
#define a_channel_mask_1   0x01000104   //  Input Channel Mask (ch 32 to 63)
#define a_citiroc_cfg      0x01000108   //  Citiroc common configuration bits
#define a_citiroc_en       0x0100010C   //  Citiroc internal parts enable mask 
#define a_citiroc_probe    0x01000110   //  Citiroc probes (signal inspector)
#define a_qd_coarse_thr    0x01000114   //  Coarse threshold for the Citiroc Qdiscr (10 bit DAC)
#define a_td_coarse_thr    0x01000118   //  Coarse threshold for the Citiroc Tdiscr (10 bit DAC)
#define a_lg_sh_time       0x0100011C   //  Low gain shaper time constant
#define a_hg_sh_time       0x01000120   //  High gain shaper time constant
#define a_hold_delay       0x01000124   //  Time from gtrg to peak hold
#define a_amux_seq_ctrl    0x01000128   //  Timing parameters for the analog mux readout and conversion
#define a_wave_length      0x0100012C   //  Waveform Length
#define a_scbs_ctrl        0x01000130	//  Citiroc SC bit stream index and select
#define a_scbs_data        0x01000134	//  Citiroc SC bit stream data
#define a_qdiscr_mask_0    0x01000138	//  Charge Discriminator mask
#define a_qdiscr_mask_1    0x0100013C	//  Charge Discriminator mask
#define a_tdiscr_mask_0    0x01000140	//  Time Discriminator mask
#define a_tdiscr_mask_1    0x01000144	//  Time Discriminator mask
#define a_tpulse_ctrl      0x01000200   //  Test pulse mask
#define a_tpulse_dac       0x01000204   //  Internal Test Pulse amplitude
#define a_hv_regaddr       0x01000210   //  HV Register Address and Data Type
#define a_hv_regdata       0x01000214   //  HV Register Data
#define a_trgho            0x01000218   //  Trigger Hold off
#define a_dc_offset        0x01000220   //  DAC for DC offset
#define a_spi_data         0x01000224   //  SPI R/W data (for Flash Memory access)
#define a_test_led         0x01000228   //  Test Mode for LEDs
#define a_tdc_mode         0x0100022C   //  R/W   C   32    TDC modes
#define a_tdc_data         0x01000230   //  R/W   C   32    Regs of TDC
#define a_tlogic_def       0x01000234   //  Trigger Logic Definition
#define a_fw_rev           0x01000300   //  Firmware Revision 
#define a_acq_status       0x01000304   //  Acquisition Status
#define a_real_time        0x01000308   //  Real Time in ms
#define a_dead_time        0x01000310   //  Dead Time in ms
#define a_fpga_temp        0x01000348   //  FPGA die Temperature 
#define a_t_or_cnt         0x01000350	//  T-OR counter
#define a_q_or_cnt         0x01000354	//  Q-OR counter
#define a_pid              0x01000400	//  PID
#define a_pcb_rev          0x01000404	//  PCB revision
#define a_fers_code        0x01000408	//  Fers CODE (5202)
#define a_rebootfpga       0x0100FFF0	//  reboot FPGA from FW uploader
#define a_test_led         0x01000228   //  LED test mode
#define a_tdc_mode         0x0100022C   //  TDC Mode
#define a_tdc_data         0x01000230   //  TDC Data

#define a_commands         0x01008000   //  Send Commands (for Eth and USB)
#define a_zs_lgthr         0x02000000   //  Threshold for zero suppression (LG)
#define a_zs_hgthr         0x02000004   //  Threshold for zero suppression (HG)
#define a_qd_fine_thr      0x02000008   //  Fine individual threshold for the Citiroc Qdiscr (4 bit DAC)
#define a_td_fine_thr      0x0200000C   //  Fine individual threshold for the Citiroc Tdiscr (4 bit DAC)
#define a_lg_gain          0x02000010   //  Preamp Low Gain Setting
#define a_hg_gain          0x02000014   //  Preamp High Gain Setting
#define a_hv_adj           0x02000018   //  HV individual adjust (8 bit DAC)
#define a_hitcnt           0x02000800   //  Hit counters 
  
// *****************************************************************
// Commands
// *****************************************************************
#define CMD_TIME_RESET     0x11  // Absolute Time reset
#define CMD_ACQ_START      0x12  // Start acquisition
#define CMD_ACQ_STOP       0x13  // Stop acquisition
#define CMD_TRG            0x14  // Send software trigger
#define CMD_RESET          0x15  // Global Reset (clear data, set all regs to default)
#define CMD_TPULSE         0x16  // Send a test pulse
#define CMD_RES_PTRG       0x17  // Reset periodic trigger counter (and rearm PTRG in single pulse mode)
#define CMD_CLEAR	       0x18  // Clear Data
#define CMD_VALIDATION	   0x19  // Trigger Validation (either positive = accept or negative = reject)
#define CMD_SET_VETO	   0x1A  // Set Veto
#define CMD_CLEAR_VETO	   0x1B  // Clear Veto
#define CMD_TDL_SYNC	   0x1C  // Sync signal from TDL
#define CMD_CFG_ASIC	   0x20  // Configure ASIC (load shift register)


#define CMD_CFG_ASIC       0x20  // Configure ASIC (load shift register)


#define crcfg_qdiscr_latch          0		// Qdiscr output: 0=latched, 1=direct                        (bit 258 of SR) 
#define crcfg_sca_bias        		1		// SCA bias: 0=high (5MHz readout speed), 1=weak             (bit 301 of SR)
#define crcfg_pdet_mode_hg    		2		// Peak_det mode HighGain: 0=peak detector, 1=T&H            (bit 306 of SR)
#define crcfg_pdet_mode_lg    		3		// Peak_det mode LowGain:  0=peak detector, 1=T&H            (bit 307 of SR)
#define crcfg_ps_ctrl_logic   		4		// Peak Sens Ctrl Logic: 0=internal, 1=external=PS_modeb_ext (bit 308 of SR)
#define crcfg_ps_trg_source   		5		// Peak Sens Trg source: 0=internal, 1=external              (bit 309 of SR)
#define crcfg_lg_pa_bias      		6		// LG Preamp bias: 0=normal, 1=weak                          (bit 323 of SR)
#define crcfg_pa_fast_sh      		7		// Fast shaper connection: 0=high gain pa, 1=low gain pa     (bit 328 of SR)
#define crcfg_8bit_dac_ref    		8		// HV adjust DAC reference: 0=2.5V, 1=4.5V                   (bit 330 of SR)
#define crcfg_ota_bias        		9		// Output OTA buffer bias auto off: 0=auto, 1=force on       (bit 1133 of SR)
#define crcfg_trg_polarity    		10		// Trigger polarity: 0=pos, 1=neg                            (bit 1141 of SR)
#define crcfg_enable_chtrg     		11		// Enable channel triggers                                   (bit 1143 of SR)
#define crcfg_enable_gtrg     		16		// Enable propagation of gtrg to the Citiroc pin global_trig
#define crcfg_enable_veto     		17		// Enable propagation of gate (= not veto) to the Citiroc pin val_evt
#define crcfg_repeat_raz      		18		// Enable loop asserting raz_chn until nor_charge stays active 

#define SHAPING_TIME_12_5NS				6
#define SHAPING_TIME_25NS				5
#define SHAPING_TIME_37_5NS				4
#define SHAPING_TIME_50NS				3
#define SHAPING_TIME_62_5NS				2
#define SHAPING_TIME_75NS				1
#define SHAPING_TIME_87_5NS				0

#define TEST_PULSE_SOURCE_EXT			0
#define TEST_PULSE_SOURCE_T0_IN			1
#define TEST_PULSE_SOURCE_T1_IN			2
#define TEST_PULSE_SOURCE_PTRG			3
#define TEST_PULSE_SOURCE_SW_CMD		4

#define TEST_PULSE_PREAMP_LG 			1
#define TEST_PULSE_PREAMP_HG 			2
#define TEST_PULSE_PREAMP_BOTH			3
/*
#define WAVE_SOURCE_LG0					0
#define WAVE_SOURCE_HG0					1
#define WAVE_SOURCE_LG1					2
#define WAVE_SOURCE_HG1					3
*/
#define APROBE_OFF						0
#define APROBE_FAST						1
#define APROBE_SLOW_LG					2
#define APROBE_SLOW_HG					3
#define APROBE_PREAMP_LG				4
#define APROBE_PREAMP_HG				5

#define DPROBE_OFF						0
#define DPROBE_PEAK_LG					1
#define DPROBE_PEAK_HG					2
#define DPROBE_HOLD						3
#define DPROBE_START_CONV				4
#define DPROBE_DATA_COMMIT				5
#define DPROBE_DATA_VALID				6
#define DPROBE_CLK_1024					7
#define DPROBE_VAL_WINDOW				8

#define GAIN_SEL_AUTO					0
#define GAIN_SEL_HIGH					1
#define GAIN_SEL_LOW					2
#define GAIN_SEL_BOTH					3

#define FAST_SHAPER_INPUT_HGPA			0
#define FAST_SHAPER_INPUT_LGPA			1

#endif
