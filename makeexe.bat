@ECHO OFF
echo.
echo ....... Compiling .......
echo.
if exist main.exe del main.exe
PsyMake | more
echo.
if exist main.cpe echo Compiled successful ! ! !
if exist main.cpe echo.
if exist main.cpe Cpe2X /CE main.cpe
if exist main.cpe del main.cpe >NUL
pause
