/*
**                  Example program for use with Callable VEE
**
**
**  The VEE example this callable VEE example is based on is telecomm.vee
**  in the 'examples/Applications' directory.
**
**  This example program uses a library of VEE user functions in 
**  examples\CallableVEE\lib.vee.
**  The library has two functions, 'Synthesize_Pulse' and 'Display_Test'.
**
**  Copyright (c) 1996-2018 Keysight Technologies
**  All Rights Reserved.
**
**  ** This program is free software. It can be redistributed
**     and/or modified.
**  ** It is distributed in the hope that it will be useful,
**     but WITHOUT ANY WARRANTY.
**  ** Without even the implied warranty of MERCHANTABILITY
**     or FITNESS FOR A PARTICULAR PURPOSE.
**
**  Compile on HP-UX:
**      Copy the 'callVEE.c' source to a convenient directory. 
**      Execute this line (without the '*')      
**
**      For HP-UX 9.x

cc -Aa -z -o callVEE -g callVEE.c -I/usr/lib/veetest/include \
    /usr/lib/veetest/lib/libvapi.a -lm

**      For HP-UX 10.01

cc -Aa -z -o callVEE -g callVEE.c -I/opt/veetest/include \
    /opt/veetest/lib/libvapi.a -lm
**
**
**  For a Compile on Win95 or NT:
**      Compile  with-in the \examples\CallableVEE\RPC_API\ directory (notice) relative paths.

cl -W3 -Od -Zi -DWIN32 -o callvee callvee.c -I..\..\..\include oldnames.lib \
    libc.lib user32.lib ..\..\..\lib\libvapi.lib wsock32.lib kernel32.lib \
    advapi32.lib

NOTE:  You need to distribute LIBVAPI.DLL with your executable. We recommend
       that you put LIBVAPI.DLL in the same directory as your executable.
       However, you can place LIBVAPI.DLL in your Windows system directory
       or any other directory that is in the PATH environment variable.
**
*/
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include "veeData.h"
#include "veeRPC.h"


/* Useful defines */
#define     YEP     1
#define     NOPE    0


/* Global data declarations, vee2DCoord is defined in veeData.h */
vee2DCoord  upperLimit[11];
vee2DCoord  lowerLimit[13];

main(long argc, char *argv[]) {

    VDC args[3];
    VDC *results1;
    VDC *results2;

    VRPC_SERVICE            aService;
    VRPC_LIBRARY            aLibrary;
    VRPC_FUNCTION           synthesizePulseFunc, displayTestFunc;
    struct VRPC_FUNC_INFO   *funcInfo;
	VRPC_FUNCTION_PAIR		*funcInfoStruct;
	VRPC_VERSION			*versionInfo;

    char                    **aListOfFunctionNames;
    long                    numberOfFunctions;
    long                    ndx;
    long                    returnCode;
    long                    loop;
    char                    *hostName;
    char                    *theDisplay;
    char                    *theGeometry;
    char                    response[128];
    double                  timeout;
    unsigned long           flags;
    unsigned long           debugMode;
    double                  referenceDelay;
    vee2DCoord              *anArrayOfCoords;
    long                    numberElements;

    /*
    ** Parse options
    **  
    **      callVEE hostname [display server] [geometry] [timeout]
    */
    if (argc < 2) {     /* must have at least a hostname */
        printf("Usage: callVEE hostname [display server] [geometry] "
               "[timeout]\n");
        exit(0);
    }
    hostName = argv[1];
    theDisplay =  ( argc > 2 ? argv[2] : (char*)0 );
    theGeometry = ( argc > 3 ? argv[3] : (char*)0 );
    if (argc > 4) {
        sscanf(argv[4],"%lg", &timeout);
    }
    else {
        timeout = (double)120.0;
    }

	/* veeSetNoisyError(); */
	
    flags = VRPC_CREATE_NEW;

    /* 
    ** VRPC_SERVICE vrpcCreateService(char *hostname, 
    **                                char *aDisplay, 
    **                                char *aGepmetry,
    **                                double aTimeoutInSeconds, 
    **                                unsigned long flags);
    **
    **      hostname: Invoke a Service on machine at hostName. Must be a 
    **                resolvable name or IP address.
    **
    **      aDisplay: A display server name, e.g. 'tigger:0.0'. If the argument
    **                is null, then the display is assumed to be the machine
    **                designated by 'hostname'. If the server is instantiated
    **                on a Win95 or WinNT machine and the hostname != aDisplay,
    **                an error will occur. Note that the display server can be
    **                a Win95 or WinNT machine if a X server is present.
    **
    **      aGeometry: Standard geometry string, e.g. '800x500+0-0'.
    **
    **      aTimeoutInSeconds: Length of time to wait for any response from the
    **                server including instantiation time.
    **
    **      flags:    If the flag == VRPC_CREATE_NEW, then a new server
    **                is instantiated on the machine, 'hostname'. If this is
    **                not set, then any existing server on 'hostname' with
    **                display server 'aDisplay' is returned
    **                
    ** The return value is a pointer to the service structure. A null return is
    ** indicative of an error.
    */
    aService = vrpcCreateService(hostName, theDisplay, theGeometry, timeout, 
                                 flags);
    if (!aService) {
        /*
        ** char *veeGetErrorString(void);
        **
        ** Returns a pointer to static memory (not yours!) that contains a null
        ** terminated string that is a (hopefully) meaningful error.
		**
        ** int16 veeGetErrorNumber(void);
        **
        ** Returns the latest error number to occur. These numbers are the same
        ** as those returned by VEE. Please see the HELP menu in VEE.
        ** 
        */
        printf("Cannot create service: %s\n", veeGetErrorString());
        printf("Error Number: %ld\n", (long)veeGetErrorNumber());
        exit(1);
    }

    /*
    ** long vrpcSetBehavior(VRPC_SERVICE aService, unsigned long flags);
    **
    **      aService: A valid service.
    **
    **      flags: This can take on the logical OR of three values, 
    **             VRPC_WAIT_SLEEPING, VRPC_WAIT_SPINNING, and 
    **             VRPC_BUFFER_EXPAND. VRPC_WAIT_SLEEPING and
    **             VRPC_WAIT_SPINNING are mutually exclusive. The *_WAIT_*
    **             flags denote the behavior of the client when waiting for a
    **             retrun from the server. Sleeping indicates that the client
    **             process may be "rolled-out" by the OS. Spinning indicates
    **             that the client will "busy-wait" for a server reply. 
    **             VRPC_BUFFER_EXPAND will cause the OS receive buffers on the
    **             for LAN communications to increase in size, if messages
    **             become larger than the default (usually 8KB).
    **
    ** A zero return indicates success. A non-zero indicates an error has 
    ** occured.
	**
    */
    returnCode = vrpcSetBehavior(aService, VRPC_WAIT_SLEEPING);
    if (returnCode != 0) {
        printf("Cannot set behavior: %s\n", veeGetErrorString());
        exit(1);
    }


    /*
    ** VRPC_LIBRARY vrpcLoadLibrary(VRPC_SERVICE aService, char *aPath);
    **
    ** This procdure will load a library of VEE user functions into a service.
    **
    **      aService: A pointer to a valid service. 
    **
    **      aPath:  : A string that is an absolute path to library of VEE
    **                user functions. 
    **
    ** Upon successful completion a valid pointer to a library is returned. If
    ** null is returned an error occured.
    */
    aLibrary = 
		vrpcLoadLibrary(aService,"~installDir/examples/CallableVEE/lib.vee");
    if (!aLibrary) {
        printf("Cannot load library:\n  %s'\n", veeGetErrorString());
        exit(1);
    }
	else {
        printf("Loaded library '%s'\n", 
               "~installDir/examples/CallableVEE/lib.vee");
	}
    
	/*
	** VRPC_VERSION *vrpcGetVersion();
	**
	**	This function returns a pointer to a version structure.  The structure
	**	is defined in veeRPC.h as:
	**
	**	typedef struct _VRPC_VERSION{
	**		unsigned short major;
	**		unsigned short minor;
	**	} VRPC_VERSION;
	** 
	**	Some of the functions used in this example are only availale in
	**	library versions 1.1 and above.
	**
	**	You should not attempt to free the memory returned as a result
	**	of calling this function.  The returned value is a pointer to
	**	statically-allocated data.
	*/
	versionInfo=vrpcGetVersion();
	if ((versionInfo->major >= 1) && (versionInfo->minor >= 1)) {
		/*
	    ** VRPC_FUNCTION_PAIR *vrpcGetFunctionNamesEx(VRPC_LIBRARY alibrary);
		**
		** 	aLibrary: a valid library pointer
		**
		**	The value returned is a pointer to a structure allocated in the
		**	callable vee library.  You should free this structure and its 
		**	contents by calling vrpcFreeFunctionNamesEx() when you will no
		**	longer need to reference the information it contains.
		**
		**	The structure is defined in veeRPC.h as:
		**
		**	typedef struct _VRPC_FUNCTION_PAIR{
		**		long numberOfFunctions;
		**		char **functionNames;
		**	} VRPC_FUNCTION_PAIR;
		**
		** 	This function returns null on failure or the adress of an allocated
		** 	structure when successful.
		**
		**	This function is available in library versions 1.1 and above.
		*/
		funcInfoStruct=vrpcGetFunctionNamesEx(aLibrary);
		if (!funcInfoStruct) {
			printf("Can't get function names: %s\n", veeGetErrorString());
			exit(1);
		}

		/* Let's print them off and free the memory associated with them */
		for (ndx = 0; ndx < funcInfoStruct->numberOfFunctions; ndx ++) {
			printf("Function name %ld: %s\n", ndx,
				   funcInfoStruct->functionNames[ndx]);
		}

		/*
		** VRPC_FUNCTION_PAIR *vrpcFreeFunctionNamesEx(
		**		VRPC_FUNCTION_PAIR *funcInfoStruct);
		**
		** 	funcInfoStruct: a pointer to a structure returned from a 
		**	call to vrpcGetFunctionNamesEx().
		**
		**	Use this function to free the memory returned as a result of 
		**	calling vrpcGetFunctionNamesEx().  The return value should
		**	always be null.
		**
		**	This function is available in library versions 1.1 and above.
		*/
		funcInfoStruct=vrpcFreeFunctionNamesEx(funcInfoStruct);
	}
	else {
		/*
		** char **vrpcGetFunctionNames(VRPC_LIBRARY aLibrary,
		**							   long *numberOfFunctions);
		**
		**	aLibrary: a valid library pointer
		**
		**	numberOfFunctions: a pointer to a long that will be filled in
		** 		with the number of functions in the library 'aLibrary'.
		**
		**	The returned value is an array of strings containing 
		**		'numberOfFunctions' function names.  The memory returned
		**		as a result of making this call is dynamically allocated.
		**		Free this memory with a call to vrpcFreeFunctionNames().
		**		This function returns NULL on failure or a valid pointer
		**		upon success.
		**
		**	Note that this function has been superseded in version 1.1
		**		of this library by the function vrpcGetFunctionNamesEx().
		**		We recommend the use of the new function when possible.
		*/
		aListOfFunctionNames=vrpcGetFunctionNames(aLibrary, &numberOfFunctions);
		if (!aListOfFunctionNames) {
			printf("Cannot get function names: %s\n", veeGetErrorString());
			exit(1);
		}
		
		for (ndx=0; ndx<numberOfFunctions; ndx++) {
			printf("Function name %ld: %s\n", ndx, aListOfFunctionNames[ndx]);
		}

		/*
		** char **vrpcFreeFunctionNames(VRPC_LIBRARY aLibrary,
		**								char **functionNames);
		**
		**	aLibrary: a valid library pointer
		**
		**	functionNames: an array of function names returned from a 
		**		call to vrpcGetFunctionNames().
		**
		**	Use this function to free the dynamically-allocated memory
		**		returned as a result of calling vrpcGetFunctionNames().  It 
		**		is essential that you use the same library pointer value 
		** 		and functionName pointer value in the call to this function
		**		as you did in the associated call to vrpcGetFunctionNames().
		**
		**	Note that this function has been superseded in version 1.1 of
		**		the library by the function vrpcFreeFunctionNamesEx().
		**		We recommend the use of the new function when possible.
		*/
		aListOfFunctionNames=vrpcFreeFunctionNames(aLibrary,
												   aListOfFunctionNames);
	}

    /*
    ** VRPC_FUNCTION vrpcFindFunction(VRPC_LIBRARY aLibrary, 
    **                                char *aFunctionName);
    **
    **      aLibrary: A valid library pointer.
    **
    **      aFunctionName: A string that is a valid function name.
    **
    ** Returns a valid function pointer. If an error occurs, a null 
    ** pointer is returned. The memory returned is owned by the API. It 
    ** is freed when the library is freed.
    **
    */
    synthesizePulseFunc = vrpcFindFunction(aLibrary, "Synthesize_Pulse");
    if (!synthesizePulseFunc) {
        printf("Unable to find function 'Synthesize_Pulse'.\n");
        exit(1);
    }
    displayTestFunc = vrpcFindFunction(aLibrary, "Display_Test");
    if (!displayTestFunc) {
        printf("Unable to find function 'Display_Test'.\n");
        exit(1);
    }

    /*
    ** struct VRPC_FUNC_INFO *vrpcFunctionInfo(VRPC_FUNCTION aFunction);
    **
    **      aFunction: A valid function pointer.
    **
    ** Find pertinent information on functions. Memory returned is yours!
    ** The structure, VRPC_FUNC_INFO, is defined in veeRPC.h.
    **
    */
    funcInfo = vrpcFunctionInfo(synthesizePulseFunc);
    if (!funcInfo) {
        printf("Unable to get function info: %s\n", veeGetErrorString());
        exit(1);
    }
    printf("Name: %s, Number args: %ld, Number results: %ld\n\n", 
           funcInfo->functionName,
           funcInfo->numArguments,
           funcInfo->numResults);

    /*
    ** struct VRPC_FUNC_INFO* vrpcFreeFunctionInfo(struct VRPC_FUNC_INFO*
    **                                                          funcInfo);
    **      funcInfo: A valid pointer to the VRPC_FUNC_INFO structure.
    **
    ** A utility function that frees the VRPC_FUNC_INFO structure.
    */
    funcInfo = vrpcFreeFunctionInfo(funcInfo);

    funcInfo = vrpcFunctionInfo(displayTestFunc);
    if (!funcInfo) {
        printf("Unable to get function info: %s\n", veeGetErrorString());
        exit(1);
    }
    printf("Name: %s, Number args: %ld, Number results: %ld\n\n",
           funcInfo->functionName,
           funcInfo->numArguments,
           funcInfo->numResults);

    funcInfo = vrpcFreeFunctionInfo(funcInfo);

    /*
    ** Okay, Lets call some VEE functions.
    */
    printf("This program simulates a Telecomm Pulse Test.\n\n");

    loop = YEP;
    while(loop) {

        /*
        ** vrpcSetExecutionMode(VRPC_LIBRARY aLibrary, 
        **                                       unsigned long executionMode)
        **      aLibrary: a valid library pointer.
        **
        **      executionMode: a flag that can take on the unique values
        **                     VRPC_DEBUG_EXECUTION or VRPC_NORMAL_EXECUTION.
        **
        **  VRPC_NORMAL_EXECUTION allows the VEE user function to execute
        **  in a normal fashion. VRPC_DEBUG_EXECUTION will run the function in
        **  debug mode. In this mode the user function can be stepped through.
        **  A non-zero return means a error has occured.
        */

        do {
            printf("Run in debug mode (y or n)? >");
            gets(response);
        } while(strncmp(response, "y", 1) && strncmp(response, "n", 1));

        debugMode = (strncmp(response, "y", 1) ? 
                            VRPC_NORMAL_EXECUTION : VRPC_DEBUG_EXECUTION);

        vrpcSetExecutionMode(aLibrary, debugMode);
            

        printf("Enter Ref Delay value in microseconds\n");

        do {
            printf("Valid numbers are between .25u and .45u\n");
            printf("> ");
            scanf("%lg", &referenceDelay);
            gets(response);  /* get the linefeed */
        } while ( referenceDelay < .25 || referenceDelay > .45);

        /* convert to seconds */
        referenceDelay = referenceDelay * 1.0E-06;

        /*
        ** VDC *vrpcCallAndReceive(VRPC_FUNCTION aFunction, 
        **                         VDC* anArrayOfArguments);
        **
        **      aFunction: a valid function pointer
        **
        **      anArrayOfArguments: an array of VDCs
        **
        **  This function blocks until either the function returns or a time
        **  out occurs. If the function executes properly and returns data,
        **  then a pointer to an array of VDCs is returned. If an error or a
        **  timeout occurs a null pointer is returned. If the function executes
        **  properly and has no results, a null pointer is returned. In this
        **  case check for a possible error using getVeeErrorNumber().
        **
        **
        ** long vrpcCall(VRPC_FUNCTION aFunction, VDC *anArrayOfArguments);
        **
        **      aFunction: a Valid function pointer
        **
        **      anArrayOfArguments: an array of VDCs
        **
        **  This function sends off the arguments pointed to by
        **  'anArrayOfArguments'. A non-zero return value indicates an error.
        **
        **
        ** VDC *vrpcReceive(VRPC_FUNCTION aFunction, unsigned long waitMode);
        **
        **      aFunction: a valid function pointer previously used in a
        **                 'vrpcCall' function call.
        **
        **      waitMode: can take on three possible unique values:
        **                VRPC_NO_WAITING, VRPC_WAIT_SLEEPING, or
        **                VRPC_WAIT_SPINNING.
        **
        **  Try and receive the results, if the unsigned long is equal to 
        **  VRPC_NO_WAITING (defined in veeRPC.h) and the function  has not 
        **  yet finished executing, then a NULL pointer is returned and an
        **  error number 'eWouldBlock' is set. Use getVeeErrorNumber() to check
        **  for this error. Any other non-zero error number indicates an error
        **  has occured. In the case of a function with no returns, this
        **  function need not be called. The flags VRPC_WAIT_SLEEPING or
        **  VRPC_WAIT_SPINNING indicate that the function should block waiting
        **  for a return. These values set the behavior described in
        **  vrpcSetBehavior(). If VRPC_NO_WAITING is used, the timeout value is
        **  ignored.
		**
		** void  veeClearErrorNumber(void);
		**
		** Clears previous error number.  This ensures that when you call
		** veeGetErrorNumber(), you get an error code generated since
		** the last call to veeClearErrorNumber(). 
		**
        */

        /*
        ** Call the first function 'Synthesize_Pulse'. This function has no
        ** user interface, so we can just use the 'vrpcCallAndReceive()'
        ** function. However, if debug mode is set we can either set a very
        ** long timeout or just wait until the user returns by using the two
        ** functions vrpcCall() and vrpcReceive() with non-blocking set.
        ** WE DO NOT WANT THE CLIENT TO TIME OUT - THAT IS A FATAL ERROR.
        */

        /*
        ** Package up the referenceDelay into a container
        */
        args[0] = vdcCreateDoubleScalar(referenceDelay);
        results1 = (VDC*)0;

        if (debugMode == VRPC_NORMAL_EXECUTION) {
			veeClearErrorNumber();
            results1 = vrpcCallAndReceive(synthesizePulseFunc, args);
            if (!results1) {
                returnCode = veeGetErrorNumber();
                printf("Call to Synthesize Pulse failed: %s\n", 
                       veeGetErrorString());
                /* we could process the error here if we want to */
                exit(1);
            }
        }
        else {

            /* debug is on */
            returnCode = vrpcCall(synthesizePulseFunc, args);
            if (returnCode != 0) {
                printf("Call to send arguments failed: %s\n", 
                       veeGetErrorString());
            }

            while (1) {
				veeClearErrorNumber();
                results1 = vrpcReceive(synthesizePulseFunc, VRPC_NO_WAITING);
                if (!results1) {
                    returnCode = veeGetErrorNumber();

                    switch (returnCode) {
                        case eConnectBroken:
                        case eHostDown:
                            /* Fatal errors */
                            printf("The VEE server has crashed: %s\n",
                                            veeGetErrorString() );
                            exit(1);

                        case eVeeServiceError:
                        case eSomeInternalError:
                            /* An error occured within the user function */
                            printf("%s\n", veeGetErrorString() );

                            /* just exit so it can be fixed */
                            exit(1);

                        case eDebugTermination:
                            /* User pressed stop */
                            printf("User pressed stop, no results\n");
                            exit(1);

                        case eWouldBlock:
                            /* Not done yet */
                            printf("Waiting\n");
#if !defined(WIN32) && !defined(_WIN32_) && !defined(__WIN32__)
                            sleep(1);
#endif
                            break;

                        default:
                            printf("Error: %s\n", veeGetErrorString() );
                            exit(1);
                    }
                }
                else {
                    break;
                }
            } 
        }

        /* clean up */
        args[0] = vdcFree(args[0]);

        /* 
        **
        ** Now call the function Display_Test
        ** This function has a User Interface that demands either a long
        ** timeout or the method used above for the debug call. However the
        ** User Interface has a built in timeout of 20 seconds and our default
        ** timeout is 120 seconds, so that should be enough
        */

        /* Create arguments for the displayTestFunc call */
        /* argument order is top to bottom on the user function */ 

        /* the first argument is the return from the last function call */
        args[0] = *results1; 

        /* Upper limit mask */
        upperLimit[0].xval = 0.0;
        upperLimit[0].yval = 0.05;
        upperLimit[1].xval = 2.5E-07;
        upperLimit[1].yval = 0.05;
        upperLimit[2].xval = 3.25E-07;
        upperLimit[2].yval = 0.8;
        upperLimit[3].xval = 3.26E-07;
        upperLimit[3].yval = 1.15;
        upperLimit[4].xval = 4.25E-07;
        upperLimit[4].yval = 1.15;
        upperLimit[5].xval = 5.000000000000001E-07;
        upperLimit[5].yval = 1.05;
        upperLimit[6].xval = 6.75E-07;
        upperLimit[6].yval = 1.05;
        upperLimit[7].xval = 7.250000000000002E-07;
        upperLimit[7].yval = -0.07000000000000001;
        upperLimit[8].xval = 1.1E-06;
        upperLimit[8].yval = 0.05;
        upperLimit[9].xval = 1.25E-06;
        upperLimit[9].yval = 0.05;
        upperLimit[10].xval = 1.75E-06;
        upperLimit[10].yval = 0.05;

        args[1] = vdcCreate2DCoord1DArray(11L, upperLimit);
        if (!args[1]) {
            printf("Unable to create a double array container: %s\n",
                   veeGetErrorString() );
            exit(1);
        }

        /* Lower limit mask */
        lowerLimit[0].xval = 0.0;
        lowerLimit[0].yval = -0.05;
        lowerLimit[1].xval = 3.499999999999999E-07;
        lowerLimit[1].yval = -0.05;
        lowerLimit[2].xval = 3.51E-07;
        lowerLimit[2].yval = 0.5;
        lowerLimit[3].xval = 4E-07;
        lowerLimit[3].yval = 0.95;
        lowerLimit[4].xval = 5.000000000000001E-07;
        lowerLimit[4].yval = 0.95;
        lowerLimit[5].xval = 6.000000000000001E-07;
        lowerLimit[5].yval = 0.9;
        lowerLimit[6].xval = 6.5E-07;
        lowerLimit[6].yval = 0.5;
        lowerLimit[7].xval = 6.51E-07;
        lowerLimit[7].yval = -0.45;
        lowerLimit[8].xval = 8.000000000000001E-07;
        lowerLimit[8].yval = -0.45;
        lowerLimit[9].xval = 9.25E-07;
        lowerLimit[9].yval = -0.2;
        lowerLimit[10].xval = 1.1E-06;
        lowerLimit[10].yval = -0.05;
        lowerLimit[11].xval = 1.25E-06;
        lowerLimit[11].yval = -0.05;
        lowerLimit[12].xval = 1.75E-06;
        lowerLimit[12].yval = -0.05;

        args[2] = vdcCreate2DCoord1DArray(13L, lowerLimit);
        if (!args[2]) {
            printf("Unable to create a double array container: %s\n",
                   veeGetErrorString() );
            exit(1);
        }

        if (debugMode == VRPC_NORMAL_EXECUTION) {
			veeClearErrorNumber();
            results2 = vrpcCallAndReceive(displayTestFunc, args);
            if (!results2) {
                returnCode = veeGetErrorNumber();
                printf("Call to Display Test failed: %s\n", 
                       veeGetErrorString());
                /* we could process the error here if we want to */
                exit(1);
            }
        }
        else {

            /* debug is on */
            returnCode = vrpcCall(displayTestFunc, args);
            if (returnCode != 0) {
                printf("Call to send arguments failed: %s\n", 
                       veeGetErrorString());
            }

            while (1) {
				veeClearErrorNumber();
                results2 = vrpcReceive(displayTestFunc, VRPC_NO_WAITING);
                if (!results2) {
                    returnCode = veeGetErrorNumber();

                    switch (returnCode) {
                        case eConnectBroken:
                        case eHostDown:
                            /* Fatal errors */
                            printf("The VEE server has crashed: %s\n",
                                            veeGetErrorString() );
                            exit(1);

                        case eVeeServiceError:
                        case eSomeInternalError:
                            /* An error occured within the user function */
                            printf("%s\n", veeGetErrorString() );

                            /* just exit so it can be fixed */
                            exit(1);

                        case eDebugTermination:
                            /* User pressed stop */
                            printf("User pressed stop, no results\n");
                            exit(1);

                        case eWouldBlock:
                            /* Not done yet */
#if !defined(WIN32) && !defined(_WIN32_) && !defined(__WIN32__)
                            sleep(1);
#endif
                            break;

                        default:
                            printf("Error: %s\n", veeGetErrorString() );
                            exit(1);
                    }
                }
                else {
                    break;
                }
            } 
        }

        /* check to see if the test passed */
        anArrayOfCoords =  vdcGet2DCoord1DArray(*results2, &numberElements);
        if (!anArrayOfCoords && numberElements == 0) {
            /* No failures */
            printf("Test Passed\n");
        }
        else if (anArrayOfCoords || numberElements > 0) {
            printf("Test Failed\n");
        }
        else {
            printf("Could not get valid return: %s\n", veeGetErrorString() );
        }

        /* clean up */
        vdcFree(args[0]);
        vdcFree(args[1]);
        vdcFree(args[2]);

        /* we assigned the container to args[0], so no vdcFree(*results1) */ 
        veeFree(results1);      /* free four byte pointer */

        vdcFree(*results2);
        veeFree(results2);      /* free four byte pointer */

        do {
            printf("Run again (y or n)? >");
            gets(response);
        } while(strncmp(response, "y", 1) && strncmp(response, "n", 1));

        loop = (strncmp(response, "y", 1) ? NOPE : YEP);
    }
    

    /*
    ** LIBRARY vrpcUnLoadLibrary(VRPC_LIBRARY aLibrary);
    **
    **      aLibrary: a valid library pointer;
    **
    **  Unloads a library from a VEE server. All functions associated with the
    **  library are also freed. Returns a null pointer if succesful;
    */
    aLibrary = vrpcUnLoadLibrary(aLibrary);

    synthesizePulseFunc = (VRPC_FUNCTION)0;
    displayTestFunc = (VRPC_FUNCTION)0;

    /*
    ** SERVICE vrpcDeleteService(SERVICE aService);
    **
    **      aService: a valid service pointer.
    **
    **  Delete the service. The instance is removed from the host.
    */
    aService = vrpcDeleteService(aService);

    return(0);
}
