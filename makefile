commit:
	#make it look nice, TODO: find a pretty maker without possible alternative indentations
	find . -iname *.h -o -iname *.c | xargs clang-format -i.
install_external:
	#install Ledger speculos the Nano emulator
	git clone https://github.com/LedgerHQ/speculos.git external/speculos
clean_external:
	rm -Rf external/speculos
cylib: 
	gcc cy_multmontgomery.c -o test
	
default: cylib