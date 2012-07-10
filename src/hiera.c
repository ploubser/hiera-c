#include <jansson.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hiera.h"

/*******************************************************************
* FUNCTION NAME: lookup_api                                        *
*******************************************************************/

char * lookup_api(char *key, char *resolution_type, char *scope_string,
                  char *config_file_path)
{
  scope_t scope;
  hiera_config_t config;
  config_from_file(config_file_path, &config);
  scope_from_string(scope_string, &scope);

  lookup_response_t lookup_response;
  lookup(key, config.hierarchy, resolution_type, &scope, &lookup_response);

  json_t *response;
  response = json_pack("{s:o, s:s, s:o}", "sources", lookup_response.sources,
      "resolution_type", resolution_type, key, lookup_response.answer);

  return json_dumps(response, JSON_INDENT(2));
}

/*******************************************************************
* FUNCTION NAME: lookup                                            *
*******************************************************************/

void lookup(const char *key, json_t *hierarchy, char *resolution_type,
            scope_t *scope, lookup_response_t *lookup_response)
{
    json_t *answer     = json_null();
    json_t *new_answer = json_array();      
    json_t *sources    = json_array();

    for (int i = 0; i <= json_array_size(hierarchy) - 1; i++) {
        const char *source     = json_string_value(json_array_get(hierarchy, i));
        const char *new_source = parse_string(source, scope->scope);

        if (new_source == NULL)
            continue;

        lookup_key(key, new_source, &answer);

        if (answer == NULL)
            continue;

        if (strncmp(resolution_type, "array", 5) == 0) {
            json_array_append(sources, json_string(new_source));
            json_array_append(new_answer, answer);
        } 
        else {
            json_array_append(sources, json_string(new_source));
            new_answer = answer;
            break;
        }
    }

    lookup_response->sources = sources;
    lookup_response->answer  = new_answer;
}

/*******************************************************************
* FUNCTION NAME: lookup_key                                        *
*******************************************************************/

void lookup_key(const char *key, const char *source, json_t **answer)
{
    char source_file[500];
    snprintf(source_file, sizeof source_file, "%s%s%s", "/tmp/", source, ".json"); 

    printf("Searching for %s\n", source_file);

    json_error_t errors;
    const json_t *data = json_load_file(source_file, 0, &errors); 
    *answer = json_object_get(data, key);
}

/*******************************************************************
* FUNCTION NAME: scope_from_file                                   *
*******************************************************************/

void scope_from_file(char *scope_file_path, scope_t *scope)
{
    printf("Loading scope: %s\n", scope_file_path);

    json_error_t errors;
    scope->scope = json_load_file(scope_file_path, 0, &errors);
}

/*******************************************************************
* FUNCTION NAME: scope_from_string                                 *
*******************************************************************/

void scope_from_string(char *scope_string, scope_t *scope)
{
    printf("Loading scope from string\n");

    json_error_t errors;
    scope->scope = json_loads(scope_string, 0, &errors);
}

/*******************************************************************
* FUNCTION NAME: config_from_file                                  *
*******************************************************************/

void config_from_file(char *config_file_path, hiera_config_t *config)
{
    printf("Searching for Hiera config file: %s\n", config_file_path);

    if (file_exist(config_file_path) == 0)
        printf("Found %s\n", config_file_path);
    else
        printf("Cannot locate %s\n", config_file_path);

    json_error_t errors;
    const json_t *data     = json_load_file(config_file_path, 0, &errors);
    config->hierarchy      = json_object_get(data, "hierarchy");
    config->data_directory = json_object_get(data, "datadir");
}

/*******************************************************************
* FUNCTION NAME: file_exist                                        *
*******************************************************************/

int file_exist(const char *path)
{
    return access(path, F_OK);
}

/******************************************************************
* FUNCTION NAME: parse_string                                     *
******************************************************************/ 

const char * parse_string(const char *string, const json_t *scope)
{
    regex_t     p_regex;
    const char  *pattern = "^%{(.*)}";
    regmatch_t  matches[2];
    size_t      number_of_matches = 2;
    int         rcomp_err, rexec_err;

    if (0 != (rcomp_err = regcomp(&p_regex, pattern, REG_EXTENDED)))
        printf("Cant compile: %d", rcomp_err);

    if (0 != (rexec_err = regexec(&p_regex, string, number_of_matches, matches, 0)))
        return string;

    char * submatch;
    size_t matchlen = matches[1].rm_eo - matches[1].rm_so;
    submatch = malloc(matchlen+1);

    strncpy(submatch, string + matches[1].rm_so, matchlen);
    submatch[matchlen] = '\0';

    regfree(&p_regex);
    const json_t *new_source; 
  
    if ((new_source=json_object_get(scope, submatch)) != NULL)
        return json_string_value(new_source);
    else
        return NULL;
}
