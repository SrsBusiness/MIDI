#include "MIDI.h"
#include <stdio.h> 

#define TRACK_LIMIT 20

callback *functions;
char *trackp[TRACK_LIMIT];
char eof[TRACK_LIMIT];
short tpqn = 24;
int mpb = 500000;
int ticks = 0;

int next_int(char *current){
    int result = 0;
    for(int i = 0; i < 4; i++)
        result = result << 8 | *(current + i);
    return result;
}

short next_short(char *current){
    short result = 0;
    for(int i = 0; i < 2; i++)
        result = result << 8 | *(current + i);
    return result;
}

int next_token(char **current){
    int result = 0;
    int i = 0;
    do{
        result = (result << 8) | **current;
    }while((i++ < 3) && ((*(*current)++ & 0x80) != 0));
    return result;
}

int init_tracks(char *file, char **trackp, int tracks){
    char *save = file;
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

char and(char *array, short length){
    char result = 1;
    for(int i = 0; i < length; i++)
        result  = result && array[i];
    return result;
}



void unget_token(char **current){
    (*current)--; 
    while((*(*current - 1)) & 0x80)
        (*current)--;
}



int MIDI_play(char *file){
    char *save = file;
    if(next_int(file) != MIDI_HEADER){
        printf("Improper Header %d %d\n", *(int *)file, MIDI_HEADER);
        return 1;
    }
    file += 4;
    if(next_int(file) != 6){
        printf("Improper Header chunk size %d\n", next_int(file));
        return 1;
    }
    file += 4;
    short type = next_short(file); // type
    file += 2;
    short tracks = next_short(file); // # of tracks
    file += 2;
    if(init_tracks(file, trackp, tracks)){
        printf("Init tracks failed\n");
        return 1;
    }
    for(int i = 0; i < tracks; i++)
        eof[i] = 0;
    tpqn = next_short(file); // time divistion
    file += 2;
    int last_ticks[TRACK_LIMIT];
    for(int i = 0; i < tracks; i++)
        last_ticks[i] = 0;
    while(!and(eof, tracks)){
        (*(functions -> tick))(mpb, tpqn);
        for(int i = 0; i < tracks; i++){
            // todo: first handle delta time token
            int time;
            printf("time offset: %d\n", (int)(trackp[i] - save));
            while((time = next_token(trackp + i)) + last_ticks[i] == ticks){
                last_ticks[i] = ticks;
                printf("time: %u\n", time);
                char command = *trackp[i]++;
                printf("command: %u\n", command);
                switch(command & 0xF0){
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
                        midi_sysex(trackp + i, command, i);
                        break;
                    default:
                        unget_token(trackp + i);
                        printf("Unrecognized command %d ticks: %d \n", next_token(trackp + i), ticks);
                        printf("Location: %d\n", (int)(*(trackp + i) - save));
                        return 1;
                }
            }
            printf("not time yet\n");
            unget_token(trackp + i);
        }
        ticks++;
    }
    return 0;
}

void midi_sysex(char **current, char command, short i){
    switch(command){
        case MIDI_META_EVENT:
            char type = *(*current)++;
            midi_meta_event(current, type, i);
            break;
        case MIDI_SYSEX_AUTH:
            if(functions -> sysex_auth)
                (*(functions -> sysex_auth))((char)next_token(current),
                        (char)next_token(current));
            else
                for(int i = 0; i < 2; i++)
                    next_token(current);
            break;
        default:
            if(functions -> sysex)
                (*(functions -> sysex))(current);
            else
                while(*((*current)++) != 0xF7);
            break;
    }
}

void midi_meta_event(char **current, char type, short i){
    switch(type){
        case MIDI_ME_SEQUENCE_NUM:
            (*current) += 3;
            break;
        case MIDI_ME_TEXT:
            if(!(functions -> me_text))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_text))(text);
            break;
        case MIDI_ME_COPYRIGHT:
            if(!(functions -> me_copyright))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_copyright))(text);
            break;
        case MIDI_ME_SEQUENCE_TRACK:
            if(!(functions -> me_sequence_track))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_sequence_track))(text);
            break;
        case MIDI_ME_INSTRUMENT:
            if(!(functions -> me__instrument))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_instrument))(text); 
            break;
        case MIDI_ME_LYRICS:
            if(!(functions -> me_lyrics))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_lyrics))(text);
            break;
        case MIDI_ME_MARKER:
            if(!(functions -> me_marker))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_marker))(text);
            break;
        case MIDI_ME_CUE_POINT:
            if(!(functions -> me_cue_point))
                return;
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_cue_point))(text);
            break;
        case MIDI_CHANNEL_PREFIX:
            (*current) += 2;
            break;
        case MIDI_EOT:
            eof[i] = 1; 
            break;
        case MIDI_SET_TEMPO:
            mpb = next_int(*current);
            (*current) += 4;
            break;
        case MIDI_SMPTE_OFFSET:
            (*current) += 6;
            break;
        case MIDI_TIME_SIGNATURE:
            (*current) += 5;
            break;
        case MIDI_KEY:
            (*current) += 3;
            break;
        case MIDI_SEQUENCER:
            int length = next_token(current);
            char text[length + 1];
            for(int i = 0; i < length; i++){
                text[i] = *(*current)++;
            }
            text[length] = 0;
            (*(functions -> me_sequencer))(text);
            break;
        default:
            break;
    }
}
