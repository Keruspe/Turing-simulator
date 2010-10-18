#ifndef __TYPES_H__
#define __TYPES_H__

#define MAX_FILENAME_LENGTH 100

#define BASE_STATE_NAME_SIZE 10
#define BASE_LETTER_SIZE 10

#define ALPHABET_BASE_LENGTH 10
#define STATES_BASE_LENGTH 10
#define TRANSITIONS_BASE_LENGTH 25
#define DATA_BASE_LENGTH 50

typedef char Move;
typedef char* State;
typedef char* Letter;

typedef struct _Transition Transition;
typedef struct _Machine Machine;

typedef struct _Data Data;

#endif /* __TYPES_H__ */
