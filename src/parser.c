#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../include/parser.h"

static void build_dotted_key(
        const char path_stack[][MAX_KEY_LEN], int depth,
        const char *pending_key, char *out_key)
{
    int pos = 0;
    for (int i = 0; i < depth && pos < MAX_KEY_LEN - 1; i++) {
        int w = snprintf(out_key + pos, (size_t)(MAX_KEY_LEN - 1 - pos), "%s.", path_stack[i]);
        if (w < 0 || (pos += w) >= MAX_KEY_LEN - 1) { pos = MAX_KEY_LEN - 1; break; }
    }
    if (pos < MAX_KEY_LEN - 1) {
        int w = snprintf(out_key + pos, (size_t)(MAX_KEY_LEN - 1 - pos), "%s", pending_key);
        if (w > 0) pos += w;
    }
    out_key[pos < MAX_KEY_LEN ? pos : MAX_KEY_LEN - 1] = '\0';
}

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
    char path_stack[MAX_DEPTH][MAX_KEY_LEN];
    int  depth = 0;

    while (!done) {
        if (!yaml_parser_parse(&parser, &event))
            goto error;

        switch (event.type) {
            case YAML_MAPPING_START_EVENT:
                if (!expecting_key && pending_key[0] != '\0') {
                    if (depth < MAX_DEPTH) {
                        strncpy(path_stack[depth], pending_key, MAX_KEY_LEN - 1);
                        path_stack[depth][MAX_KEY_LEN - 1] = '\0';
                        depth++;
                    } else {
                        fprintf(stderr, "Warning: max nesting depth (%d) exceeded under \"%s\"\n",
                                MAX_DEPTH, pending_key);
                    }
                    pending_key[0] = '\0';
                }
                expecting_key = 1;
                break;
            case YAML_MAPPING_END_EVENT:
                if (depth > 0) depth--;
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
                        char full_key[MAX_KEY_LEN] = {0};
                        build_dotted_key(path_stack, depth, pending_key, full_key);
                        strncpy(out->entries[out->count].key, full_key, MAX_KEY_LEN - 1);
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
