#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include "settings.h"
#include <string.h>
#include <string.h>

int is_dir(const char * path){
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    } else if (ENOENT == errno) {
        return 0;
    } else {
        return -1;
    }
}

void create_config_file(){
    system("mkdir ~/.config/treebark");
    //system("touch ~/.config/c_lookup/files.txt");
    system("touch ~/.config/treebark/config.txt");

    //FILE* ffiles = (FILE *)malloc(sizeof(FILE *));
    FILE * ffiles = fopen((const char *)config_path, "w");
    if(ffiles == NULL){
        printf("cannot open configuration file\n");
        printf("FATAL ERROR\n");
        exit(-1);
    }
    fprintf(ffiles, "How to format the Treebark configuration file\nAll lines not starting with a '#' character are comments.\nLines starting with a '#' character define variables. Format:\n  #<variable-name>: <variable-value>");
    fclose(ffiles);
}

void load_defaults(){
    // this loads all variables to their default values, so if they are not set in the config the program won't break.
}

int load_settings(){
    FILE * config;
    strcpy((char *)THEME,"treebark_default");
    char * home = getenv("HOME");
    char * config_folder[255];
    strcpy((char *)config_path, home);
    strcat((char *)config_path,"/.config/treebark");
    strcpy((char *)config_css_path,(const char *)config_path);
    strcpy((char *)config_folder,(const char *)config_path);
    //strcat((char *)config_css_path,"/style.css");
    strcat((char *)config_css_path,"/themes/");
    strcat((char *)config_path,"/config.txt");

    load_defaults();

    if(is_dir((const char *)config_folder)<1){
        create_config_file();
    }

    config = fopen((const char *)config_path,"r");

    char line[100] = {0};
    char varname[20];
    char varvalue[80];
    int word = 0;
    int loc = 0;

    while (fgets(line, 100, config)){
        if (line[0]=='#'){
            for(int i = 1;i<strlen(line);i++){
                if(line[i]==':'){
                    word = 1;
                    loc = 0;
                }
                else if(line[i]==' ') continue;
                else{
                    if(word == 0){
                        varname[loc] = line[i];
                        loc++;
                    }
                    else{
                        varvalue[loc] = line[i];
                        loc++;
                    }
                }
            }
            if(strcmp(varname,"theme")==0){
                strcpy((char *)THEME,(const char *)varvalue);
            }
        }
    }

    if(strcmp((const char *)THEME,"")!=0){
        strcat((char *)config_css_path,(const char *)THEME);
        strcat((char *)config_css_path,".css");
    }

    //printf("%s, %s\n",THEME,config_css_path);

    fclose(config);
}