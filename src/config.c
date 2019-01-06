#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cairo.h>
#include <cjson/cJSON.h>

#include "config.h"
#include "modules/status.h"
#include "modules/display.h"
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

int get_check_string(char **dest, cJSON *json)
{
    if (check_error(json))
    {
        return 1;
    }
    *dest = malloc(strlen(json->valuestring) + 1);
    if (*dest == NULL)
    {
        fprintf(stderr, "error allocating memory for data string\n");
        return 1;
    }
    strcpy(*dest, json->valuestring);
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

void destroy_data_arc(struct drawable *arc)
{
    if (arc->type != data_arc)
    {
        fprintf(stderr, "unknown object passed to destroy_data_arc\n");
        return;
    }
    free(arc->data_arc.data);
    free(arc);
}

void destroy_line_chart(struct drawable *chart)
{
    if (chart->type != line_chart)
    {
        fprintf(stderr, "unknown object passed to destroy_line_chart\n");
        return;
    }
    free(chart->line_chart.data);
    free(chart);
}

int read_data_arc(cJSON *item, struct drawable *arc)
{
    if (get_check_string(&arc->data_arc.data, cJSON_GetObjectItem(item, "data")))
    {
        return 1;
    }

    cJSON *max_json = cJSON_GetObjectItem(item, "max");
    if (check_error(max_json))
    {
        return 1;
    }
    arc->data_arc.max = max_json->valueint;

    cJSON *bg_rgba_json = cJSON_GetObjectItem(item, "bg_rgba");
    if (check_error(bg_rgba_json))
    {
        return 1;
    }
    if (cJSON_GetArraySize(bg_rgba_json) != 4)
    {
        return 1;
    }
    arc->data_arc.bg_rgba.r = cJSON_GetArrayItem(bg_rgba_json, 0)->valuedouble;
    arc->data_arc.bg_rgba.g = cJSON_GetArrayItem(bg_rgba_json, 1)->valuedouble;
    arc->data_arc.bg_rgba.b = cJSON_GetArrayItem(bg_rgba_json, 2)->valuedouble;
    arc->data_arc.bg_rgba.a = cJSON_GetArrayItem(bg_rgba_json, 3)->valuedouble;

    cJSON *fg_rgba_json = cJSON_GetObjectItem(item, "fg_rgba");
    if (check_error(fg_rgba_json))
    {
        return 1;
    }
    if (cJSON_GetArraySize(fg_rgba_json) != 4)
    {
        return 1;
    }
    arc->data_arc.fg_rgba.r = cJSON_GetArrayItem(fg_rgba_json, 0)->valuedouble;
    arc->data_arc.fg_rgba.g = cJSON_GetArrayItem(fg_rgba_json, 1)->valuedouble;
    arc->data_arc.fg_rgba.b = cJSON_GetArrayItem(fg_rgba_json, 2)->valuedouble;
    arc->data_arc.fg_rgba.a = cJSON_GetArrayItem(fg_rgba_json, 3)->valuedouble;

    cJSON *thickness_json = cJSON_GetObjectItem(item, "thickness");
    if (check_error(thickness_json))
    {
        return 1;
    }
    arc->data_arc.thickness = thickness_json->valueint;

    cJSON *x_center_json = cJSON_GetObjectItem(item, "x");
    if (check_error(x_center_json))
    {
        return 1;
    }
    arc->data_arc.x = x_center_json->valueint;

    cJSON *y_center_json = cJSON_GetObjectItem(item, "y");
    if (check_error(y_center_json))
    {
        return 1;
    }
    arc->data_arc.y = y_center_json->valueint;

    cJSON *radius_json = cJSON_GetObjectItem(item, "radius");
    if (check_error(radius_json))
    {
        return 1;
    }
    arc->data_arc.radius = radius_json->valueint;

    cJSON *start_angle_json = cJSON_GetObjectItem(item, "start_angle");
    if (check_error(start_angle_json))
    {
        return 1;
    }
    arc->data_arc.start_angle = start_angle_json->valuedouble;

    cJSON *end_angle_json = cJSON_GetObjectItem(item, "end_angle");
    if (check_error(end_angle_json))
    {
        return 1;
    }
    arc->data_arc.end_angle = end_angle_json->valuedouble;

    return 0;
}

int read_line_chart(cJSON *item, struct drawable *chart)
{
    if (get_check_string(&chart->line_chart.data, cJSON_GetObjectItem(item, "data")))
    {
        return 1;
    }

    cJSON *max_json = cJSON_GetObjectItem(item, "max");
    if (check_error(max_json))
    {
        return 1;
    }
    chart->line_chart.max = max_json->valueint;

    cJSON *rgba_json = cJSON_GetObjectItem(item, "rgba");
    if (check_error(rgba_json))
    {
        return 1;
    }
    if (cJSON_GetArraySize(rgba_json) != 4)
    {
        return 1;
    }
    chart->line_chart.rgba.r = cJSON_GetArrayItem(rgba_json, 0)->valuedouble;
    chart->line_chart.rgba.g = cJSON_GetArrayItem(rgba_json, 1)->valuedouble;
    chart->line_chart.rgba.b = cJSON_GetArrayItem(rgba_json, 2)->valuedouble;
    chart->line_chart.rgba.a = cJSON_GetArrayItem(rgba_json, 3)->valuedouble;

    cJSON *thickness_json = cJSON_GetObjectItem(item, "thickness");
    if (check_error(thickness_json))
    {
        return 1;
    }
    chart->line_chart.thickness = thickness_json->valueint;

    cJSON *x_json = cJSON_GetObjectItem(item, "x");
    if (check_error(x_json))
    {
        return 1;
    }
    chart->line_chart.x = x_json->valueint;

    cJSON *y_json = cJSON_GetObjectItem(item, "y");
    if (check_error(y_json))
    {
        return 1;
    }
    chart->line_chart.y = y_json->valueint;

    cJSON *width_json = cJSON_GetObjectItem(item, "width");
    if (check_error(width_json))
    {
        return 1;
    }
    chart->line_chart.width = width_json->valueint;

    cJSON *height_json = cJSON_GetObjectItem(item, "height");
    if (check_error(height_json))
    {
        return 1;
    }
    chart->line_chart.height = height_json->valueint;

    cJSON *data_count_json = cJSON_GetObjectItem(item, "data_count");
    if (check_error(data_count_json))
    {
        return 1;
    }
    chart->line_chart.data_count = data_count_json->valueint;

    return 0;
}

vector_t *drawables;

int read_drawables(char *config_file)
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

    drawables = vector_init();
    cJSON *drawables_json = cJSON_GetObjectItem(root, "drawables");
    if (!check_error(drawables_json))
    {
        int drawable_count = cJSON_GetArraySize(drawables_json);
        for (int i = 0; i < drawable_count; ++i)
        {
            cJSON *item = cJSON_GetArrayItem(drawables_json, i);
            if (!check_error(item))
            {
                cJSON *drawable_type = cJSON_GetObjectItem(item, "type");
                if (!strcmp(drawable_type->valuestring, "arc"))
                {
                    struct drawable *arc = malloc(sizeof(struct drawable));
                    arc->type = data_arc;
                    if (read_data_arc(item, arc))
                    {
                        destroy_data_arc(arc);
                        continue;
                    }

                    vector_push_back(drawables, arc);
                }
                else if (!strcmp(drawable_type->valuestring, "chart"))
                {
                    struct drawable *chart = malloc(sizeof(struct drawable));
                    chart->type = line_chart;
                    if (read_line_chart(item, chart))
                    {
                        destroy_line_chart(chart);
                        continue;
                    }

                    vector_push_back(drawables, chart);
                }
            }
        }
    }

    cJSON_Delete(root);
    return 0;
}

void clear_drawables()
{
    for (unsigned int i = 0; i < drawables->count; ++i)
    {
        if (((struct drawable *)drawables->data[i])->type == data_arc)
        {
            destroy_data_arc(drawables->data[i]);
        }
        else if (((struct drawable *)drawables->data[i])->type == line_chart)
        {
            destroy_line_chart(drawables->data[i]);
        }
    }
    vector_teardown(drawables);
}

void (*daemon_fncs[1])() = {
    update_status};

void (*display_fncs[1])(cairo_t *) = {
    draw_background};
