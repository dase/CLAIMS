/*
 * command_line.cpp
 *
 *  Created on: Mar 10, 2015
 *      Author: wangli
 */
#include "command_line.h"

bool input_struct::append(const char* str) {
    while(*str)
        string[index++]=*str++;
    if(index!=string_length)
        return true;
    else
        return false;
}

void input_struct::reset() {
    memset(string,0,string_length);
    index=0;
}

input_struct::input_struct():index(0){
    memset(string,0,string_length);
};

bool input_struct::append_til(const char* str, char target,
		bool& target_match) {
    while(*str&&*str!=target)
        string[index++]=*str++;
    if(*str==target){
        string[index++]=*str++;
        target_match=true;
    }
    else
        target_match=false;
    if(index!=string_length)
        return true;
    else
        return false;
}

void recovery_history() {
    string line;
    ifstream input_file("./.history");
    input_struct is;
    if(!input_file)
        return;
    while(!input_file.eof()){
        bool last_line;
        getline(input_file,line);
        is.append_til(line.c_str(),';',last_line);
        if(last_line){
            add_history(is.string);
            is.reset();
        }
        else{
        	is.append("\n");
        }
    }
    input_file.close();
}

void init_command_line() {
    // Configure readline to auto-complete paths when the tab key is hit.
    rl_bind_key('\t', rl_complete);

    // Restore the history from file
    recovery_history();
}

void store_history(char* command) {
    string str(command);
    ofstream output_file("./.history",std::ofstream::out | std::ofstream::app);
    output_file<<str<<endl;
}

int get_one_command(string& str) {
    char* input_line, shell_prompt[100];
    string line;

	// Create prompt string from user name and current working directory.
	snprintf(shell_prompt, sizeof(shell_prompt), "CLAIMS>");

	// Display prompt and read input (n.b. input must be freed after use)...
	bool first_line=true;
	bool last_line=false;
	input_struct is;
	while(true){
		if(!first_line)
			snprintf(shell_prompt,sizeof(shell_prompt),"     > ");
		first_line=false;
		input_line = readline(shell_prompt);
		is.append_til(input_line,';',last_line);
		if(last_line)
			break;
		is.append("\n");
		free(input_line);
	}

	// Add input to history.
	add_history(is.string);
	store_history(is.string);

	printf("input is %s\n",is.string);
	str=string(is.string);

}
