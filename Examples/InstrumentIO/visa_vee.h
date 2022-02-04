int __stdcall viOpenDefaultRM(int* vi);
int __stdcall viFindRsrc(int sesn, char* expr, int* vi, int* retCnt, char desc[]);
int __stdcall viFindNext(int vi, char desc[]);
int __stdcall viStatusDesc(int vi, int status, char desc[]);
int __stdcall viClose(int vi);
