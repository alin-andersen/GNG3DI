#ifndef GNG3DHANDLER_H
#define GNG3DHANDLER_H

#include <stdlib.h>
#include <float.h>

#include <iostream>
#include <fstream>
#include <string>

#include <QString>
#include <QDebug>
#include <QRunnable>
#include <QThreadPool>
#include <QThread>
#include <QMutex>
#include <QFile>
#include <QTimer>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

#include <glm/vec4.hpp>

#include "Core/GNG3DInputMesh.h"
#include "Core/GNG3DSimplifiedMesh.h"
#include "Core/GNG3DConfig.h"
#include "Core/GNG3DResult.h"

class GNG3DHandler
{

private:

    GNG3DConfig* config;
    QMutex mutex;
    int mu_counter = 0;

public:

    Assimp::Importer importer;
    Assimp::Exporter exporter;
    aiScene* scene = NULL;

    GNG3DInputMesh*      imesh = NULL;
    GNG3DSimplifiedMesh* smesh = NULL;

    GNG3DHandler(GNG3DConfig* config);
    ~GNG3DHandler();

    void SetConfig(GNG3DConfig* config);

    bool Load(QString* path);
    int  Step(int s, GNG3DResult* result);
    int  StepsUntilFinished(GNG3DResult* result, int N_goal);
    bool Save(QString* path, QString* extension);
    bool Swap(QString* path);

private slots:

    void SlotRun(void);

};

#endif
