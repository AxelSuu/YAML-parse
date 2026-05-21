#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../include/parser.h"

int parse(const char *filepath, YamlMap *out){
    yaml_parser_t parser;
    yaml_event_t event;

    int done = 0;
    int expecting_key = 0;
    char pending_key[MAX_KEY_LEN] = {0};

    /* Create the Parser object. */
    yaml_parser_initialize(&parser);

    /* Set a file input. */
    if (filepath == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }
    
    FILE *input = fopen(filepath, "rb");
    if (input == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return 0;
    }

    yaml_parser_set_input_file(&parser, input);

    /* Read the event sequence. */
    while (!done) {

        /* Get the next event. */
        if (!yaml_parser_parse(&parser, &event))
            goto error;

        /*
        ...
        Process the event.
        ...
        */

        //printf("Event: %d\n", event.type);

        switch(event.type) {
            case YAML_STREAM_START_EVENT:
                break;
            case YAML_STREAM_END_EVENT:
                break;
            case YAML_DOCUMENT_START_EVENT:
                break;
            case YAML_DOCUMENT_END_EVENT:
                break;
            case YAML_MAPPING_START_EVENT:
                expecting_key = 1;
                break;
            case YAML_MAPPING_END_EVENT:
                expecting_key = 0;
                break;
            case YAML_SEQUENCE_START_EVENT:
                break;
            case YAML_SEQUENCE_END_EVENT:
                break;
            case YAML_SCALAR_EVENT: {
                const char *scalar = (const char *)event.data.scalar.value;
                if (expecting_key) {
                    strncpy(pending_key, scalar, MAX_KEY_LEN - 1);
                    pending_key[MAX_KEY_LEN - 1] = '\0';
                    expecting_key = 0;
                } else {
                    if (out->count < MAX_ENTRIES) {
                        strncpy(out->entries[out->count].key, pending_key, MAX_KEY_LEN - 1);
                        out->entries[out->count].key[MAX_KEY_LEN - 1] = '\0';
                        strncpy(out->entries[out->count].value, scalar, MAX_VAL_LEN - 1);
                        out->entries[out->count].value[MAX_VAL_LEN - 1] = '\0';
                        out->count++;
                    }
                    expecting_key = 1;
                }
                break;
            }
            default:
                break;
        }

        /* Are we finished? */
        done = (event.type == YAML_STREAM_END_EVENT);

        /* The application is responsible for destroying the event object. */
        yaml_event_delete(&event);

    }

    /* Destroy the Parser object. */
    yaml_parser_delete(&parser);

    return 1;

    /* On error. */
    error:

    /* Destroy the Parser object. */
    yaml_parser_delete(&parser);

    return 0;


}