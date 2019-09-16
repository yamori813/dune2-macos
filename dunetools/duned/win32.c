// win32.c
//
// Time-stamp: <29/09/02 18:55:02 keuchel@rainer-keuchel.de>

// Mallocs a string and sets lpszal to point to it.

#include <windows.h>

int
w32_getregstr(HKEY hRoot,LPCTSTR lpszKey, LPCTSTR lpszValueName, 
              TCHAR **plpszVal)
{
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize = 0;
  TCHAR *lpszVal = NULL;

  if(hRoot == NULL)
    hRoot = HKEY_LOCAL_MACHINE;

  if(RegOpenKeyEx(hRoot, 
                  lpszKey, 
                  0,
                  KEY_ALL_ACCESS,
                  &hKey
                  ) == ERROR_SUCCESS)
    {
      LONG lResult;

      // get size of string...
      if(RegQueryValueEx(hKey, 
                         lpszValueName, 
                         NULL, 
                         &dwType,
                         (LPBYTE) lpszVal,
                         &dwSize) == ERROR_SUCCESS)
        {
          lpszVal = malloc(dwSize + 1);

          lResult = RegQueryValueEx(hKey, 
                                    lpszValueName, 
                                    NULL, 
                                    &dwType,
                                    (LPBYTE) lpszVal,
                                    &dwSize);
        }

      RegCloseKey(hKey);

      if(lResult == ERROR_SUCCESS)
        {
          *plpszVal = lpszVal;

          return 0;
        }
    }

  *plpszVal = NULL;
  return -1;
}

char *
win32_getinstalldir()
{
  TCHAR *dir;

  if(w32_getregstr(HKEY_LOCAL_MACHINE,
		   "Software\\Allgeier\\UnibasicSQL", 
		   "InstallDir",
		   &dir) < 0)
    return NULL;
  
  return dir;
}

