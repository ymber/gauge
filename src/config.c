#include <stdlib.h>
#include <stdio.h>

#include <cairo.h>
#include <cjson/cJSON.h>

#include "config.h"
#include "modules/status.h"
#include "modules/background.h"

int readfile(char *file_path, char **file_string)
{
    int length;
    int error = 0;
    FILE *fp = fopen(file_path, "r");

    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        *file_string = malloc(length + 1);
        if (*file_string)
        {
            fseek(fp, 0, SEEK_SET);
            // only supports 4 byte encodings
            fread(*file_string, 4, length, fp);
            (*file_string)[length] = '\0';
        }
        else
        {
            error = 1;
        }
        fclose(fp);
    }
    else
    {
        error = 1;
    }

    return error;
}

int check_error(cJSON *json)
{
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return 1;
    }
    return 0;
}

struct config settings;

int read_config(char *config_file)
{
    char *file_string = NULL;
    if (readfile(config_file, &file_string))
    {
        return 1;
    }
    cJSON *root = cJSON_Parse(file_string);
    free(file_string);
    if (check_error(root))
    {
        return 1;
    }

    cJSON *config_json = cJSON_GetObjectItem(root, "config");
    if (check_error(config_json))
    {
        return 1;
    }

    cJSON *size_x = cJSON_GetObjectItem(config_json, "size_x");
    if (!check_error(size_x))
    {
        settings.size_x = size_x->valueint;
    }
    cJSON *size_y = cJSON_GetObjectItem(config_json, "size_y");
    if (!check_error(size_y))
    {
        settings.size_y = size_y->valueint;
    }
    cJSON *daemon_freq = cJSON_GetObjectItem(config_json, "daemon_freq");
    if (!check_error(daemon_freq))
    {
        settings.daemon_freq.tv_sec = daemon_freq->valueint / 1000;
        settings.daemon_freq.tv_nsec = daemon_freq->valueint % 1000;
    }
    cJSON *display_freq = cJSON_GetObjectItem(config_json, "display_freq");
    if (!check_error(display_freq))
    {
        settings.display_freq.tv_sec = display_freq->valueint / 1000;
        settings.display_freq.tv_nsec = display_freq->valueint % 1000;
    }

    cJSON_Delete(root);
    return 0;
}

void (*daemon_fncs[1])() = {
    update_status};

void (*display_fncs[1])(cairo_t *) = {
    draw_background};
