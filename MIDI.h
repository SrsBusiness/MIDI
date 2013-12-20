#define MIDI_HEADER 0x4D546864
#define MIDI_TRACK  0x4D54726B

// Controller Types
#define MIDI_BANK_SELECT 0x00
#define MIDI_MODULATION 0x01
#define MIDI_BREATH_CONTROLLER 0x02
#define MIDI_FOOT_CONTROLLER 0x04
#define MIDI_PORTAMENTO_TIME 0x05
#define MIDI_DATA_ENTRY 0x06
#define MIDI_MAIN_VOLUME 0x07
#define MIDI_BALANCE 0x08
#define MIDI_PAN 0x0A
#define MIDI_EXPRESSION_CONTROLLER 0x0B
#define MIDI_EFFECT_CONTROL_1 0x0C
#define MIDI_EFFECT_CONTROL_2 0x0D
#define MIDI_GPCONTROLLER_1 0x10
#define MIDI_GPCONTROLLER_2 0x11
#define MIDI_GPCONTROLLER_3 0x12
#define MIDI_GPCONTROLLER_4 0x13
#define MIDI_DAMPER_PEDAL 0x40
#define MIDI_PORTAMENTO 0x41
#define MIDI_SOSTENUTO 0x42
#define MIDI_SOFT_PEDAL 0x43
#define MIDI_LEGATO_FOOTSWITCH 0x44
#define MIDI_HOLD_2 0x45
#define MIDI_SOUND_CONTROLLER_1 0x46
#define MIDI_SOUND_CONTROLLER_2 0x47
#define MIDI_SOUND_CONTROLLER_3 0x48
#define MIDI_SOUND_CONTROLLER_4 0x49
#define MIDI_SOUND_CONTROLLER_5 0x4A
#define MIDI_SOUND_CONTROLLER_6 0x4B
#define MIDI_SOUND_CONTROLLER_7 0x4C
#define MIDI_SOUND_CONTROLLER_8 0x4D
#define MIDI_SOUND_CONTROLLER_9 0x4E
#define MIDI_SOUND_CONTROLLER_10 0x4F
#define MIDI_GPCONTROLLER_5 0x50
#define MIDI_GPCONTROLLER_5 0x51
#define MIDI_GPCONTROLLER_5 0x52
#define MIDI_GPCONTROLLER_5 0x53
#define MIDI_PORTAMENTO_CONTROL 0x54
#define MIDI_EFFECTS_1_DEPTH 0x5B
#define MIDI_EFFECTS_2_DEPTH 0x5C
#define MIDI_EFFECTS_3_DEPTH 0x5D
#define MIDI_EFFECTS_4_DEPTH 0x5E
#define MIDI_EFFECTS_5_DEPTH 0x5F
#define MIDI_DATA_INCREMENT 0x60
#define MIDI_DATA_DECREMENT 0x61

// MIDI events
#define MIDI_NOTE_OFF 0x08
#define MIDI_NOTE_ON 0x09
#define MIDI_NOTE_AFTERTOUCH 0x0A
#define MIDI_CONTROLLER 0x0B
#define MIDI_PROGRAM_CHANGE 0x0C
#define MIDI_CHANNEL_AFTERTOUCH 0x0D
#define MIDI_PITCH_BEND 0x0E
#define MIDI_SYSEX  0xF0
#define MIDI_SYSEX_DIV/AUTH  0xF7
#define MIDI_META_EVENT 0xFF

// meta event types
#define MIDI_ME_SEQUENCE_NUM    0x00
#define MIDI_ME_TEXT    0x01
#define MIDI_ME_COPYRIGHT 0x02
#define MIDI_ME_SEQUENCE/TRACK  0x03
#define MIDI_ME_INSTRUMENT  0x04
#define MIDI_ME_LYRICS  0x05
#define MIDI_ME_MARKER  0x06
#define MIDI_ME_CUE_POINT 0x07
#define MIDI_CHANNEL_PREFIX 0x20
#define MIDI_EOT    0x2F // End Of Track
#define MIDI_SET_TEMPO 0x51
#define MIDI_SMPTE_OFFSET   0x54
#define MIDI_TIME_SIGNATURE 0x58
#define MIDI_KEY    0x59
#define MIDI_SEQUENCER 0x7F


//note pitches
#define MIDI_C_0    0x00
#define MIDI_C#_0   0x01
#define MIDI_D_0    0x02
#define MIDI_D#_0   0x03
#define MIDI_E_0    0x04
#define MIDI_F_0    0x05
#define MIDI_F#_0   0x06
#define MIDI_G_0    0x07
#define MIDI_G#_0   0x08
#define MIDI_A_0    0x09
#define MIDI_A#_0   0x0A
#define MIDI_B_0    0x0B
#define MIDI_C_1    0x0C
#define MIDI_C#_1   0x0D
#define MIDI_D_1    0x0E
#define MIDI_D#_1   0x0F
#define MIDI_E_1    0x10
#define MIDI_F_1    0x11
#define MIDI_F#_1   0x12
#define MIDI_G_1    0x13
#define MIDI_G#_1   0x14
#define MIDI_A_1    0x15
#define MIDI_A#_1   0x16
#define MIDI_B_1    0x17
#define MIDI_C_2    0x18
#define MIDI_C#_2   0x19
#define MIDI_D_2    0x1A
#define MIDI_D#_2   0x1B
#define MIDI_E_2    0x1C
#define MIDI_F_2    0x1D
#define MIDI_F#_2   0x1E
#define MIDI_G_2    0x1F
#define MIDI_G#_2   0x20
#define MIDI_A_2    0x21
#define MIDI_A#_2   0x22
#define MIDI_B_2    0x23
#define MIDI_C_3    0x24
#define MIDI_C#_3   0x25
#define MIDI_D_3    0x26
#define MIDI_D#_3   0x27
#define MIDI_E_3    0x28
#define MIDI_F_3    0x29
#define MIDI_F#_3   0x2A
#define MIDI_G_3    0x2B
#define MIDI_G#_3   0x2C
#define MIDI_A_3    0x2D
#define MIDI_A#_3   0x2E
#define MIDI_B_3    0x2F
#define MIDI_C_4    0x30
#define MIDI_C#_4   0x31
#define MIDI_D_4    0x32
#define MIDI_D#_4   0x33
#define MIDI_E_4    0x34
#define MIDI_F_4    0x35
#define MIDI_F#_4   0x36
#define MIDI_G_4    0x37
#define MIDI_G#_4   0x38
#define MIDI_A_4    0x39
#define MIDI_A#_4   0x3A
#define MIDI_B_4    0x3B
#define MIDI_C_5    0x3C
#define MIDI_C#_5   0x3D
#define MIDI_D_5    0x3E
#define MIDI_D#_5   0x3F
#define MIDI_E_5    0x40
#define MIDI_F_5    0x41
#define MIDI_F#_5   0x42
#define MIDI_G_5    0x43
#define MIDI_G#_5   0x44
#define MIDI_A_5    0x45
#define MIDI_A#_5   0x46
#define MIDI_B_5    0x47
#define MIDI_C_6    0x48
#define MIDI_C#_6   0x49
#define MIDI_D_6    0x4A
#define MIDI_D#_6   0x4B
#define MIDI_E_6    0x4C
#define MIDI_F_6    0x4D
#define MIDI_F#_6   0x4E
#define MIDI_G_6    0x4F
#define MIDI_G#_6   0x50
#define MIDI_A_6    0x51
#define MIDI_A#_6   0x52
#define MIDI_B_6    0x53
#define MIDI_C_7    0x54
#define MIDI_C#_7   0x55
#define MIDI_D_7    0x56
#define MIDI_D#_7   0x57
#define MIDI_E_7    0x58
#define MIDI_F_7    0x59
#define MIDI_F#_7   0x5A
#define MIDI_G_7    0x5B
#define MIDI_G#_7   0x5C
#define MIDI_A_7    0x5D
#define MIDI_A#_7   0x5E
#define MIDI_B_7    0x5F
#define MIDI_C_8    0x60
#define MIDI_C#_8   0x61
#define MIDI_D_8    0x62
#define MIDI_D#_8   0x63
#define MIDI_E_8    0x64
#define MIDI_F_8    0x65
#define MIDI_F#_8   0x66
#define MIDI_G_8    0x67
#define MIDI_G#_8   0x68
#define MIDI_A_8    0x69
#define MIDI_A#_8   0x6A
#define MIDI_B_8    0x6B
#define MIDI_C_9    0x6C
#define MIDI_C#_9   0x6D
#define MIDI_D_9    0x6E
#define MIDI_D#_9   0x6F
#define MIDI_E_9    0x70
#define MIDI_F_9    0x71
#define MIDI_F#_9   0x72
#define MIDI_G_9    0x73
#define MIDI_G#_9   0x74
#define MIDI_A_9    0x75
#define MIDI_A#_9   0x76
#define MIDI_B_9    0x77
#define MIDI_C_10    0x78
#define MIDI_C#_10   0x79
#define MIDI_D_10    0x7A
#define MIDI_D#_10   0x7B
#define MIDI_E_10    0x7C
#define MIDI_F_10    0x7D
#define MIDI_F#_10   0x7E
#define MIDI_G_10    0x7F

