    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <ctype.h>
    #include <dirent.h>
    #include <errno.h>
    #include "shellProject.h"


    //////////////////////////////////////////////////////
    ///////Builtin Functions//////////////////////////////
    //////////////////////////////////////////////////////

    char* remove_spaces(char * source, char * target){
        while(*source++ && *target){
            if (!isspace(*source))
                 *target++ = *source;
        }

        return target;
    }

    void getCurrentPath(){
        char * cwd;
        cwd = getcwd (0, 0);
        if (! cwd) {
            fprintf (stderr, "getcwd failed: %s\n", strerror (errno));
        }
        else {
            //printf("\n\nI am here\n\n");
            //printf("%s\n", cwd);
            currentWorkDir = cwd;
            free(cwd);
            //printf("\n\nI am here\n\n");
        }
    }

    void goHome(){
        strcpy(currLoc, home);
        chdir(home);
    }

    void goPath(const char* thePathStr){
        getCurrentPath();
        printf("\n\nI am here\n\n");
        printf("%s", currentWorkDir);
        printf("\n\nI am here\n\n");

        strcpy(currLoc, currentWorkDir);
        strcat(currLoc, "/");
        strcat(currLoc, thePathStr);
        currentWorkDir = currLoc;
        char *dir = "";
        dir = remove_spaces(currentWorkDir, dir);

        printf("\n\nI am here\n\n");
        printf("%s", dir);
        printf("\n\nI am here\n\n");

        chdir(currentWorkDir);
    }

    void do_print_Alias(struct AliasNode* alias) {
        char* toPrint = alias->key;
            printf("%s", toPrint);
        printf(": ");
            toPrint = alias->value;
            printf("%s", toPrint);
            printf("\n");
    }

    void printAlias() {
        if (aliasHead == 0) {
            printf("No aliases have been created.");
            return;
        }
        else {
            struct AliasNode* current = aliasHead;
            printf("Current Aliases: \n");
            while (current != 0) {
                do_print_Alias(current);
                current = current->next;
            }
        }
    }

    void goLS(){
        DIR *dirp;
        struct dirent* dir;
        dirp = opendir(currentWorkDir);
        if(dirp){
            while((dir = readdir(dirp)) != NULL){
                printf("%s\n", dir->d_name);
            }
            closedir(dirp);
        }
    }

    // void goLSWord(){
    //     DIR *dirp;
    //     struct dirent *dir;
    //     dirp = opendir(".");
    //     int flag = 0;
    //     if (dirp)
    //     {
    //         while ((dir = readdir(dirp)) != NULL){
    //             if(strcmp(fileName, dir->d_name) == 0){
    //                 flag = 1;
    //                 DIR *d2;
    //                 struct dirent *dir2;
    //                 d2 = opendir(dir->d_name);
    //                 if (d2)
    //                 {
    //                     while ((dir2 = readdir(d2)) != NULL)
    //                     {
    //                         printf("%s\n", dir2->d_name);
    //                     }
    //                 closedir(d2);
    //                 }
    //             }
    //         }
    //     closedir(dirp);
    //     if(flag == 0)
    //         printf("Directory %s does not exist.\n", fileName);
    //     }
    // }

    ///////////////////////////////////////////////////////
    ///////Error Handling//////////////////////////////////
    ///////////////////////////////////////////////////////

    void understand_errors(){
        printf("An error occured while parsing the yacc file.\n");
    }

    void recover_from_errors(){
        // Find out if error occurs in middle of command
        // That is, the command still has a “tail”
        // In this case you have to recover by “eating”
        // the rest of the command.
        // To do this: use yylex() directly.
        printf("Command Error!!!");
    }

    ///////////////////////////////////////////////////////
    ///////Shell Functions/////////////////////////////////
    ///////////////////////////////////////////////////////

    int check_in_file(){
        return 0;
    }

    int check_out_file(){
        return 0;
    }

    void init_scanner_and_parser(){
        // Need to initialize all the variables
        argc = 0;
        bicmd = 0;
        builtin = 0;
    }

    void printPrompt(){
        getCurrentPath();
        printf("%s", currentWorkDir);
        printf(" || SHELL---->$");
        return;
    }

    void shell_init(){
        //init all variables
        currcmd = 0;

        //define (allocate storage) for some var/tables
        //struct alias aliastab[MAXALIAS];
        //struct env envtab[MAXENV];

        //init all tables (e.g., alias table)
        //get PATH environment variable (use getenv())
        char *currPath = getenv("PATH");
        char *homePath = getenv("HOME");

        char* dlim = ":";
        char* tok = strtok(currPath, dlim);

        int i = 0;
        while(tok != NULL){
            pathtab[i] = tok;
            i++;
            tok = strtok(NULL, dlim);
        }
        pathtab[i] = NULL;
        //printf("two: %d\n", strlen(pathtab));
        //get HOME env variable (also use getenv())

        //printf("%s", home);
        home = homePath;
        //disable anything that can kill your shell
        //(the shell should never die; only can be exit)
        //do anything you feel should be done as init
        return;


        // init all variables.
        // define (allocate storage) for some var/tables
        // init all tables (e.g., alias table)

        //Dont require
        // get PATH environment variable (use getenv())
        // get HOME env variable (also use getenv())

        // disable anything that can kill your shell.
        // (the shell should never die; only can be exit)
        // do anything you feel should be done as init
    }

    void do_it(){
        switch (bicmd) {
          case CDHome_CMD:
                goHome();
                break;
          case CDPath_CMD:
                goPath(strPath);
                break;
          case LS_CMD:
                goLS();
          case LSWord_CMD:
                //goLSWord();
          case ALIAS_CMD:
                printAlias();
                break;
          case UNALIAS_CMD:
                break;
          case SETENV_CMD:
                break;
          case UNSETENV_CMD:
                break;
          case PRINTENV_CMD:
                break;
          case NEWLINE_CMD:
                break;
          default:
                break;
        }
    }

    void undoit(){
        //Need to define this function
    }

    void execute_it(){
        // Handle  command execution, pipelining, i/o redirection, and background processing.
        // Utilize a command table whose components are plugged in during parsing by yacc.

        /*
         * Check Command Accessability and Executability
         */
        if(isExe == 0) {
            //use access() system call with X_OK
            return;
        }

        /*
         * Check io file existence in case of io-redirection.
         */
        if( check_in_file() == SYSERR ) {
            //nuterr("Cann't read from : %s",srcf);
            return;
        }
        if( check_out_file() == SYSERR ) {
            //nuterr("Cann't write to : %s",distf);
            return;
        }

        //Build up the pipeline (create and set up pipe end points (using pipe, dup)
        //Process background
    }

    int executable(){
        int i = 0;
        for(i = 0; pathtab[i] != NULL; ++i){
            int len = strlen(pathtab[i]) + strlen(input_command) + strlen("/"); // add 1 for the '/'
            executable_path = malloc(len);
            strcpy(executable_path, pathtab[i]);
            strcat(executable_path, "/");
            strcat(executable_path, input_command);
            int retVal = access(executable_path, X_OK);
            if(retVal == 0)
                return OK;
        }
        return SYSERR;
    }

    void processCommand(){
        if (builtin)
            do_it();
        else
            execute_it();
    }

    int getCommand(){
        init_scanner_and_parser();
        if (yyparse()) {
            understand_errors();
            return ERRORS;
        }
        else {
            if (bicmd == BYE_CMD) {
                return BYE_CMD;
            }
            else {
                return OK;
            }
        }
    }

    int main(){
        printf("Welcome to the shell!!!!\n");
        shell_init();
        while (1) {
            printPrompt();
            switch (CMD = getCommand()) {
                case BYE_CMD:
                    exit(0);
                case ERRORS:
                    recover_from_errors();
                    break;
                case OK:
                    processCommand();
                    break;
            }
        }
        return 0;
    }
