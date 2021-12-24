#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/*
any loose text is a comment
#variable-name: value(whitespace will be stripped)
*/

char * config_path[100];
char * config_css_path[255];

char * THEME[80];

int load_settings();

#endif