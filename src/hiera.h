#ifndef _HIERAH_
#define _HIERAH_
#include <jansson.h>

#define HIERA_MAX_HIERARCHY      100
#define HIERA_MAX_DATA_DIRECTORY 100
#define HIERA_MAX_SOURCE_NAME    100
#define DEFAULT_SCOPE_FILE_PATH  0
#define DEFAULT_RESOLUTION_TYPE  "priority"
#define DEFAULT_DATA_DIRECTORY   "/var/lib/hiera"
#define DEFAULT_CONFIG_FILE_PATH "/etc/hiera.json"

typedef struct cli_options {
    char *config_file_path;
    char *scope_file_path;
    char *resolution_type;
    char *key;
} cli_options_t;

typedef struct hiera_config {
    json_t *hierarchy;
    json_t *data_directory;
} hiera_config_t;

typedef struct lookup_response {
    json_t *sources;
    json_t *answer;
} lookup_response_t;

typedef struct scope {
    json_t *scope;
} scope_t;

// Function Prototypes
void         display_usage     (void);
void         config_from_file  (char *config_file_path, hiera_config_t *config);
void         scope_from_file   (char *scope_file_path, scope_t *scope);
void         scope_from_string (char *scope_string, scope_t *scope);
void         set_cli_options   (int argc, char *argv[], cli_options_t *cli_options);
int          file_exist        (const char *path);
const char * parse_string      (const char *string, const json_t *scope);
void         lookup_key        (const char *key, const char *source, json_t **answer);
void         lookup            (const char *key, json_t *hierarchy, char *resolution_type,
                                scope_t *scope, lookup_response_t *lookup_response);
char *       lookup_api        (char *key, char *resolution_type, char *scope_string,
                                char *config_file_path);
#endif
