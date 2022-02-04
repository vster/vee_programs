#include <stdlib.h>

#ifdef WIN32
#  define DLLEXPORT __declspec(dllexport)
#else
#  define DLLEXPORT
#endif

/* The description will show up on the Program Explorer when you select
   "Show Description" from the object menu and the Function Selection
   dialog box in the small window on the bottom of the box.
*/
DLLEXPORT char myFunc_desc[] = "This function adds 1.0 to the array passed in";

DLLEXPORT long myFunc(long arraySize, double *array) {
	long i;

	for (i = 0; i < arraySize; i++, array++) {
		*array += 1.0;
	}

	return(arraySize);
}
