/******************************************************************************
*  
* File:  CallVEE.cpp
*
* Description:
*
*   This example shows how to do use the Callable VEE Automation server from
*   C++.
*
* Requirements: If running on Windows 95, DCOM has to be installed. YOU
*               ALSO HAVE TO CONFIGURE THE CALLABLE VEE SERVER TO EXECUTE
*               USING THE IDENTITY OF "THE INTERACTIVE USER".  TO DO THIS,
*               FOLLOW THESE STEPS:
*
*               1. Select Start -> Run, and type in "dcomcnfg"
*               2. Select "Keysight Callable VEE Server" on the Applications
*                  tab then press the Properties button.
*               3. Select the Identity tab and select "The interactive user".
*               4. Press Apply.
*
*               NOTE: IF YOU DON"T FOLLOW THESE STEPS, VEE WILL HANG ON THE
*                     REMOTE COMPUTER IF YOU SELECT DEBUG MODE BECAUSE VEE
*                     WILL WAIT FOR YOU TO PRESS RUN OR STEP.  HOWEVER, VEE
*                     WILL BE RUNNING IN A HIDDEN WINDOWSTATION ON NT/2000
*                     THAT YOU CAN"T SEE.  IF THIS HAPPENS, END THE VEE
*                     PROCESS. THIS PROGRAM WILL THEN RETURN WITH AN ERROR.
*
*               If you run into "Access denied" errors, then run dcomcnfg again
*               on the remote computer.  Bring up the Callable VEE Server
*               Properties and select the Security tab.  Make sure you have
*               both Access and Launch permissions.
*
* Execution Instructions:
*
*   From the command line, type the following (or double-click this file in
*	the Windows Explorer)
*
*	CallVEE.exe [hostname] [geometry (800x600+10+10)]
*
* This example is provided as an illustration "as is", and
* Keysight Technologies makes no warranty of any kind with regard to this
* example.
*
* Copyright (c) 2000-2018 Keysight Technologies.
*
******************************************************************************/
#define _WIN32_DCOM		// To get CoCreateInstanceEx prototype
#include <windows.h>
#include <stdio.h>


// Pull in the CallableVEE Type Library to get access to smart pointers,
// property/method definitions, etc.
#import "..\..\..\callserv.tlb" no_namespace named_guids

// Path the VEE library file
#define LIB_PATH "~installDir/examples/CallableVEE/lib.vee"

// This macro provides a more type safe version of QueryInterface. Telling the
// compiler to cast the second parameter using the C-style cast "(void**)" 
// allows anything to pass a compile time check. Using static_cast and 
// reinterpret_cast in this manner is much safer.
#define SAFEQI(Type, Expr) QueryInterface(IID_##Type, \
                           reinterpret_cast<void**>(static_cast<Type**>(Expr)))

// Function prototypes
inline void     dumpSystemErrorInfo(HRESULT hr);
inline void     testHR(HRESULT _hr, char* msg);
inline wchar_t *strAnsiToWide(char *pszStr);


int main(int argc, char** argv)
{
	HRESULT             hr;
	bool                bDone;
	char*               pszHostname;
	char*               pszGeometry;
    char                response[128];
	long                rgIndices[2];
	long                ndx;
	long                width, height, top, left;
	double              dReferenceDelay;
	double              upperLimit[11][2];
	double              lowerLimit[13][2];
	VARIANT_BOOL        bDebug;
	_CallServerPtr      pCallServer;
	LibraryPtr          pLib;
	UserFunctionsPtr    pUserFuncColl;
	UserFunctionPtr     pSynthesizePulseUF;
	UserFunctionPtr     pDisplayTestUF;
	VeeDataContainerPtr pErrorPointsVDC;
	VeeDataContainerPtr pPulseVDC;
	VeeDataContainerPtr pUpLimitVDC;
	VeeDataContainerPtr pLoLimitVDC;
	_bstr_t             bstrName;
	_variant_t          varIndex;
	_variant_t          varParam;
	_variant_t          varInputs;
	_variant_t          varOutputs;
	SAFEARRAY*          pSaIn;
	SAFEARRAY*          pSaOut;
	SAFEARRAY*          pSa;
	SAFEARRAYBOUND      rgsaBound[2];
	void*               pvArrayElement;

	// Initialize locals
	pszHostname = NULL;
	pszGeometry = NULL;
	bDone  = FALSE;
	pSaIn  = 0;
	pSaOut = 0;

	// Parse command line options
	if (argc > 3)
	{
		printf("\nUsage: CallVEE.exe [hostname] [geometry (800x600+10+10)]\n");
		exit(1);
	}

	if (argc >= 2)
		pszHostname = argv[1];				

	if (argc == 3)
		pszGeometry = argv[2];	

	// Initialize the COM Library
	CoInitialize(NULL);

	try 
	{
		if (!pszHostname)
		{
			// Create the CallServer server object on the local computer
			pCallServer.CreateInstance(CLSID_CallServer);
		}
		else
		{
			COSERVERINFO serverinfo;
			MULTI_QI     qi[1] = {&IID__CallServer, NULL, 0};

			// hostname specified, use CoCreateInstanceEx to create object
			// on the remote computer specified by pszHostname.
			serverinfo.dwReserved1 = 0;
			serverinfo.dwReserved2 = 0;
			serverinfo.pwszName    = strAnsiToWide(pszHostname);
			serverinfo.pAuthInfo   = NULL;

			// This call actually creates the object on the remote host.
			hr = CoCreateInstanceEx(CLSID_CallServer, NULL, 
			                        CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, 
			                        &serverinfo, 1, qi);

			// Free allocated memory
			free(serverinfo.pwszName);

			testHR(hr, "Failed to create remote server.\nCheck DCOM "
			           "permissions on remote host using DCOMCNFG.EXE.");

			testHR(qi[0].hr, "QueryInterface for IID__CallServer failed on "
			                 "remote host.");

			// Attach the interface pointer we got back to our smart pointer
			pCallServer.Attach(reinterpret_cast<_CallServer*>(qi[0].pItf));
		}

		// Test if object creation succeeded
		if (pCallServer == NULL)
			throw "Failed to create the CallableVEE CallServer object";

		// If user specified geometry then set up the CallServer appropriately
		if (pszGeometry)
		{
			width = 800;
			height = 600;
			top = left = 10;
			sscanf(pszGeometry, "%dx%d+%d+%d", &width, &height, &left, &top);
			pCallServer->PutWidth(width);
			pCallServer->PutHeight(height);
			pCallServer->PutTop(top);
			pCallServer->PutLeft(left);
		}

		// Load the VEE User Function library
		_bstr_t bstrLibPath(LIB_PATH);
		pLib = pCallServer->GetLibraries()->Load(bstrLibPath);
        printf("Loaded library '%s'\n", (char*)pLib->GetPath());

		// Print out the names of the UserFunctions in this library.
		pUserFuncColl = pLib->GetUserFunctions();
		long nNumFuncs = pUserFuncColl->GetCount();
		for (ndx = 0 ; ndx < nNumFuncs ; ndx++)
		{
			varIndex = ndx;
			UserFunctionPtr pUserFunc = pUserFuncColl->GetItem(varIndex);
			printf("Function name %d: %s\n", ndx, (char*)pUserFunc->GetName());
		}
		printf("\n");

		// Print out some info on the Synthesize_Pulse User Function
		varIndex = "Synthesize_Pulse";
		pSynthesizePulseUF = pUserFuncColl->GetItem(varIndex);
		bstrName = pSynthesizePulseUF->GetName();
		printf("Name: %s, Number inputs: %d, Number outputs: %d\n", 
		       (char*)bstrName,
		       pSynthesizePulseUF->GetInputArguments()->GetCount(),
		       pSynthesizePulseUF->GetOutputArguments()->GetCount());

		// Print out some info on the Display_Test User Function
		varIndex = "Display_Test";
		pDisplayTestUF = pUserFuncColl->GetItem(varIndex);
		bstrName = pDisplayTestUF->GetName();
		printf("Name: %s, Number inputs: %d, Number outputs: %d\n\n", 
		       (char*)bstrName,
		       pDisplayTestUF->GetInputArguments()->GetCount(),
		       pDisplayTestUF->GetOutputArguments()->GetCount());

		//
		// Build the upper limit mask
		//
		upperLimit[0][0]  =  0.0;     // xval
		upperLimit[0][1]  =  0.05;    // yval
        upperLimit[1][0]  =  2.5E-07;
        upperLimit[1][1]  =  0.05;
        upperLimit[2][0]  =  3.25E-07;
        upperLimit[2][1]  =  0.8;
        upperLimit[3][0]  =  3.26E-07;
        upperLimit[3][1]  =  1.15;
        upperLimit[4][0]  =  4.25E-07;
        upperLimit[4][1]  =  1.15;
        upperLimit[5][0]  =  5.000000000000001E-07;
        upperLimit[5][1]  =  1.05;
        upperLimit[6][0]  =  6.75E-07;
        upperLimit[6][1]  =  1.05;
        upperLimit[7][0]  =  7.250000000000002E-07;
        upperLimit[7][1]  = -0.07000000000000001;
        upperLimit[8][0]  =  1.1E-06;
        upperLimit[8][1]  =  0.05;
        upperLimit[9][0]  =  1.25E-06;
        upperLimit[9][1]  =  0.05;
        upperLimit[10][0] =  1.75E-06;
        upperLimit[10][1] =  0.05;

		// Create SAFEARRAY to put data into
		// The Create2DCoord1DArray method of VeeDataContainer expects the
		// data as a n by 2 safearray of doubles.
		rgsaBound[0].lLbound = 0;
		rgsaBound[0].cElements = 11;
		rgsaBound[1].lLbound = 0;
		rgsaBound[1].cElements = 2;
		pSa = SafeArrayCreate(VT_R8, 2, rgsaBound);
		if (!pSa)
			throw "Upper limit SafeArrayCreate failed";

		// Put data into SAFEARRAY
		for (ndx = 0 ; ndx < 11 ; ndx++)
		{
			rgIndices[0] = ndx;
			rgIndices[1] = 0;
			pvArrayElement = static_cast<void*>(&upperLimit[ndx][0]);
			hr = SafeArrayPutElement(pSa, rgIndices, pvArrayElement);
			testHR(hr, "Failed to fill in upper limit array!");

			rgIndices[1] = 1;
			pvArrayElement = static_cast<void*>(&upperLimit[ndx][1]);
			hr = SafeArrayPutElement(pSa, rgIndices, pvArrayElement);
			testHR(hr, "Failed to fill in upper limit array!");
		}

		// Now create a VeeDataContainer that will be one of the 
		// arguments in the Display_Test UserFunction call
		pUpLimitVDC = pCallServer->CreateVeeDataContainer();
		V_VT(&varParam) = VT_ARRAY | VT_R8;
		V_ARRAY(&varParam) = pSa;
		pUpLimitVDC->Create2DCoord1DArray(varParam);

		// Free the upperLimit SAFEARRAY
		varParam.Clear();
		pSa = 0;

		//
		// Build the lower limit mask
		//
        lowerLimit[0][0]  =  0.0;  // xval
        lowerLimit[0][1]  = -0.05; // yval
        lowerLimit[1][0]  =  3.499999999999999E-07;
        lowerLimit[1][1]  = -0.05;
        lowerLimit[2][0]  =  3.51E-07;
        lowerLimit[2][1]  =  0.5;
        lowerLimit[3][0]  =  4E-07;
        lowerLimit[3][1]  =  0.95;
        lowerLimit[4][0]  =  5.000000000000001E-07;
        lowerLimit[4][1]  =  0.95;
        lowerLimit[5][0]  =  6.000000000000001E-07;
        lowerLimit[5][1]  =  0.9;
        lowerLimit[6][0]  =  6.5E-07;
        lowerLimit[6][1]  =  0.5;
        lowerLimit[7][0]  =  6.51E-07;
        lowerLimit[7][1]  = -0.45;
        lowerLimit[8][0]  =  8.000000000000001E-07;
        lowerLimit[8][1]  = -0.45;
        lowerLimit[9][0]  =  9.25E-07;
        lowerLimit[9][1]  = -0.2;
        lowerLimit[10][0] =  1.1E-06;
        lowerLimit[10][1] = -0.05;
        lowerLimit[11][0] =  1.25E-06;
        lowerLimit[11][1] = -0.05;
        lowerLimit[12][0] =  1.75E-06;
        lowerLimit[12][1] = -0.05;

		// Create SAFEARRAY to put data into
		// The Create2DCoord1DArray method of VeeDataContainer expects the
		// data as a n by 2 safearray of doubles.
		rgsaBound[0].lLbound = 0;
		rgsaBound[0].cElements = 13;
		rgsaBound[1].lLbound = 0;
		rgsaBound[1].cElements = 2;
		pSa = SafeArrayCreate(VT_R8, 2, rgsaBound);
		if (!pSa)
			throw "Lower limit SafeArrayCreate failed";

		// Put data into SAFEARRAY
		for (ndx = 0 ; ndx < 13 ; ndx++)
		{
			rgIndices[0] = ndx;
			rgIndices[1] = 0;
			pvArrayElement = static_cast<void*>(&lowerLimit[ndx][0]);
			hr = SafeArrayPutElement(pSa, rgIndices, pvArrayElement);
			testHR(hr, "Failed to fill in lower limit array!");

			rgIndices[1] = 1;
			pvArrayElement = static_cast<void*>(&lowerLimit[ndx][1]);
			hr = SafeArrayPutElement(pSa, rgIndices, pvArrayElement);
			testHR(hr, "Failed to fill in lower limit array!");
		}

		// Now create a VeeDataContainer that will be one of the 
		// arguments in the Display_Test UserFunction call
		pLoLimitVDC = pCallServer->CreateVeeDataContainer();
		V_VT(&varParam) = VT_ARRAY | VT_R8;
		V_ARRAY(&varParam) = pSa;
		pLoLimitVDC->Create2DCoord1DArray(varParam);

		// Free the upperLimit SAFEARRAY
		varParam.Clear();
		pSa = 0;

	    printf("This program simulates a Telecomm Pulse Test.\n\n");

		//
		// Continue to run test as long as user wants to.
		//
		while (!bDone)
		{
			// Ask user if they want to run in debug mode?
	        do 
	        {
	            printf("Run in debug mode (y or n)? >");
	            gets(response);
	        } 
	        while(strncmp(response, "y", 1) && strncmp(response, "n", 1));

			if (strncmp(response, "y", 1))
				bDebug = VARIANT_FALSE;
			else
				bDebug = VARIANT_TRUE;

			// Set the debug mode
			pCallServer->PutDebug(bDebug);

			// Get the delay value from the use
	        printf("Enter Reference Delay value in microseconds\n");
	        do 
	        {
	            printf("Valid numbers are between .25u and .45u\n");
	            printf("> ");
	            scanf("%lg", &dReferenceDelay);
	            gets(response);
	        } 
	        while ((dReferenceDelay < .25) || (dReferenceDelay > .45));

	        // Convert to seconds
	        dReferenceDelay *= 1.0E-06;

			//
			// Prepare the inputs for the Synthesize_Pulse User Function
			//

			// Create a SAFEARRAY for input arguments - lower bound 0, size 1
			pSaIn = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			if (!pSaIn)
				throw "SafeArrayCreateVector failed";

			// Prepare the inputs array which must be a SAFEARRAY of type 
			// VT_VARIANT.
			// If there are no inputs, you may pass a VARIANT with the type
			// VT_ERROR and value 0x80020004.  This particular funtion only
			// takes one parameter.
			V_VT(&varParam) = VT_R8;
			V_R8(&varParam) = dReferenceDelay;
			pvArrayElement = static_cast<void*>(&varParam);
			rgIndices[0] = 0;
			hr = SafeArrayPutElement(pSaIn, rgIndices, pvArrayElement);
			testHR(hr, "Failed to fill in Synthesize_Pulse input args array!");

			// Prepare the inputs parameter to call.  The "inputs" parameter is
			// a VARIANT that contains the SAFEARRAY of VARIANTs we built above.
			V_VT(&varInputs) = VT_ARRAY | VT_VARIANT;
			V_ARRAY(&varInputs) = pSaIn;

			// Prepare the outputs array, in this case we just pass in an empty 
			// SAFEARRAY of type VT_VARIANT.  If there are no outputs, you may
			// pass a VARIANT with the type VT_ERROR and value 0x80020004.
			V_VT(&varOutputs) = VT_BYREF | VT_ARRAY | VT_VARIANT;
			V_ARRAYREF(&varOutputs) = &pSaOut;

			//
			// Call the VEE UserFunction - Synthesize_Pulse
			//
			pSynthesizePulseUF->Call(varInputs, &varOutputs);

			// 
			// Grab the output of Synthesize_Pulse
			//

			// The output comes back as a SAFEARRAY of type VT_VARIANT.  I know
			// that this function has one output of type 
			// VT_DISPATCH (VeeDataContainerPtr).
			rgIndices[0] = 0;
			pvArrayElement = static_cast<void*>(&varParam);
			hr = SafeArrayGetElement(pSaOut, rgIndices, pvArrayElement);
			testHR(hr, "Could not retrieve Synthesize_Pulse result");

			pPulseVDC = V_DISPATCH(&varParam);
			varParam.Clear();
		
			// Free inputs & outputs variants and their contents
			varInputs.Clear();
			varOutputs.Clear();
			pSaIn = 0;
			pSaOut = 0;

			//
			// Prepare the inputs for the Display_Test User Function
			//

			// Create a SAFEARRAY for input arguments - lower bound 0, size 3
			pSaIn = SafeArrayCreateVector(VT_VARIANT, 0, 3);
			if (!pSaIn)
				throw "SafeArrayCreateVector failed";

			// First input parameter RefDelay
			//
			// NOTE: VDC parameters have to be passed in a VARIANT using the
			// type VT_DISPATCH.  Therefore, we need to QueryInterface for
			// an IDispatch pointer.  we use the SAFEQI() macro to do this.
			// QueryInterface not only gives us an IDispatch pointer back
			// but it also increments the reference count on the interface.
			// Once we haved stuffed the IDispatch pointer into the VARIANT
			// and the VARIANT into the SAFEARRAY, we call VariantClear()
			// on the VARIANT we used to release the interface, which
			// decrements the reference count.
			//
			V_VT(&varParam) = VT_DISPATCH;
			pPulseVDC->SAFEQI(IDispatch, &V_DISPATCH(&varParam));
			pvArrayElement = static_cast<void*>(&varParam);
			rgIndices[0] = 0;
			hr = SafeArrayPutElement(pSaIn, rgIndices, pvArrayElement);
			varParam.Clear();
			testHR(hr, "Failed to fill in Display_Test input args array ndx:1");

			// Second input parameter UpperLimit
			V_VT(&varParam) = VT_DISPATCH;
			pUpLimitVDC->SAFEQI(IDispatch, &V_DISPATCH(&varParam));
			pvArrayElement = static_cast<void*>(&varParam);
			rgIndices[0] = 1;
			hr = SafeArrayPutElement(pSaIn, rgIndices, pvArrayElement);
			varParam.Clear();
			testHR(hr, "Failed to fill in Display_Test input args array ndx:2");

			// Third input parameter LowerLimit
			V_VT(&varParam) = VT_DISPATCH;
			pLoLimitVDC->SAFEQI(IDispatch, &V_DISPATCH(&varParam));
			pvArrayElement = static_cast<void*>(&varParam);
			rgIndices[0] = 2;
			hr = SafeArrayPutElement(pSaIn, rgIndices, pvArrayElement);
			varParam.Clear();
			testHR(hr, "Failed to fill in Display_Test input args array ndx:3");

			// Prepare the inputs parameter to call.  The "inputs" parameter is
			// a VARIANT that contains the SAFEARRAY of VARIANTs we built above.
			V_VT(&varInputs) = VT_ARRAY | VT_VARIANT;
			V_ARRAY(&varInputs) = pSaIn;

			// Prepare the outputs array, in this case we just pass in an empty 
			// SAFEARRAY of type VT_VARIANT.
			V_VT(&varOutputs) = VT_BYREF | VT_ARRAY | VT_VARIANT;
			V_ARRAYREF(&varOutputs) = &pSaOut;

			//
			// Call the VEE UserFunction - Display_Test
			//
			pDisplayTestUF->Call(varInputs, &varOutputs);

			// 
			// Grab the output of Display_Test
			//

			// The output comes back as a SAFEARRAY of type VT_VARIANT.  I know
			// that this function has one output of type 
			// VT_DISPATCH (VeeDataContainerPtr).
			rgIndices[0] = 0;
			pvArrayElement = static_cast<void*>(&varParam);
			hr = SafeArrayGetElement(pSaOut, rgIndices, pvArrayElement);
			testHR(hr, "Could not retrieve Display_Test result");

			pErrorPointsVDC = V_DISPATCH(&varParam);
			varParam.Clear();

			// The VDC returned is a 1D Array of 2D coords.  If there is
			// no data in it, then the test passed.  Any data in it represents
			// failed points.
			if (pErrorPointsVDC->GetNumElements() > 0)
				printf("Test failed\n");
			else
				printf("Test passed\n");
		
			// Free inputs & outputs variants and their contents
			varInputs.Clear();
			varOutputs.Clear();
			pSaIn = 0;
			pSaOut = 0;

			// Ask if the user wants to run again
	        do 
	        {
	            printf("Run again (y or n)? >");
	            gets(response);
	        } 
	        while(strncmp(response, "y", 1) && strncmp(response, "n", 1));

	        bDone = (strncmp(response, "y", 1) ? TRUE : FALSE);
		}
	}
	catch (_com_error &err)
	{
		// veeErrorOffset is provided by importing callserv.tlb.  If the
		// difference between the error number and veeErrorOffset is > 0 and
		// <= 999, then the difference corresponds to an internal VEE error
		// or an error raised by the VEE program.
		long veeErrNum = err.Error() - veeErrorOffset;

		if (veeErrNum == 871)
		{
			printf("\nUser pressed stop, no results\n");
		}
		else
		{
			printf("Error:\n");
			printf("Code = %08lx\n", err.Error());
			printf("Message = %s\n", err.ErrorMessage());
			printf("Source = %s\n", (char*) err.Source());
			printf("Description = %s\n", (char*) err.Description());
		}
	}
	catch (const HRESULT hr)
	{
		printf("Error HRESULT: 0x%08lx\n", hr);
		dumpSystemErrorInfo(hr);
	}
	catch (const char* pszMsg)
	{
		printf("Error message: %s\n", pszMsg);
	}
	catch (...)
	{
		printf("Unexpected exception\n");
	}

	// Release the pointers before we call CoUninitialize().  If we let the 
	// compiler release the interfaces when the objects are destructed 
	// you get an acess violation because the CoUninitialize() results in 
	// the server (VEE) being shutdown. Consequently, the Release calls fail.
	varIndex.Clear();
	varParam.Clear();
	pUpLimitVDC = 0;
	pLoLimitVDC = 0;
	pPulseVDC = 0;
	pErrorPointsVDC = 0;
	pSynthesizePulseUF = 0;
	pDisplayTestUF = 0;
	pUserFuncColl = 0;
	pLib = 0;
	pCallServer = 0;

	// Closes the COM Library
	CoUninitialize();

	return 0;
}


inline void dumpSystemErrorInfo(HRESULT hr)
{
	void* lpMsgBuf = 0;

	// See if there is more info available from the OS.
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
	              NULL, (DWORD)hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
	              (LPTSTR) &lpMsgBuf, 0, NULL );

	if (lpMsgBuf)
	{
		printf("System error info: %s\n", (char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
}


// Convenient inline function to test HR results and throw an exception
// on failed hresult.
inline void testHR(HRESULT hr, char* msg) 
{ 
	if (FAILED(hr))
	{
		printf("Error message: %s\n", msg);
		throw hr;
	}
}


inline wchar_t *strAnsiToWide(char *pszStr) 
{
	long   len;
	WCHAR* pwszNewStr;

	if (!pszStr)
		return (WCHAR*) 0;
	
	len = strlen(pszStr);
	pwszNewStr = (WCHAR*)malloc(sizeof(WCHAR)*(len+1));
	mbstowcs(pwszNewStr, pszStr, len+1);
	
	return pwszNewStr;
}


