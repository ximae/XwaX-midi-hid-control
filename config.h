/***** config.h *****/

/*
   Configure your midi note assignments here.

example :

up = 34; will assign note 34 to the up cursor key

*/

#ifndef _CONFIG_H_
#define _CONFIG_H_


        /*cursors*/
	      int up = 10;    // seek up
        int down = 13;  // seek down
        int left = 11;  // folder up
        int right = 9; // folder down
        /* deck 1 */
        int f1 = 29;   // load to deck1
        int f2 = 36;   // set time 
        int f3 = 37;   // toggle vinyl control 
        /* deck 2 */
        int f5 = 15;   // load to deck1
        int f6 = 21;   // set time 
        int f7 = 22;   // toggle vinyl control 
        /* deck 3 */
        int f9 = 17;   // load to deck1
        int f10 = 18;  // set time 
        int f11 = 19;  // toggle vinyl control 
        /* additional controls */
        int control = 38;
        int shift = 23;
        int tab = 20;
    
#endif
