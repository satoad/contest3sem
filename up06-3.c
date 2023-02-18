#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>

char *normalize(const char* path)
{
    int length = strlen(path);
    char *norm_path = calloc(2 * length, sizeof(char));
    char buf[3];
    buf[2] = '\0';

    int i = 0;
    int j = 0;
    while (i <= length - 1) {
        buf[0] = path[i];
        buf[1] = path[i + 1];
        if (((strcmp(buf, "//")) && (strcmp(buf, "/.")) && (strcmp(buf, "./"))
                && (strcmp(buf, "..") || ((path[i - 1] != '/') || path[i + 2] != '/')))) {
            if ((buf[0] == '.') && (norm_path[j - 1] != '.')) {
                norm_path[j++] = '/';
            }
            norm_path[j] = buf[0];
            norm_path[j + 1] = buf[1];
            j += 2;
            i += 2;
            continue;
        }

        if (!strcmp(buf, "//") || !strcmp(buf, "/.") || !strcmp(buf, "./")) {
            i++;
            continue;
        }

        if (!strcmp(buf, "..")) {
            i+= 2;
            int g = j;
            while ((norm_path[g] != '/') && (g != 0)) {
                norm_path[g] = '\0';
                g--;
            }
            j = g;
            norm_path[j] = '\0';
        }
    }

    if (strlen(norm_path) > 1) {
        if (norm_path[j - 1] != '/') {
            norm_path[j++] = '/';
        }
        if (strlen(norm_path) == j)
            norm_path[j] = '\0';
    }
    return norm_path;
}

char *relativize_path(const char *path1, const char *path2) {
    char *norm_path1 = normalize(path1);
    char *norm_path2 = normalize(path2);
    char *result = calloc(strlen(norm_path1) * 3 + strlen(norm_path2), sizeof(char));

    int length1 = strlen(norm_path1);
    int length2 = strlen(norm_path2);

    int i = 0;
    int last_slash = 0;
    int prev_slash = 0;
    while ((norm_path1[i] == norm_path2[i]) && (i < length1) && (i < length2)) {
        if (norm_path1[i] == '/') {
            last_slash = i;
        }

        if ((norm_path1[i] == '/') && (i != length1 - 1)) {
            prev_slash = i;
        }
        i++;
    }

    int g = 0;
    for (int z = last_slash + 1; z < length1 - 1; z++) {
        if (norm_path1[z] == '/') {
            result[g++] = '.';
            result[g++] = '.';
            result[g++] = '/';
        }
    }

    if ((prev_slash == 0) || (length1 < length2)) {
        strcpy(result + g, norm_path2 + prev_slash + 1);
    } else {
        if ((!strcmp(norm_path1, norm_path2))) {
            strcpy(result + g, norm_path2 + prev_slash + 1);
            g = strlen(result);
        }

        strcpy(result + g, norm_path2 + last_slash + 1);
    }

    int length3 = strlen(result);
    if (length3 > 0) {
        if (result[length3 - 1] == '/') {
            result[length3 - 1] = '\0';
        }
    }


    if (result[0] == '\0') {
        result[0] = '.';
    }

    free(norm_path1);
    free(norm_path2);
    return result;
}