#include "mylain/repl.h"
#include "mylain/global.h"
#include "mylain/lexer.h"

void lain_repl(void)
{
    puts(LAIN_MOTTO);
    while(LAIN_LOCAL_RUNNING) {
        // Get our own commands
        if(LAIN_REMOTE_COM_QUEUE->amount > 0ul)
            printf("[%llu] ", LAIN_REMOTE_COM_QUEUE->amount);
        char* input = readline("lain > ");
        {
            unsigned i;
            for(i = 0; i < strlen(input); i++) {
                input[i] = tolower(input[i]);
            }
        }
        add_history(input);
        
        unsigned r = lain_eval(input);

        switch(r) {
        case LAIN_RETURN_SUCCESS:
            puts("!ans{Ok}");
            break;
        case LAIN_RETURN_ONGOING:
            puts("!ans{Err:missingargs}");
            lain_reset_all();
            break;
        default:
            printf("!ans{Err:0x%08X}\n"
                   "On command: \"%s\"\n", r, input);
            lain_reset_all();
            break;
        };
        free(input);
    }
}

unsigned lain_eval(char* command)
{
    char* cr_literal = strtok(command, " ");
    unsigned cr_returnval = LAIN_RETURN_FAILURE;
    while(cr_literal != NULL) {
        enum LAIN_COMMAND comm = lain_get_command(cr_literal);
        
        switch(comm) {
        case LAIN_COM_QUIT:
            LAIN_LOCAL_RUNNING = 0u;
            cr_returnval = LAIN_RETURN_SUCCESS;
            break;
            
        case LAIN_COM_ATOM:
            if(!LAIN_MSTATE) {
                printf("atom{%s}\n", cr_literal);
            }
            break;

        default:
            LAIN_MSTATE |= comm;
            break;
        };

        cr_returnval = lain_dispatch(cr_literal, comm);
        cr_literal = strtok(NULL, " ");
    }
    cr_returnval = lain_dispatch(NULL, LAIN_COM_END);
    return cr_returnval;
}
