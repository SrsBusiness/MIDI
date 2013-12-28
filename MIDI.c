#include "MIDI.h"
//#include <stdio.h> 
#include "tm4c123gh6pm.h"

#define TRACK_LIMIT 20

callback *functions;

int init_tracks(unsigned char *file, unsigned char **trackp, int tracks){
    while(file && *(unsigned int *)file != MIDI_TRACK)
        file++;
    if(!file)
        return 1;
    trackp[0] = file;
    for(int i = 1; i < tracks; i++){
        file += 4;
        file += 4 + *(unsigned int *)file;
        trackp[i] = file; 
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

int next_token(unsigned char **current){
    int result = 0;
    do
        result = (result << 8) | *((*current));
    while((*(*current)++) & 0x80);
    return result;
}

void unget_token(unsigned char **current){
    (*current)--; 
    while((*--(*current)) & 0x80);
    current++;
}

int MIDI_play(unsigned char *file){
    if(*(unsigned int *)file != MIDI_HEADER)
        return 1;
    file += 8;
    unsigned short type = *(unsigned short *)file;
    file += 2;
    unsigned short tracks = *(unsigned short *)file;
    unsigned char *trackp[TRACK_LIMIT];
    if(init_tracks(file, trackp, tracks))
        return 1;
    unsigned char eof[TRACK_LIMIT];
    for(int i = 0; i < tracks; i++)
        eof[i] = 0;
    unsigned short tpqn = *(unsigned short *)file;
    file += 2;
    unsigned int last_ticks[TRACK_LIMIT];
    for(int i = 0; i < tracks; i++)
        last_ticks[i] = 0;
    unsigned int ticks = 0;
    while(!and(eof, tracks)){
        (*(functions -> tick))();
        for(unsigned int i = 0; i < tracks; i++){
            // todo: first handle delta time token
            while(next_token(trackp + i) + last_ticks[i] == ticks)
                switch(next_token(trackp + i)){
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
                        return 1;
                }
            unget_token(trackp + i);
        }
        ticks++;
    }
    return 0;
}

