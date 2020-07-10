#ifndef GNG3DCONFIG_H
#define GNG3DCONFIG_H

#include <QString>
#include <QColor>

class GNG3DConfig
{

public:

    GNG3DConfig();
    GNG3DConfig* Clone();

    // general

    unsigned int seed = 0;

    // gng3d

    int N            = 50;   // percentage size
    float epsilon_b  = 0.6;  // fraction for winner
    float epsilon_n  = 0.01; // fraction for winner 2
    int alpha_max    = 30;   // max age for edges
    float d          = 0.2;  // multiplied with each neuron error each step to increase error
    float alpha      = 0.8;  // multiplied to decrease neuron error
    int lambda       = 10;   // least amount of iterations before vertices can be created or destroyed
    int mu           = 30;

    // rendering
    bool separate_camera_control   = false;
    bool skip_rendering            = false;
    bool dynamic_lod               = false;
    bool age_rendering             = false;

    // rendering nodes
    float point_size_node = 2.5f;
    QColor no_winner_color_node;
    QColor signal_color_node;
    QColor winner_color_node;
    QColor winner2_color_node;

    // rendering lines
    float line_size_edge = 4.0f;
    QColor line_color_edge;

    // dynamic lod
    float min_detail          =  0.1f;
    float min_detail_distance = 15.0f;
    float max_detail          =  0.6f;
    float max_detail_distance =  5.0f;
};

#endif
