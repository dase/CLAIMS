#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

/* The maximum length of a command */
const int string_length=1024;

/* A auxiliary struct storing command */
struct input_struct{
    input_struct();
    /* reset the cotent of the command */

    void reset();

    /* add a string to the command */
    bool append(const char* str);

    /* add str to string until a char specified by target is read.*/
    bool append_til(const char* str, char target, bool & target_match);
    /* the space holding the command */
    char string[string_length];
    /* the index indicating the end of the command */
    int index;
};


/* recovery history from file ".history" */
void recovery_history();

/* store current comment to the ".history" file */
void store_history(char* command );

void init_command_line();

/** get one command from terminal.
 * Note: init_command_line() must be called before.
 */
int get_one_command(string &str);
