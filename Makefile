# Faudrait ajouter des librairies, Qt

GXX = g++
FLAGS = # -Wall -Werror -O3 -pedantic -g
SRCFILES = *.cpp
HFILES = *.h

TARGET = prg.exe
TARGET2 = prgp.exe # Programme fusionnant le Prolog/C++

all:
	$(GXX) $(FLAGES) $(SRCFILES) -o $(TARGET)

	# --- CONNEXION PROLOG/C++ ---
	# plcon.exe -f none -F none -g true -t "consult(['%2']),qsave_program('pltmp.exe',[goal='\ $welcome',toplevel=prolog,init_file=none])"
	# copy /b %1+pltmp.exe %3
	# del pltmp.exe
	# ----------------------------
clean:
	rm -f *~

clear: clean
	rm -f $(TARGET)

exec: all
	./$(TARGET)

