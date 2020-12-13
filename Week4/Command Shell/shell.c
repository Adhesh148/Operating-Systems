#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#define MAX 1024

void init_shell();
char** parse_args(char *cmd_line);
int execute_args(char **cmd_args,char cwd[]);
int isInBuiltCmd(char *cmd);
int executeInBuiltCmd(char **cmd_args,char cwd[]);
void historyLog(char *cmd_line,char cwd[]);

int main()
{
	// start the shell
	init_shell();
}

void init_shell()
{
	// get current directory
	char cwd[MAX];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd,"/history.txt");

	// Initiate shell loop.
	while(1)
	{
		printf("@ ");

		// Wait for user input - read a line
		char *cmd_line;
		size_t line_size = 1024;
		cmd_line = (char *)malloc(line_size * sizeof(char));
		if(cmd_line == NULL)
		{
			perror("Unable to allocate memory");
			exit(0);
		}

		// getline() gets the entire line of input. replace last character with EOF
		size_t len = getline(&cmd_line,&line_size,stdin);
		if(cmd_line[0] == '\n')
			continue;
		cmd_line[len-1] = 0;

		// Write to history file
		historyLog(cmd_line,cwd);

		// Parse the user input
		char** cmd_args = parse_args(cmd_line);

		// Execute the command line args
		int status;
		status = execute_args(cmd_args,cwd);

		// Free all variables used
		free(cmd_line);
		free(cmd_args);
	}

}

char** parse_args(char *cmd_line)
{
	// Split the cmd_line on spaces
	char **arg_list = malloc(MAX * sizeof(char*));

	// Extract the first argument
	char *arg = strtok(cmd_line," ");
	int pos = 0;
	while(arg!=NULL)
	{
		arg_list[pos++] = arg;
		arg = strtok(NULL," ");
	}
	return arg_list;
}

int execute_args(char **cmd_args,char cwd[])
{	
	int status = 0;

	// Check if command is in-built
	if(isInBuiltCmd(cmd_args[0]) == 1)
		status = executeInBuiltCmd(cmd_args,cwd);
	else
	{
		// Execute using execvp and parent waits for process completion
		pid_t pid;

		// Create process
		pid = vfork();
		if(pid == 0)
		{	
			// Within child
			status = 1;
			if(execvp(cmd_args[0],cmd_args) == -1)
			{
				printf("bash: Invalid command.\n");
				status = 0;
			}
			exit(0);
		}
		else if(pid > 0)
		{
			// Within parent
			while(wait(NULL) > 0);
		}
	}
	return status;
}

int isInBuiltCmd(char *cmd)
{
	char *inBuiltCmds[] = {"cd", "exit","help"};
	int num_cmds = sizeof(inBuiltCmds)/sizeof(inBuiltCmds[0]);

	// Compare strings to see if the cmd is In-Built command
	int i;
	for(i=0;i<num_cmds;++i)
		if(strcmp(cmd,inBuiltCmds[i]) == 0)
			return 1;

	if(cmd[0] == 33)
		return 1;

	return 0;
}

int executeInBuiltCmd(char **cmd_args,char cwd[])
{
	// cd command.
	if(strcmp("cd",cmd_args[0]) == 0)
	{
		if(cmd_args[0] == NULL)
		{
			printf("bash: cd: Argument missing.\n");
			return 0;
		}
		else
		{
			if(chdir(cmd_args[1])!=0)
			{
				printf("bash: cd: No such file or directory.\n");
				return 0;
			}
		}
		return 1;
	}

	// exit command
	if(strcmp("exit",cmd_args[0]) == 0)
	{
		// quit program
		exit(0);
	}

	// history command
	int num_history = 0;
	if(cmd_args[0][0] == 33)
	{
		if((num_history = isNumber(cmd_args[0])) > 0)
		{
			int retrieve_len = retrieveHistory(num_history,cwd);
		}
		else
		{
			printf("bash: history: Invalid argument passed.\n");
			return 0;
		}
	}

	// printf("inBuilt\n");
	return 0;
}

void historyLog(char *cmd_line,char cwd[])
{
	FILE *fp = fopen(cwd,"a+");
	fprintf(fp, "%s\n",cmd_line);
	fclose(fp);
}

int isNumber(char *cmd)
{
	// printf("Number = %s\n",cmd);
	int pos = 1;
	int len = strlen(cmd);
	for(;pos<len;++pos)
	{
		if(cmd[pos] < 48 || cmd[pos] > 57)
			return 0;
	}

	cmd = cmd+1;
	return atoi(cmd);
}

int retrieveHistory(int num_history,char cwd[])
{	
	FILE *fp = fopen(cwd,"r");
	char record_list[MAX][MAX];
	char record[MAX];
	int i,num=0;
	int size = 1024;
	long int record_length = 0;

	while (fgets(record, size, fp))
	{
		record_length = strlen(record);
		record[record_length-1] = 0;
		strcpy(record_list[num],record);
		num = num + 1;
	}
	
	if(num < num_history)
	{
		for(i=0;i<num;++i)
			printf("%s\n",record_list[i]);
		printf("hist: only %d records available..\n",num);
		return num;
	}
	else
	{
		for(i=(num - num_history);i<num;++i)
			printf("%s\n",record_list[i]);
	}

	return num_history;
}