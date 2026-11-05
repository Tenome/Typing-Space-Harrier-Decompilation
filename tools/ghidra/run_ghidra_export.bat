@echo off
rem Export the original exe from Ghidra (headless) into build\ghidra\export\tsh.exe\ (decompiled.c, functions.tsv, ...).
rem Needs GHIDRA_INSTALL_DIR (a Ghidra 11/12 install) and Java 21+; orig\tsh.exe must exist. Output is git-ignored.
setlocal
set "ROOT=%~dp0..\.."
if "%GHIDRA_INSTALL_DIR%"=="" (echo Set GHIDRA_INSTALL_DIR first & exit /b 1)
if not exist "%ROOT%\build\ghidra" mkdir "%ROOT%\build\ghidra"
call "%GHIDRA_INSTALL_DIR%\support\analyzeHeadless.bat" "%ROOT%\build\ghidra" TSH -import "%ROOT%\orig\tsh.exe" -overwrite ^
  -scriptPath "%~dp0" -postScript ExportAll.java "%ROOT%\build\ghidra\export" -log "%ROOT%\build\ghidra\ghidra.log"
