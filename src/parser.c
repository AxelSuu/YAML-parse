#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../include/parser.h"

int parse(const char *filepath, YamlMap *out) {
    if (filepath == NULL || out == NULL) {
        fprintf(stderr, "Invalid arguments to parse()\n");
        return 0;
    }

    FILE *input = fopen(filepath, "rb");
    if (input == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return 0;
    }

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize YAML parser\n");
        fclose(input);
        return 0;
    }

    yaml_parser_set_input_file(&parser, input);

    int done = 0;
    int expecting_key = 0;
    char pending_key[MAX_KEY_LEN] = {0};

    while (!done) {
        if (!yaml_parser_parse(&parser, &event))
            goto error;

        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                expecting_key = 1;
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
                    } else {
                        fprintf(stderr, "Warning: MAX_ENTRIES (%d) reached, entry \"%s\" dropped\n",
                                MAX_ENTRIES, pending_key);
                    }
                    expecting_key = 1;
                }
                break;
            }
            default:
                break;
        }

        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);
    fclose(input);
    return 1;

error:
    yaml_parser_delete(&parser);
    fclose(input);
    return 0;
}

const char *yaml_get(const YamlMap *map, const char *key) {
    for (int i = 0; i < map->count; i++) {
        if (strcmp(map->entries[i].key, key) == 0)
            return map->entries[i].value;
    }
    return NULL;
}
