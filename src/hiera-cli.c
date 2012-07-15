#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <jansson.h>
#include "hiera.h"
#include "log.h"

/*******************************************************************
* FUNCTION NAME: main                                              *
*******************************************************************/

int main(int argc, char *argv[])
{
  cli_options_t cli_options;
  set_cli_options(argc, argv, &cli_options);

  scope_t scope;
  if (cli_options.scope_file_path != 0)
    scope_from_file(cli_options.scope_file_path, &scope);
  else
    scope.scope = json_null();

  hiera_config_t config;
  config_from_file(cli_options.config_file_path, &config);

  lookup_response_t lookup_response;

  if (cli_options.key != NULL)
    lookup(cli_options.key, config.hierarchy, cli_options.resolution_type,
           &scope, &lookup_response);
  else {
    log_console(CONSOLE_LOG_ERR, "%s\n", "Please supply a data item to look up.");
    exit(EXIT_FAILURE);
  }

  json_decref(config.hierarchy);
  json_decref(config.data_directory);
  json_decref(scope.scope);

  if (lookup_response.answer == NULL) {
    printf("NULL\n");
    exit(EXIT_FAILURE);
  }

  json_t *response = json_null();
  switch(json_typeof(lookup_response.answer))
  {
    case JSON_NULL:
      printf("NULL");
      exit(EXIT_FAILURE);
    default:
      response = json_pack("{s:o, s:s, s:o}",
                           "sources", lookup_response.sources,
                           "resolution_type", cli_options.resolution_type,
                           cli_options.key, lookup_response.answer);
      json_incref(lookup_response.answer);
      json_incref(lookup_response.sources);
      json_dumpf(response, stdout, JSON_INDENT(2));
      json_decref(response);
      break;
  }

  json_decref(lookup_response.answer);
  json_decref(lookup_response.sources);
  return 0;
}

/*******************************************************************
* FUNCTION NAME: display_usage                                     *
*******************************************************************/

void display_usage(void)
{
  fputs("Usage: hiera [options] key\n\n", stdout);
  fputs("  --help    Show usage info.\n", stdout);
  fputs("  --array   Array search.\n", stdout);
  fputs("  --config  The Hiera configuration file to use.\n", stdout);
  fputs("  --debug   Show debug output.\n", stdout);
  fputs("  --scope   The scope JSON file to use.\n", stdout);

  exit(EXIT_FAILURE);
}

/*******************************************************************
* FUNCTION NAME: set_cli_options                                   *
*******************************************************************/

void set_cli_options(int argc, char *argv[], cli_options_t *cli_options)
{
  int option;
  static const char *opt_string = "das:c:h";
  static const struct option long_options[] = {
    { "array",  no_argument,       NULL, 'a' },
    { "debug",  no_argument,       NULL, 'd' },
    { "config", optional_argument, NULL, 'c' },
    { "scope",  optional_argument, NULL, 's' },
    { "help",   no_argument,       NULL, 'h' },
    { 0, 0, 0, 0 }
  };

  // Set the default values which are defined in "hiera.h"
  cli_options->config_file_path = DEFAULT_CONFIG_FILE_PATH;
  cli_options->scope_file_path  = DEFAULT_SCOPE_FILE_PATH; 
  cli_options->resolution_type  = DEFAULT_RESOLUTION_TYPE; 

  // Process the cli options
  while ((option = getopt_long(argc, argv, opt_string, long_options, NULL)) != -1)
  {
    switch(option)
    {
      case 'h':
        display_usage();
        break;
      case 's':
        cli_options->scope_file_path = optarg;
        break;
      case 'c':
        cli_options->config_file_path = optarg;
        break;
      case 'a':
        cli_options->resolution_type = "array";
        break;
      case 'd':
        set_log_mask(CONSOLE_LOG_DEBUG);
        break;
    }
  }

  // Set the lookup key
  cli_options->key = argv[optind];
}

