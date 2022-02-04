/*************************************************************************
*
*    Modified version of the NI-DAQ Windows Function Prototype
*
*    This include file has been modified to work the Keysight VEE 
*    Import Library object to support the NIDAQ32.DLL.
*
*    This modified version is based on the 6.6 version of NI-DAQ.
*
*    This include file is provided "as is", and Keysight Technologies makes 
*    no warranty of any kind with regard to this file.
*
*    A few notes for VEE users:
*
*    1. VEE does not support the unsigned short or the unsigned long data types.
*       Where the data type of a parameter has been modified, there is a 
*       comment indicating what the old data type was.
*
*    2. Going from unsigned long to signed long (or just long) you lose
*       some dynamic range.  Whereas unsigned longs can have a value 
*       between:
*
*       0 and 4,294,967,295
* 
*       signed longs can have values between:
*
*       -2,147,483,648 and +2,147,483,647
*
*    3. Going from unsigned short to signed short (or just short) you lose
*       some dynamic range.  Whereas unsigned shorts can have a value 
*       between:
*
*       0 and 65,535
* 
*       signed shorts can have values between:
*
*       -32,768 and +32,767
*
*    In VEE, when an unsigned long has a value > 2,147,483,647 or when
*    an unsigned short has a value > 32,767, the number will wrap around
*    and become negative.  For instance, when an unsigned short with 
*    a value of 32,768 has a value of -32,768 in VEE.  An unsigned short
*    value of 65,535 would have a value of -1 in VEE.  In VEE, the signed
*    short (Int16) value can be promoted to an signed long (Int32) with a
*    following formula that can be placed in a formula box:
*
*    (A < 0 ? A + 65536 : A)
*
*    Unfortunately, there is no larger integer type than Int32 in VEE. So when
*    handling longs in VEE that were meant to be unsigned longs you will
*    need to be aware of the limited dynamic range of positive numbers.
*
*************************************************************************/

short __stdcall AI_Change_Parameter (
	short      slot,
	short      channel,
	long       paramID,                // Changed from unsigned to signed long
	long       paramValue);            // Changed from unsigned to signed long
short __stdcall AI_Check (
	short      slot,
	short     *status,
	short     *value);
short __stdcall AI_Clear (
	short      slot);
short __stdcall AI_Configure (
	short      slot,
	short      chan,
	short      inputMode,
	short      inputRange,
	short      polarity,
	short      driveAIS);
short __stdcall AI_Mux_Config (
	short      slot,
	short      numMuxBrds);
short __stdcall AI_Read (
	short      slot,
	short      chan,
	short      gain,
	short     *value);
short __stdcall AI_Setup (
	short      slot,
	short      chan,
	short      gain);
short __stdcall AI_VRead (
	short      slot,
	short      chan,
	short      gain,
	double    *volts);
short __stdcall AI_VScale (
	short      slot,
	short      chan,
	short      gain,
	double     gainAdjust,
	double     offset,
	short      reading,
	double    *voltage);
short __stdcall Align_DMA_Buffer (
	short      slot,
	short      resource,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	long       bufSize,                // Changed from unsigned to signed long
	long      *alignIndex);            // Changed from unsigned to signed long
short __stdcall AO_Calibrate (
	short      board,
	short      operation,
	short      EEPROMloc);
short __stdcall AO_Configure (
	short      slot,
	short      chan,
	short      outputPolarity,
	short      IntOrExtRef,
	double     refVoltage,
	short      updateMode);
short __stdcall AO_Change_Parameter (
	short      slot,
	short      channel,
	long       paramID,                // Changed from unsigned to signed long
	long       paramValue);            // Changed from unsigned to signed long
short __stdcall AO_Update (
	short      slot);
short __stdcall AO_VWrite (
	short      slot,
	short      chan,
	double     voltage);
short __stdcall AO_Write (
	short      slot,
	short      chan,
	short      value);
short __stdcall Calibrate_E_Series (
	short      deviceNumber,
	long       calOp,                  // Changed from unsigned to signed long
	long       setOfCalConst,          // Changed from unsigned to signed long
	double     calRefVolts);
short __stdcall Calibrate_59xx (
	short      deviceNumber,
	long       operation,              // Changed from unsigned to signed long
	double     refVoltage);
short __stdcall Calibrate_DSA (
	short      deviceNumber,
	long       operation,              // Changed from unsigned to signed long
	double     refVoltage);
short __stdcall Config_Alarm_Deadband (
	short      slot,
	short      mode,
	char      *chanStr,
	double     trigLvl,
	double     deadbandWidth,
	long       handle,                 // Changed from HWND to long
	short      alarmOnMsg,
	short      alarmOffMsg,
	long       callbackAddr);          // Changed from unsigned to signed long
short __stdcall Config_ATrig_Event_Message (
	short      slot,
	short      mode,
	char      *chanStr,
	double     trigLvl,
	double     winSize,
	short      trigSlope,
	long       skipCnt,                // Changed from unsigned to signed long
	long       preTrigScans,           // Changed from unsigned to signed long
	long       postTrigScans,          // Changed from unsigned to signed long
	long       handle,                 // Changed from HWND to long
	short      msg,
	long       callBackAddr);          // Changed from unsigned to signed long
short __stdcall Config_DAQ_Event_Message (
	short      slot,
	short      mode,
	char      *chanStr,
	short      DAQEvent,
	long       trigVal0,
	long       trigVal1,
	long       skipCnt,                // Changed from unsigned to signed long
	long       preTrigScans,           // Changed from unsigned to signed long
	long       postTrigScans,          // Changed from unsigned to signed long
	long       handle,                 // Changed from HWND to long
	short      msg,
	long       callBackAddr);          // Changed from unsigned to signed long
short __stdcall Configure_HW_Analog_Trigger (
	short      deviceNumber,
	long       onOrOff,                // Changed from unsigned to signed long
	long       lowValue,
	long       highValue,
	long       mode,                   // Changed from unsigned to signed long
	long       trigSource);            // Changed from unsigned to signed long
short __stdcall CTR_Config (
	short      slot,
	short      ctr,
	short      edgeMode,
	short      gateMode,
	short      outType,
	short      outPolarity);
short __stdcall CTR_EvCount (
	short      slot,
	short      ctr,
	short      timebase,
	short      cont);
short __stdcall CTR_EvRead (
	short      slot,
	short      ctr,
	short     *overflow,
	short     *counts);                // Changed from unsigned to signed short
short __stdcall CTR_FOUT_Config (
	short      slot,
	short      FOUT,
	short      mode,
	short      timebase,
	short      division);
short __stdcall CTR_Period (
	short      slot,
	short      ctr,
	short      timebase);
short __stdcall CTR_Pulse (
	short      slot,
	short      ctr,
	short      timebase,
	short      delay,                  // Changed from unsigned to signed short
	short      pulseWidth);            // Changed from unsigned to signed short
short __stdcall CTR_Rate (
	double     freq,
	double     duty,
	short     *timebase,
	short     *period1,                // Changed from unsigned to signed short
	short     *period2);               // Changed from unsigned to signed short
short __stdcall CTR_Reset (
	short      slot,
	short      ctr,
	short      outState);
short __stdcall CTR_Restart (
	short      slot,
	short      ctr);
short __stdcall CTR_Simul_Op (
	short      slot,
	short      numCtrs,
	short     *ctrList,
	short      mode);
short __stdcall CTR_Square (
	short      slot,
	short      ctr,
	short      timebase,
	short      period1,                // Changed from unsigned to signed short
	short      period2);               // Changed from unsigned to signed short
short __stdcall CTR_State (
	short      slot,
	short      ctr,
	short     *outState);
short __stdcall CTR_Stop (
	short      slot,
	short      ctr);
short __stdcall DAQ_Check (
	short      slot,
	short     *progress,
	long      *retrieved);             // Changed from unsigned to signed long
short __stdcall DAQ_Clear (
	short      slot);
short __stdcall DAQ_Config (
	short      slot,
	short      startTrig,
	short      extConv);
short __stdcall DAQ_DB_Config (
	short      slot,
	short      dbMode);
short __stdcall DAQ_DB_HalfReady (
	short      slot,
	short     *halfReady,
	short     *status);
short __stdcall DAQ_DB_Transfer (
	short      slot,
	short     *hbuffer,
	long      *ptsTfr,                 // Changed from unsigned to signed long
	short     *status);
short __stdcall DAQ_Monitor (
	short      slot,
	short      chan,
	short      seq,
	long       monitorCnt,             // Changed from unsigned to signed long
	short     *monitorBuf,             // Changed from unsigned to signed long
	long      *newestIndex,            // Changed from unsigned to signed long
	short     *status);
short __stdcall DAQ_Op (
	short      slot,
	short      chan,
	short      gain,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate);
short __stdcall DAQ_Rate (
	double     rate,
	short      units,
	short     *timebase,
	short     *sampleInt);             // Changed from unsigned to signed short
short __stdcall DAQ_Start (
	short      slot,
	short      chan,
	short      gain,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	short      timebase,
	short      sampInt);               // Changed from unsigned to signed short
short __stdcall DAQ_StopTrigger_Config (
	short      slot,
	short      preTrig,
	long       preTrigCnt);            // Changed from unsigned to signed long
short __stdcall DAQ_to_Disk (
	short      slot,
	short      chan,
	short      gain,
	char      *fileName,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate,
	short      concat);
short __stdcall DAQ_VScale (
	short      slot,
	short      chan,
	short      gain,
	double     gainAdjust,
	double     offset,
	long       cnt,                    // Changed from unsigned to signed long
	short     *binArray,
	double    *voltArray);
short __stdcall DIG_Block_Check (
	short      slot,
	short      grp,
	long      *remaining);             // Changed from unsigned to signed long
short __stdcall DIG_Block_Clear (
	short      slot,
	short      grp);
short __stdcall DIG_Block_In (
	short      slot,
	short      grp,
	short     *buffer,
	long       cnt);                   // Changed from unsigned to signed long
short __stdcall DIG_Block_Out (
	short      slot,
	short      grp,
	short     *buffer,
	long       cnt);                   // Changed from unsigned to signed long
short __stdcall DIG_Block_PG_Config (
	short      slot,
	short      grp,
	short      PGmode,
	short      reqSource,
	short      timebase,
	short      interval,               // Changed from unsigned to signed short
	short      alGate);
short __stdcall DIG_DB_Config (
	short      slot,
	short      grp,
	short      DBMode,
	short      oldDataStop,
	short      partialTransfer);
short __stdcall DIG_DB_HalfReady (
	short      slot,
	short      grp,
	short     *halfReady);
short __stdcall DIG_DB_Transfer (
	short      slot,
	short      grp,
	short     *halfBuffer,
	long       ptsTfr);                // Changed from unsigned to signed long
short __stdcall DIG_Grp_Config (
	short      slot,
	short      grp,
	short      grpsize,
	short      port,
	short      direction);
short __stdcall DIG_Grp_Mode (
	short      slot,
	short      grp,
	short      sigType,
	short      edge,
	short      reqpol,
	short      ackpol,
	short      settleTime);
short __stdcall DIG_Grp_Status (
	short      slot,
	short      grp,
	short     *status);
short __stdcall DIG_In_Grp (
	short      slot,
	short      grp,
	short     *grp_pat);
short __stdcall DIG_In_Line (
	short      slot,
	short      port,
	short      linenum,
	short     *state);
short __stdcall DIG_In_Port (
	short      slot,
	short      port,
	short     *pattern);
short __stdcall DIG_Line_Config (
	short      slot,
	short      port,
	short      linenum,
	short      direction);
short __stdcall DIG_Out_Grp (
	short      slot,
	short      grp,
	short      grp_pat);
short __stdcall DIG_Out_Line (
	short      slot,
	short      port,
	short      linenum,
	short      state);
short __stdcall DIG_Out_Port (
	short      slot,
	short      port,
	short      pattern);
short __stdcall DIG_Prt_Config (
	short      slot,
	short      port,
	short      latch_mode,
	short      direction);
short __stdcall DIG_Prt_Status (
	short      slot,
	short      port,
	short     *status);
short __stdcall DIG_SCAN_Setup (
	short      slot,
	short      grp,
	short      numPorts,
	short     *portList,
	short      direction);
short __stdcall Get_DAQ_Device_Info (
	short      deviceNumber,
	long       infoType,               // Changed from unsigned to signed long
	long      *infoVal);               // Changed from unsigned to signed long
short __stdcall Get_NI_DAQ_Version (
	long      *version);               // Changed from unsigned to signed long
short __stdcall GPCTR_Config_Buffer (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       reserved,               // Changed from unsigned to signed long
	long       numPoints,              // Changed from unsigned to signed long
	long      *buffer);                // Changed from unsigned to signed long
short __stdcall GPCTR_Read_Buffer (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       readMode,               // Changed from unsigned to signed long
	long       readOffset,
	long       numPointsToRead,        // Changed from unsigned to signed long
	double     timeOut,
	long      *numPointsRead,          // Changed from unsigned to signed long
	long      *buffer);                // Changed from unsigned to signed long
short __stdcall Line_Change_Attribute (
	short      deviceNumber,
	long       lineNumber,             // Changed from unsigned to signed long
	long       attribID,               // Changed from unsigned to signed long
	long       attribValue);           // Changed from unsigned to signed long
short __stdcall GPCTR_Control (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       action);                // Changed from unsigned to signed long
short __stdcall GPCTR_Set_Application (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       application);           // Changed from unsigned to signed long
short __stdcall GPCTR_Watch (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       watchID,                // Changed from unsigned to signed long
	long      *watchValue);            // Changed from unsigned to signed long
short __stdcall ICTR_Read (
	short      slot,
	short      counter,
	short     *cnt);                   // Changed from unsigned to signed short
short __stdcall ICTR_Reset (
	short      slot,
	short      counter,
	short      state);
short __stdcall ICTR_Setup (
	short      slot,
	short      counter,
	short      mode,
	short      cnt,                    // Changed from unsigned to signed short
	short      binBCD);
short __stdcall Init_DA_Brds (
	short      slot,
	short     *brdCode);
short __stdcall Lab_ISCAN_Check (
	short      slot,
	short     *status,
	long      *retrieved,              // Changed from unsigned to signed long
	short     *finalScanOrder);
short __stdcall Lab_ISCAN_Op (
	short      slot,
	short      numChans,
	short      gain,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate,
	double     scanRate,
	short     *finalScanOrder);
short __stdcall Lab_ISCAN_Start (
	short      slot,
	short      numChans,
	short      gain,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	short      timebase,
	short      sampleInt,              // Changed from unsigned to signed short
	short      scanInt);               // Changed from unsigned to signed short
short __stdcall Lab_ISCAN_to_Disk (
	short      slot,
	short      numChans,
	short      gain,
	char      *fileName,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate,
	double     scanRate,
	short      concat);
short __stdcall LPM16_Calibrate (
	short      slot);
short __stdcall MIO_Config (
	short      slot,
	short      dither,
	short      useAMUX);
short __stdcall RTSI_Clear (
	short      slot);
short __stdcall RTSI_Clock (
	short      slot,
	short      connect,
	short      direction);
short __stdcall RTSI_Conn (
	short      slot,
	short      brdSignal,
	short      busLine,
	short      direction);
short __stdcall RTSI_DisConn (
	short      slot,
	short      brdSignal,
	short      busLine);
short __stdcall SC_2040_Config (
	short      deviceNumber,
	short      channel,
	short      sc2040Gain);
short __stdcall SCAN_Demux (
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	short      numChans,
	short      muxMode);
short __stdcall SCAN_Op (
	short      slot,
	short      numChans,
	short     *chans,
	short     *gains,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate,
	double     scanRate);
short __stdcall SCAN_Sequence_Demux (
	short      numChans,
	short     *chanVector,
	long       bufferSize,             // Changed from unsigned to signed long
	short     *buffer,
	short      samplesPerSequence,
	short     *scanSequenceVector,
	long      *samplesPerChanVector);  // Changed from unsigned to signed long
short __stdcall SCAN_Sequence_Retrieve (
	short      deviceNumber,
	short      samplesPerSequence,
	short     *scanSequenceVector);
short __stdcall SCAN_Sequence_Setup (
	short      deviceNumber,
	short      numChans,
	short     *chanVector,
	short     *gainVector,
	short     *scanRateDivVector,
	short     *scansPerSequence,
	short     *samplesPerSequence);
short __stdcall SCAN_Setup (
	short      slot,
	short      num_chans,
	short     *chans,
	short     *gains);
short __stdcall SCAN_Start (
	short      slot,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	short      tb1,
	short      si1,                    // Changed from unsigned to signed short
	short      tb2,
	short      si2);                   // Changed from unsigned to signed short
short __stdcall SCAN_to_Disk (
	short      slot,
	short      numChans,
	short     *chans,
	short     *gains,
	char      *fileName,
	long       cnt,                    // Changed from unsigned to signed long
	double     sampleRate,
	double     scanRate,
	short      concat);
short __stdcall Calibrate_1200 (
	short      deviceNumber,
	short      calOP,
	short      saveNewCal,
	short      EEPROMloc,
	short      calRefChan,
	short      grndRefChan,
	short      DAC0chan,
	short      DAC1chan,
	double     calRefVolts,
	double     gain);
short __stdcall SCXI_AO_Write (
	short      chassisID,
	short      moduleSlot,
	short      DACchannel,
	short      opCode,
	short      rangeCode,
	double     voltCurrentData,
	short      binaryDat,
	short     *binaryWritten);
short __stdcall SCXI_Cal_Constants (
	short      chassisID,
	short      moduleSlot,
	short      SCXIchannel,
	short      operation,
	short      calArea,
	short      rangeCode,
	double     SCXIgain,
	short      DAQdevice,
	short      DAQchannel,
	short      DAQgain,
	double     TBgain,
	double     volt1,
	double     binary1,
	double     volt2,
	double     binary2,
	double    *binEEprom1,
	double    *binEEprom2);
short __stdcall SCXI_Calibrate (
	short      chassisID,
	short      moduleSlot,
	short      moduleChan,
	short      operation,
	short      calArea,
	double     SCXIgain,
	double     inputRefVoltage,
	short      DAQdevice,
	short      DAQchan);
short __stdcall SCXI_Calibrate_Setup (
	short      chassisID,
	short      moduleSlot,
	short      calOp);
short __stdcall SCXI_Change_Chan (
	short      chassisID,
	short      moduleSlot,
	short      chan);
short __stdcall SCXI_Configure_Filter (
	short      chassisID,
	short      moduleSlot,
	short      channel,
	short      filterMode,
	double     freq,
	short      cutoffDivDown,          // Changed from unsigned to signed short
	short      outClkDivDown,          // Changed from unsigned to signed short
	double    *actFreq);
short __stdcall SCXI_Get_Chassis_Info (
	short      chassisID,
	short     *chassisType,
	short     *address,
	short     *commMode,
	short     *commPath,
	short     *numSlots);
short __stdcall SCXI_Get_Module_Info (
	short      chassisID,
	short      slot,
	long      *modulePresent,
	short     *opMode,
	short     *DAQboard);
short __stdcall SCXI_Get_State (
	short      chassisID,
	short      moduleSlot,
	short      port,
	short      channel,
	long      *data);                  // Changed from unsigned to signed long
short __stdcall SCXI_Get_Status (
	short      chassisID,
	short      moduleSlot,
	short      wait,
	long      *data);                  // Changed from unsigned to signed long
short __stdcall SCXI_Load_Config (
	short      chassisID);
short __stdcall SCXI_MuxCtr_Setup (
	short      slot,
	short      enable,
	short      scanDiv,
	short      muxCtrVal);             // Changed from unsigned to signed short
short __stdcall SCXI_Reset (
	short      chassisID,
	short      moduleSlot);
short __stdcall SCXI_Scale (
	short      chassisID,
	short      moduleSlot,
	short      SCXIchannel,
	double     SCXIgain,
	double     TBgain,
	short      DAQdevice,
	short      DAQchannel,
	short      DAQgain,
	long       numPoints,              // Changed from unsigned to signed long
	short     *binArray,
	double    *voltArray);
short __stdcall SCXI_SCAN_Setup (
	short      chassisID,
	short      numModules,
	short     *modules,
	short     *numChans,
	short     *startChans,
	short      DAQboard,
	short      modeFlag);
short __stdcall SCXI_Set_Config (
	short      chassisID,
	short      chassisType,
	short      address,
	short      commMode,
	short      slotOrCOMM,
	short      numSlots,
	long      *moduleTypes,
	short     *opModes,
	short     *DAQboards);
short __stdcall SCXI_Set_Gain (
	short      chassisID,
	short      moduleSlot,
	short      channel,
	double     gain);
short __stdcall SCXI_Set_Input_Mode (
	short      chassisID,
	short      moduleSlot,
	short      inputMode);
short __stdcall SCXI_Set_State (
	short      chassisID,
	short      moduleSlot,
	short      port,
	short      channel,
	long       data);                  // Changed from unsigned to signed long
short __stdcall SCXI_Single_Chan_Setup (
	short      chassisID,
	short      moduleSlot,
	short      chan,
	short      DAQboard);
short __stdcall SCXI_Track_Hold_Control (
	short      chassisID,
	short      moduleSlot,
	short      state,
	short      DAQboard);
short __stdcall SCXI_Track_Hold_Setup (
	short      chassisID,
	short      moduleSlot,
	short      mode,
	short      source,
	short      send,
	short      holdCnt,
	short      DAQboard);
short __stdcall Select_Signal (
	short      deviceNumber,
	long       signal,                 // Changed from unsigned to signed long
	long       source,                 // Changed from unsigned to signed long
	long       sourceSpec);            // Changed from unsigned to signed long
short __stdcall Set_DAQ_Device_Info (
	short      deviceNumber,
	long       infoType,               // Changed from unsigned to signed long
	long       infoVal);               // Changed from unsigned to signed long
short __stdcall Timeout_Config (
	short      slot,
	long       numTicks);
short __stdcall WFM_Chan_Control (
	short      slot,
	short      channel,
	short      operation);
short __stdcall WFM_Check (
	short      slot,
	short      channel,
	short     *progress,
	long      *itersDone,              // Changed from unsigned to signed long
	long      *pointsDone);            // Changed from unsigned to signed long
short __stdcall WFM_ClockRate (
	short      slot,
	short      group,
	short      whickClock,
	short      timebase,
	long       updateInterval,         // Changed from unsigned to signed long
	short      mode);
short __stdcall WFM_DB_Config (
	short      slot,
	short      numChans,
	short     *chanVect,
	short      DBMode,
	short      oldDataStop,
	short      partialTransfer);
short __stdcall WFM_DB_HalfReady (
	short      slot,
	short      numChans,
	short     *chanVect,
	short     *halfReady);
short __stdcall WFM_DB_Transfer (
	short      slot,
	short      numChans,
	short     *chanVect,
	short     *buffer,
	long       cnt);                   // Changed from unsigned to signed long
short __stdcall WFM_from_Disk (
	short      slot,
	short      numChans,
	short     *chanVect,
	char      *fileName,
	long       startPts,               // Changed from unsigned to signed long
	long       endPts,                 // Changed from unsigned to signed long
	long       iterations,             // Changed from unsigned to signed long
	double     rate);
short __stdcall WFM_Group_Control (
	short      slot,
	short      group,
	short      operation);
short __stdcall WFM_Group_Setup (
	short      slot,
	short      numChans,
	short     *chanVect,
	short      group);
short __stdcall WFM_Load (
	short      slot,
	short      numChans,
	short     *chanVect,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	long       iterations,             // Changed from unsigned to signed long
	short      mode);
short __stdcall WFM_Op (
	short      slot,
	short      numChans,
	short     *chanVect,
	short     *buffer,
	long       cnt,                    // Changed from unsigned to signed long
	long       iterations,             // Changed from unsigned to signed long
	double     rate);
short __stdcall WFM_Rate (
	double     rate,
	short      units,
	short     *timebase,
	long      *updateInterval);        // Changed from unsigned to signed long
short __stdcall WFM_Scale (
	short      slot,
	short      chan,
	long       cnt,                    // Changed from unsigned to signed long
	double     gain,
	double    *voltArray,
	short     *binArray);
short __stdcall AI_Read_Scan (
	short      slot,
	short     *reading);
short __stdcall AI_VRead_Scan (
	short      slot,
	double    *reading);
short __stdcall SCXI_ModuleID_Read (
	short      scxiID,
	short      moduleSlot,
	long      *id);
short __stdcall AO_VScale (
	short      slot,
	short      chan,
	double     voltage,
	short     *value);
short __stdcall GPCTR_Change_Parameter (
	short      deviceNumber,
	long       gpCounterNumber,        // Changed from unsigned to signed long
	long       paramID,                // Changed from unsigned to signed long
	long       paramValue);            // Changed from unsigned to signed long
short __stdcall DIG_Trigger_Config (
	short      slot,
	short      grp,
	short      startTrig,
	short      startPol,
	short      stopTrig,
	short      stopPol,
	long       ptsAfterStopTrig,       // Changed from unsigned to signed long
	long       pattern,                // Changed from unsigned to signed long
	long       patternMask);           // Changed from unsigned to signed long
short __stdcall SCXI_Set_Threshold (
	short      chassisID,
	short      moduleSlot,
	short      channel,
	double     threshHold,
	double     hysteresis);
short __stdcall WFM_Set_Clock (
	short      slot,
	short      group,
	long       whichClock,             // Changed from unsigned to signed long
	double     desiredRate,
	long       units,                  // Changed from unsigned to signed long
	double    *actualRate);
short __stdcall DAQ_Set_Clock (
	short      slot,
	long       whichClock,             // Changed from unsigned to signed long
	double     desiredRate,
	long       units,                  // Changed from unsigned to signed long
	double    *actualRate);
short __stdcall Tio_Select_Signal (
	short      deviceNumber,
	long       signal,                 // Changed from unsigned to signed long
	long       source,                 // Changed from unsigned to signed long
	long       sourceSpec);            // Changed from unsigned to signed long
short __stdcall Tio_Combine_Signals (
	short      deviceNumber,
	long       internalLine,           // Changed from unsigned to signed long
	long       logicalExpression);     // Changed from unsigned to signed long
short __stdcall DIG_In_Prt (
	short      slot,
	short      port,
	long      *pattern);
short __stdcall DIG_Out_Prt (
	short      slot,
	short      port,
	long       pattern);
short __stdcall AI_Get_Overloaded_Channels (
	short      deviceNumber,
	short     *numChannels,
	short     *channelList);
short __stdcall Calibrate_TIO (
	short      deviceNumber,
	long       operation,              // Changed from unsigned to signed long
	long       setOfCalConst,          // Changed from unsigned to signed long
	double     referenceFreq);
short __stdcall DIG_Change_Message_Config (
	short      deviceNumber,
	short      operation,
	char      *riseChanStr,
	char      *fallChanStr,
	long       handle,                 // Changed from HWND to long
	short      msg,
	long       callBackAddr);          // Changed from unsigned to signed long
short __stdcall DIG_Change_Message_Control (
	short      deviceNumber,
	short      ctrlCode);
short __stdcall DIG_Filter_Config (
	short      deviceNumber,
	short      mode,
	char      *chanStr,
	double     interval);



/* Obsolete Functions */


short __stdcall Get_DAQ_Event (
	long       timeOut,                // Changed from unsigned to signed long
	short     *handle,
	short     *msg,
	short     *wParam,
	long      *lParam);
short __stdcall Peek_DAQ_Event (
	long       timeOut,                // Changed from unsigned to signed long
	short     *handle,
	short     *msg,
	short     *wParam,
	long      *lParam);
short __stdcall REG_Level_Read (
	short      slot,
	short      registerIndex,
	long      *registerValue);         // Changed from unsigned to signed long
short __stdcall REG_Level_Write (
	short      slot,
	short      registerIndex,
	long       bitsAffected,           // Changed from unsigned to signed long
	long       bitSettings,            // Changed from unsigned to signed long
	long      *registerValue);         // Changed from unsigned to signed long

short __stdcall USE_E_Series ( void);
short __stdcall USE_E_Series_AI ( void);
short __stdcall USE_E_Series_AO ( void);
short __stdcall USE_E_Series_DIO ( void);
short __stdcall USE_E_Series_GPCTR ( void);
short __stdcall USE_E_Series_GPCTR_Simple ( void);
short __stdcall USE_E_Series_Misc ( void);
short __stdcall USE_E_Series_WFM ( void);
short __stdcall USE_E_Series_DAQ ( void);




