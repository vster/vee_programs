// This API can be used to locate the user's profile directory

// HRESULT SHGetFolderPath(HWND hwndOwner, int nFolder, HANDLE hToken,
//                         DWORD dwFlags, LPTSTR pszPath);


long _stdcall SHGetFolderPathA(long hwndOwner, long nFolder, long hToken, 
                               long dwFlags, char* pszPath);

