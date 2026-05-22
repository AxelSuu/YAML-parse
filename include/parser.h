#ifndef PARSER_H
#define PARSER_H

#define MAX_ENTRIES 64
#define MAX_KEY_LEN 256
#define MAX_VAL_LEN 256

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
} YamlEntry;

typedef struct {
    YamlEntry entries[MAX_ENTRIES];
    int count;
} YamlMap;


int parse(const char *filepath, YamlMap *out);
const char *yaml_get(const YamlMap *map, const char *key);

#endif /* PARSER_H */