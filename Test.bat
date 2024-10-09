@echo off
cls
echo Compilando il progetto...

rem Compila il progetto usando il Makefile
make

rem Verifica se la compilazione ha avuto successo
if %ERRORLEVEL% equ 0 (
    echo Compilazione completata con successo.
    
    rem Esegui il programma
    echo Eseguendo il programma...
    .\progetto.exe
) else (
    echo Errore durante la compilazione.
)

pause
