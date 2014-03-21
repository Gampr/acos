#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "header.h"

#define BAD_PARSE -1

struct struct_proc {
    char * name;
    size_t count_argues;    
    char ** argues;
    char left_separ, right_separ;
    char * input;
    size_t count_outp;
    char ** output;
    
    
};
typedef struct struct_proc proc;

struct struct_job {
    char background;
    size_t count_procs;
    proc * procs;    
};
typedef struct struct_job job;

size_t parse_str(const char * str);
int parse_job(job * cur_job, char * sjob);
int parse(char * str, job ** jobs, size_t * count);
int parse_proc(proc * cur_proc, char * sproc, char lsep, char rsep);


int main(int argc, char ** argv) {
    char * string;
    size_t count;
    job * jobs;
    safe_gets(stdin, &string);
    parse(string, &jobs, &count);

    return 0;
}

int parse_proc(proc * cur_proc, char * sproc, char lsep, char rsep) {
    size_t len;
    size_t i_sym;
    size_t c_proc;
    size_t pred;
    char prs[2];
    (cur_proc)->input = NULL;
    (cur_proc)->count_argues = 0;
    (cur_proc)->output = NULL;
    (cur_proc)->left_separ = lsep;
    (cur_proc)->right_separ = rsep;
    len = strlen(sproc);
    for(i_sym = 0; i_sym < len; ++ i_sym) {
        i_sym += parse_str(&(sproc[i_sym]));
        if((i_sym == 0 || sproc[i_sym] != 0) && sproc[i_sym] == '>')
            (cur_proc)->count_outp++;
    }
    (cur_proc)->output = (char **)malloc((cur_proc)->count_outp * sizeof(char *));
    for(i_sym = c_proc = pred = prs[0] = prs[1] = 0; i_sym < len; ++ i_sym) {
        i_sym += parse_str(&(sproc[i_sym]));
        if(sproc[i_sym] == '>' || sproc[i_sym] == '<') {
            if(sproc[i_sym] == '>' && sproc[i_sym + 1] == '>') {
                i_sym ++;
                prs[0] = prs[1] = '>';
                //add flag >>
            }
            else {
                prs[0] = 0;
                prs[1] = sproc[i_sym];
            }
        }
    }

    return 0;
}

int parse_job(job * cur_job, char * sjob) {
    size_t len;
    size_t i_sym;
    size_t c_proc;
    size_t pred;
    size_t bg;
    (cur_job)->count_procs = 1;
    len = strlen(sjob);
    bg = 0;
    for(i_sym = 0; i_sym < len; ++ i_sym) {
        if(sjob[i_sym] != '&' && sjob[i_sym] != ' ')
            bg = 0;
        if(sjob[i_sym] == '&')
            bg = i_sym;
        i_sym += parse_str(&(sjob[i_sym]));
        if(sjob[i_sym] == '|')
            (cur_job)->count_procs ++;
    }
    (cur_job)->procs = (proc *)malloc((cur_job)->count_procs * sizeof(proc));
    (cur_job)->background = (bg != 0);
    sjob[bg] = ' ';
    for(i_sym = c_proc = pred = 0; i_sym < len; ++ i_sym) {
        i_sym += parse_str(&(sjob[i_sym]));
        if(sjob[i_sym] == '|') {
            sjob[i_sym] = 0;
            parse_proc(&((cur_job)->procs[c_proc]), &sjob[pred], c_proc > 0, 1);
            c_proc ++;
            pred = i_sym + 1;
        }
    }
    parse_proc(&((cur_job)->procs[c_proc ++]), &sjob[pred], (cur_job)->count_procs > 0, 0);
    return 0;
}

size_t parse_str(const char * str) {
    if(str[0] != '\'' && str[0] != '\"')
        return 0;

    if(str[0] == '\'') {
        char * c;
        c = strstr(&str[1], "'");
        if(NULL == c) {
            return 0;
        }
        else {
            return (c - str) * sizeof(char);
        }
    }
    else {
        size_t len;
        int res;
        int ign;
        len = strlen(str);
        for(res = 1, ign = 0; res < len; ++ res) {
            if(str[res] == '\"' && !ign)
                return res;
            if(str[res] == '\\' && !ign)
                ign = 1;
            else
                ign = 0;
            
        }
        return 0;
    }
}

int parse(char * str, job ** jobs, size_t * count) {
    size_t len;
    size_t i_sym;
    size_t c_job;
    size_t pred;
    *count = 1;
    len = strlen(str);
    for(i_sym = 0; i_sym < len; ++ i_sym) {
        if(str[i_sym] == '#') {
            str[i_sym] = 0;
            len = strlen(str);
            break;
        i_sym += parse_str(&(str[i_sym]));
        if(str[i_sym] == ';')
            (*count) ++;
    }
    *jobs = (job *)malloc((*count) * sizeof(job));
    for(i_sym = c_job = pred = 0; i_sym < len; ++ i_sym) {
        }
        i_sym += parse_str(&(str[i_sym]));
        if(str[i_sym] == ';') {
            str[i_sym] = 0;
            parse_job(&((*jobs)[c_job ++]), &str[pred]);
            pred = i_sym + 1;
        }
    }
    parse_job(&((*jobs)[c_job]), &str[pred]);
    
    return 0;
}

