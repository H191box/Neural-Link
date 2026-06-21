#include "flags.h"

typedef struct {
    char name[FLAG_NAME_LEN];
    bool value;
} Flag;

static Flag flags[MAX_FLAGS];
static int flag_count = 0;

/* Simple string hash for flag lookup */
static u16 flag_hash(const char *name) {
    u16 h = 0;
    int i = 0;
    while (name[i] != '\0') {
        h = h * 31 + (u16)name[i];
        i++;
    }
    return h;
}

void flags_init(void) {
    int i;
    for (i = 0; i < MAX_FLAGS; i++) {
        flags[i].name[0] = '\0';
        flags[i].value = FALSE;
    }
    flag_count = 0;
}

int flag_find(const char *name) {
    int i;
    for (i = 0; i < flag_count; i++) {
        if (my_strcmp(flags[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void flag_set(const char *name, bool value) {
    int idx = flag_find(name);
    if (idx >= 0) {
        flags[idx].value = value;
    } else if (flag_count < MAX_FLAGS) {
        int i;
        for (i = 0; i < FLAG_NAME_LEN - 1 && name[i] != '\0'; i++) {
            flags[flag_count].name[i] = name[i];
        }
        flags[flag_count].name[i] = '\0';
        flags[flag_count].value = value;
        flag_count++;
    }
}

bool flag_get(const char *name) {
    int idx = flag_find(name);
    if (idx >= 0) return flags[idx].value;
    return FALSE;
}

bool flag_exists(const char *name) {
    return flag_find(name) >= 0;
}

void flags_clear(void) {
    flags_init();
}

void flag_set_index(int index, bool value) {
    if (index >= 0 && index < flag_count) {
        flags[index].value = value;
    }
}

bool flag_get_index(int index) {
    if (index >= 0 && index < flag_count) {
        return flags[index].value;
    }
    return FALSE;
}