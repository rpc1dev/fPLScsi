plscsi: *.c*
	gcc -o plscsi -Wall *.c*

sgio: *.c*
	gcc -o sgio -Wall -DSCSI_STUB scsistub.cpp sgio.cpp
