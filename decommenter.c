/*  
    Final Project : De-comment Progral  Section-01
    File_Name - decommenter.c  

    Group Members  - Abenezer Kindie (ATR/5410/09)
                   - Dawit Yonas (ATR/7114/09)
                   - Daniel Geremew (ATR/8104/09)
*/


#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

/*  a global enum to keep track of the state declared as 'State' */

typedef enum StateEnum {
    IN_RAW_CODE,
    TO_START_A_COMMENT, 
    IN_COMMENT,
    TO_END_A_COMMENT,
    IN_STRING, 
    STRING_ESCAPE, 
    IN_CHAR,
    CHAR_ESCAPE
} State;

/*  function declarations that are used to handle each state  */

void handleInRawCodeState(State *state, char in);
void handleInStringState(State *state, char in);
void handleStringEscapeState(State *state, char in);
void handleInCharState(State *state, char in);
void handleCharEscapeState(State *state, char in);
void handleToStartACommentState(State *state, char in,int *commentStartLine,int *curruntLine);
void handleInCommentState(State *state, char in);
void handleToEndACommentState(State *state, char in);

void countLine(int *line, char in);
void checkCompletion(State *state, int lastCommentline);

/*  takes input as characters and removes comments  */

void main(){

    State state = IN_RAW_CODE;

    int line = 1;  /*  to keep track of the number of lines  */
    int commentStartLine = 1;  /*  used to track of where the last unterminated comment started  */

    for(;;){
        char in = getchar();
        countLine(&line, in);

        if (in == EOF) {
            checkCompletion(&state,  commentStartLine);
            break;
        }

        switch (state) {
            case IN_RAW_CODE:
                handleInRawCodeState(&state, in);
            break;
            case IN_STRING:
                handleInStringState(&state, in);
            break;
            case STRING_ESCAPE:
                handleStringEscapeState(&state, in);
            break;
            case IN_CHAR:
                handleInCharState(&state, in);
            break;
            case CHAR_ESCAPE:
                handleCharEscapeState(&state, in);
            break;
            case TO_START_A_COMMENT:
                handleToStartACommentState(&state, in,&commentStartLine,&line);
            break;
            case IN_COMMENT:
                handleInCommentState(&state, in);
            break;
            case TO_END_A_COMMENT:
                handleToEndACommentState(&state, in);
            break;
        }
    }
}

/*  writes out the 'in' argument if it's not in TO_START_A_COMMENT state
    and changes the state to appropriate states based on 'in' content.*/

void handleInRawCodeState(State *state, char in) {
    
    if (in == '\"') {
        *state = IN_STRING;
    } 
    else if (in == '\'') {
        *state = IN_CHAR;
    } 
    else if (in == '/') {
        *state = TO_START_A_COMMENT;
    }

    if (*state != TO_START_A_COMMENT) {
        putchar(in);
    }
};

/*  writes out characters entered that are within a " ". 
    changes state to appropriate state depending on the "in" content  */

void handleInStringState(State *state, char in) {

    if (in == '\\') {
        *state = STRING_ESCAPE;
    }
    else if (in == '\"') {
        *state = IN_RAW_CODE;
    }
    putchar(in);

};

/*  writes out characters entered that are escaped strings and 
    changes state to IN_STRING  */

void handleStringEscapeState(State *state, char in) {

    *state = IN_STRING;
    putchar(in);

};

/*  writes out characters entered that are within a ' '. 
    changes state to appropriate state depending on the "in" content  */

void handleInCharState(State *state, char in) {

    if (in == '\\') {
        *state = CHAR_ESCAPE;
    } 
    else if (in == '\'') {
        *state = IN_RAW_CODE;
    }
    putchar(in);

};

/*  writes out characters entered that are escaped chars and 
    changes state to IN_CHAR  */

void handleCharEscapeState(State *state, char in) {

    *state = IN_CHAR;
    putchar(in);

};

/*  handles the /* characters which show beginning of a comment. 
    changes state to appropriate state depending on the "in" content 
    And the line at which unterminated comment started will be tracked */

void handleToStartACommentState(State *state, char in,int *commentStartLine,int *curruntLine) {

    if (in == '*') {
        *state = IN_COMMENT;
        *commentStartLine = *curruntLine;
    } 
    else if ((in == '/') && *state == TO_START_A_COMMENT) {
        putchar(in);
        *state = TO_START_A_COMMENT;
    }  
    else if (in == '/') {
        *state = TO_START_A_COMMENT;
    } 
    else {
        *state = IN_RAW_CODE;
    }

    if (*state == IN_RAW_CODE) {
        putchar('/');
        putchar(in);
    }

};

/*  excludes lines that are comment from the print stream. 
    changes state to approptiate states depending on the conditions met */

void handleInCommentState(State *state, char in) {

    if (in == '*') {
        *state = TO_END_A_COMMENT;
    }

    if (in == '\n') {
        putchar(in);
    }

};

/*  handeles end of a comment ( * /) and 
    changes state to appropriate state based on the contents of in  */

void handleToEndACommentState(State *state, char in) {

    if (in == '/') {
        *state = IN_RAW_CODE;
    } else if (in == '*') {
        *state = TO_END_A_COMMENT;
    } else {
        *state = IN_COMMENT;
    }

    if (*state == IN_RAW_CODE) {
        putchar(' ');
    }

};

/*  check if there are any incomplete comments in the file and 
    throw an error with the corresponding line number  */

void checkCompletion(State *state, int lastCommentline) {

    if (*state == IN_COMMENT || *state == TO_END_A_COMMENT) {
        fprintf(stderr, "Error: Comment started on line  %d  is unterminated comment.\n", lastCommentline);
    }

}

/* Keeps track of lines processed  */ 

void countLine(int *line, char in) {

    if (in == '\n') {
        *line += 1;
    }

}