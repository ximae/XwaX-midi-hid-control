# XwaX-midi-hid-control
 
  This is a Little side program that captures and converts midi notes into xwax hid control keystrokes via a virtual keyboard .
  
  It requires the libasound2-dev package for compilation.
  
     gcc -o xwaxm2h xwaxm2h.c -lasound -Wall
     
 
 You configure which notes are assigned to which actions in the config.h file prior to compilation. Just add the midi note number in the corresponding variable,
 
 To know which midi notes correspond to which button in your controller either look n the controller manual or just compile this program and it will tell you on screen which note you just pressed. so you can then just edit the file and recompile with desired configuration.     

usage:

 sudo ./xwaxm2h "path to your midi controller" ie: (/dev/midi1)
 
  ls /dev/ | grep midi will show you which are available. 
  
  hope its usefull!

