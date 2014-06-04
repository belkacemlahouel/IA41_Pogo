@echo off
plcon.exe -f none -F none -g true -t "consult(['%2']),qsave_program('pltmp.exe',[goal='\$welcome',toplevel=prolog,init_file=none])"
copy /b %1+pltmp.exe %3
del pltmp.exe