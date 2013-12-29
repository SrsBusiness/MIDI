#include "MIDI.h"
#include <stdio.h> 
#include "tm4c123gh6pm.h"

#define TRACK_LIMIT 20

callback *functions;
unsigned int next_int(unsigned char *current){
    unsigned int result = 0;
    for(int i = 0; i < 4; i++)
        result = result << 8 | *(current + i);
    return result;
}

unsigned short next_short(unsigned char *current){
    unsigned short result = 0;
    for(int i = 0; i < 2; i++)
        result = result << 8 | *(current + i);
    return result;
}
unsigned int next_token(unsigned char **current){
    unsigned int result = 0;
    int i = 0;
    do{
        result = (result << 8) | **current;
        printf("next_token: %u\n", **current);
    }
    while((*(*current)++) & 0x80 && i++ < 3);
    return result;
}

int init_tracks(unsigned char *file, unsigned char **trackp, int tracks){
    unsigned char *save = file;
    while(file && next_int(file) != MIDI_TRACK)// should loop 2 times
        file++;
    if(!file){
        return 1;
    }
    trackp[0] = file + 8;
    printf("offset track1 %d\n", (int)(trackp[0] - save));
    for(int i = 1; i < tracks; i++){
        file += 4;
        file += 4 + next_int(file);
        if(next_int(file) != MIDI_TRACK)
            return 1;
        trackp[i] = file + 8; 
    }
    return 0;
}

void MIDI_init(callback *callbacks){
    functions = callbacks;
}

unsigned char and(unsigned char *array, short length){
    unsigned char result = 1;
    for(int i = 0; i < length; i++)
        result  = result && array[i];
    return result;
}



void unget_token(unsigned char **current){
    (*current)--; 
    while((*(*current - 1)) & 0x80)
        (*current)--;
}



int MIDI_play(unsigned char *file){
    unsigned char *save = file;
    if(next_int(file) != MIDI_HEADER){
        printf("Improper Header %d %d\n", *(unsigned int *)file, MIDI_HEADER);
        return 1;
    }
    file += 4;
    if(next_int(file) != 6){
        printf("Improper Header chunk size %d\n", next_int(file));
        return 1;
    }
    file += 4;
    unsigned short type = next_short(file); // type
    file += 2;
    unsigned short tracks = next_short(file); // # of tracks
    file += 2;
    unsigned char *trackp[TRACK_LIMIT];
    if(init_tracks(file, trackp, tracks)){
        printf("Init tracks failed\n");
        return 1;
    }
    unsigned char eof[TRACK_LIMIT];
    for(int i = 0; i < tracks; i++)
        eof[i] = 0;
    unsigned short tpqn = next_short(file); // time divistion
    file += 2;
    unsigned int last_ticks[TRACK_LIMIT];
    for(int i = 0; i < tracks; i++)
        last_ticks[i] = 0;
    unsigned int ticks = 0;
    while(!and(eof, tracks)){
        (*(functions -> tick))();
        for(unsigned int i = 0; i < tracks; i++){
            // todo: first handle delta time token
            int time;
            printf("time offset: %d\n", (int)(trackp[i] - save));
            while((time = next_token(trackp + i)) + last_ticks[i] == ticks)
                last_ticks[i] += time;
                printf("time: %u\n", time);
                unsigned int command = next_token(trackp + i);
                printf("command: %u\n", command);
                switch(command){
                    case MIDI_NOTE_OFF:
                        if(functions -> note_off)
                            (*(functions -> note_off))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i),
                                    (char)next_token(trackp + i));
                        else
                            for(int i = 0; i < 3; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_NOTE_ON:
                        if(functions -> note_on)
                            (*(functions -> note_on))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i),
                                    (char)next_token(trackp + i));
                        else
                            for(int i = 0; i < 3; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_NOTE_AFTERTOUCH:
                        if(functions -> note_aftertouch)
                            (*(functions -> note_aftertouch))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i),
                                    (char)(next_token(trackp + i)));
                        else
                            for(int i = 0; i < 3; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_CONTROLLER:
                        if(functions -> controller)
                            (*(functions -> controller))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i),
                                    (char)(next_token(trackp + i))); 
                        else
                            for(int i = 0; i < 3; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_PROGRAM_CHANGE:
                        if(functions -> program_change)
                            (*(functions -> program_change))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i));
                        else
                            for(int i = 0; i < 2; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_CHANNEL_AFTERTOUCH:
                        if(functions -> channel_aftertouch)
                            (*(functions -> channel_aftertouch))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i));
                        else
                            for(int i = 0; i < 2; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_PITCH_BEND:
                        if(functions -> pitch_bend) 
                            (*(functions -> pitch_bend))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i),
                                    (char)next_token(trackp + i)); 
                        else{
                            for(int i = 0; i < 3; i++)
                                next_token(trackp + i);
                        }
                        break;
                    case MIDI_SYSEX:
                        if(functions -> sysex)
                            (*(functions -> sysex))(trackp + i);
                        else
                            while(*(trackp[i]++) != 0xF7);
                        break;
                    case MIDI_SYSEX_AUTH:
                        if(functions -> sysex_auth)
                            (*(functions -> sysex_auth))((char)next_token(trackp + i),
                                    (char)next_token(trackp + i));
                        else
                            for(int i = 0; i < 2; i++)
                                next_token(trackp + i);
                        break;
                    case MIDI_META_EVENT:
                        // this function is NOT allowed to be null;
                        (*(functions -> sysex))(trackp + i);
                        break;
                    default:
                        unget_token(trackp + i);
                        printf("Unrecognized command %d ticks: %d \n", next_token(trackp + i), ticks);
                        printf("Location: %d\n", (int)(*(trackp + i) - save));
                        return 1;
                }
            printf("not time yet\n");
            unget_token(trackp + i);
        }
        ticks++;
    }
    return 0;
}

