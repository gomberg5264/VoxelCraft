echo Copying %1 to %2
robocopy %1 %2 * /mir > NUL
if %errorlevel% leq 1 exit 0 else exit %errorlevel%