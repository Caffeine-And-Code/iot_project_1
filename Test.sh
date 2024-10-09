#!/bin/bash
clear

# Compilazione del file sorgente
echo "Compilando i file..."
make
# Controllo se la compilazione è avvenuta con successo
if [ $? -eq 0 ]; then
    echo "Compilazione completata con successo."
    
    # Esecuzione del programma
    echo "Eseguendo il programma..."
    ./progetto
else
    echo "Errore durante la compilazione."
fi
