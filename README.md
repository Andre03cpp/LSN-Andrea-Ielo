# Esercizi LSN - Andrea Ielo
Raccolta dei laboratori del corso di Laboratorio di Simulazione Numerica.
Per scaricare tutti i laboratori contemporaneamente, cliccare sul pulsante verde **Code** in alto e selezionare **Download ZIP**.

## Ambienti di lavoro
Ho lavorato in ambiante **windows** con macchina virtuale **WLS** per eseguire codice scritto in C++.

Per eseguire i jupyter notebook ho utilizzato un environment **Anaconda**.

## Come eseguire i codici
Per eseguire il codice su C++ basta utilizzare le istruzioni del makefile (di norma ./main.exe).

Una eccezione è il laboratorio 6 in cui ho utilizzato uno script in python (quindi bisogna assicurarsi di avere un environment su cui possa girare python attivo) che automatizza l'esecuzione del file in C++.

Le linee di comando sono le seguenti:

``` python pipeline.py whole ``` e ``` python magnetization.py whole ```

### Salvataggio dei files
I files di output generati dagli eseguibili, per poter essere utilizzati nei jupiter notebook, devono avere lo stesso identico nome a quelli già presenti e utilizzati nella cartella.
