#include <linux/input.h>
#include<linux/uinput.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <alsa/seq_event.h>
#include <alsa/seq_midi_event.h>
#include "config.h"


/* Setup the uinput device Function */
 int setup_uinput_device() {
  int uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
  if (uinp_fd == 0) {
     printf("Unable to open /dev/uinput\n");
     return -1;
    }

  struct uinput_user_dev uinp;
  memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
  strncpy(uinp.name, "Virtual Keyboard Device", strlen("Virtual Keyboard Device"));
  uinp.id.version = 4;
  uinp.id.bustype = BUS_USB;
  uinp.id.product = 1;
  uinp.id.vendor = 1;

// Setup the uinput device
  ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
  ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
  int i = 0;
  for (i = 0; i < 256; i++) {
      ioctl(uinp_fd, UI_SET_KEYBIT, i);
     }

// Create input device into input sub-system 
  write(uinp_fd, &uinp, sizeof(uinp));
  if (ioctl(uinp_fd, UI_DEV_CREATE)) {
     printf("Unable to create UINPUT device.");
     return -1;
    }
  return uinp_fd; 
}

/* actual key sending function */
 void send_key_event(int fd, unsigned int keycode, int keyvalue) {
  struct input_event event;
  gettimeofday(&event.time, NULL);

  event.type = EV_KEY;
  event.code = keycode;
  event.value = keyvalue;

  if (write(fd, &event, sizeof(event)) < 0) {
     printf("simulate key error\n");
    } 

  event.type = EV_SYN;
  event.code = SYN_REPORT;
  event.value = 0;
  write(fd, &event, sizeof(event));

  if (write(fd, &event, sizeof(event)) < 0) {
     printf("simulate key error\n");
    } 
}

/* Main program */
int main(int argc, char *argv[]) {
   
    FILE* midi;
    unsigned int notenumber[128];
    unsigned int key_code[128];
    int temp, i;
    snd_midi_event_t* parser;
    snd_seq_event_t event;

/*usage*/
 if (argc < 2) {
    fprintf(stderr,"Usage: %s device \n",*argv);
    return 1;
   }


/* initialize array*/
 for ( i = 0; i < 128; i++) {
     key_code[i] = 0;
     notenumber[i] = 0;
    }

/*midinote to keycode array assign*/
    key_code[up] = 103;
    notenumber[up] = up;
    
    key_code[down] = 108;
    notenumber[down] = down;
  
    key_code[left] = 105;
    notenumber[left] = left;
 
    key_code[right] = 106;
    notenumber[right] = right;

    key_code[f1] = 59;
    notenumber[f1] = f1;
  
    key_code[f2] = 60;
    notenumber[f2] = f2;

    key_code[f3] = 61;
    notenumber[f3] = f3;

    key_code[f5] = 63;
    notenumber[f5] = f5;

    key_code[f6] = 64;
    notenumber[f6] = f6;

    key_code[f7] = 65;
    notenumber[f7] = f7;

    key_code[f9] = 67;
    notenumber[f9] = f9;

    key_code[f10] = 68;
    notenumber[f10] = f10;
 
    key_code[f11] = 69;
    notenumber[f11] = f11;

    key_code[control] = 29;
    notenumber[control] = control;

    key_code[shift] = 42;
    notenumber[shift] = shift;

    key_code[tab] = 15;
    notenumber[tab] = tab; 
 
/* initialize virtual keyboard*/
 int uinp_fd;
 if ((uinp_fd = setup_uinput_device()) < 0) {
     printf("Unable to find uinput device\n");
     return -1;
    }
sleep(1);

/*midi*/
if (!(midi = fopen(argv[1],"rb"))) {
   fprintf(stderr,"Unable to open MIDI device '%s'\n",argv[1]);
   return 3;
  }
    
 snd_midi_event_new(32,&parser);
/*feed implied note on message*/
 snd_midi_event_encode_byte(parser,0x9A,&event);

/*main loop*/
 fputs("Ready for midi input\n",stderr);
  while ((temp = fgetc(midi)) != EOF) { 
    if (snd_midi_event_encode_byte(parser, temp, &event) == 1) {
        if (event.type == SND_SEQ_EVENT_NOTEON) {
           if ((event.data.note.note <= 127) && (event.data.note.velocity)) {
              temp = event.data.note.note;
              if (temp == notenumber[event.data.note.note]) { 
                   
                  send_key_event(uinp_fd, key_code[temp], 1);
                  send_key_event(uinp_fd, key_code[temp], 0);
                                        
                 }
     printf("%i->%i\n",event.data.note.note,(int)key_code[event.data.note.note]);
             } 
          }
       }
   }



/* Destroy the input device */
 ioctl(uinp_fd, UI_DEV_DESTROY);
/* Close the UINPUT device */
 close(uinp_fd);
/*close midi */
 snd_midi_event_free(parser);
 fclose(midi);
/*exit*/
 return 0;
} 
