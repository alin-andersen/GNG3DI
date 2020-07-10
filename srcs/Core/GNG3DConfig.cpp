#include "GNG3DConfig.h"

GNG3DConfig::GNG3DConfig()
{
    no_winner_color_node.setNamedColor(QString("#ff0000"));
    signal_color_node.setNamedColor(QString("#ffaa00"));
    winner_color_node.setNamedColor(QString("#80ff80"));
    winner2_color_node.setNamedColor(QString("#ffff80"));
    line_color_edge.setNamedColor(QString("#00ff00"));
}

GNG3DConfig* GNG3DConfig::Clone(void)
{
    GNG3DConfig* config = new GNG3DConfig();

    // general

    config->seed = seed;

    // gng3d

    config->N = N;
    config->epsilon_b = epsilon_b;
    config->epsilon_n = epsilon_n;
    config->alpha_max = alpha_max;
    config->d = d;
    config->alpha = alpha;
    config->lambda = lambda;
    config->mu = mu;

    // rendering

    config->separate_camera_control = separate_camera_control;
    config->skip_rendering = skip_rendering;
    config->dynamic_lod = dynamic_lod;
    config->age_rendering = age_rendering;

    // rendering nodes

    config->point_size_node = point_size_node;
    config->no_winner_color_node = no_winner_color_node;
    config->signal_color_node = signal_color_node;
    config->winner_color_node = winner_color_node;
    config->winner2_color_node = winner2_color_node;

    // rendering edges

    config->line_size_edge = line_size_edge;
    config->line_color_edge = line_color_edge;

    // dynamic lod

    config->min_detail = min_detail;
    config->min_detail_distance = min_detail_distance;
    config->max_detail = max_detail;
    config->max_detail_distance = max_detail_distance;

    return config;
}
