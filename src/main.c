#include <stdio.h>
#include "../include/parser.h"

int main(int argc, char* argv[]){
        
    if (!(argc == 2)){
        fprintf(stderr, "Usage: %s <file.yaml>\n", argv[0]);
        return 1;
    }

    YamlMap map = {0};
    if (!parse(argv[1], &map)) {
        fprintf(stderr, "Parse failed\n");
        return 1;
    }

    for (int i = 0; i < map.count; i++)
        printf("%s = %s\n", map.entries[i].key, map.entries[i].value);

    return 0;
}