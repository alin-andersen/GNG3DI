#include "GNG3DHandler.h"

GNG3DHandler::GNG3DHandler(GNG3DConfig* config)
{
    this->config = config->Clone();
}

GNG3DHandler::~GNG3DHandler()
{
    if(config != NULL) delete config;
    if(imesh  != NULL) delete imesh;
    if(smesh  != NULL) delete smesh;
}

void GNG3DHandler::SetConfig(GNG3DConfig* config)
{
    mutex.lock();

    if(this->config != NULL) delete this->config;
    this->config = config->Clone();

    mutex.unlock();
}

bool GNG3DHandler::Load(QString* path)
{
    // components that we can exclude
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                                aiComponent_NORMALS                 |
                                aiComponent_CAMERAS                 |
                                aiComponent_LIGHTS                  |
                                aiComponent_TEXCOORDS               |
                                aiComponent_COLORS                  |
                                aiComponent_BONEWEIGHTS             |
                                aiComponent_TANGENTS_AND_BITANGENTS |
                                aiComponent_ANIMATIONS              |
                                aiComponent_TEXTURES                |
                                aiComponent_MATERIALS);

    scene = importer.ReadFile(path->toUtf8().constData(),
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FindInvalidData       |
            aiProcess_RemoveComponent       |
            aiProcess_OptimizeGraph         |
            aiProcess_OptimizeMeshes);
    if(!scene) return false;

    srand(config->seed);
    qDebug() << config->seed << " seed ";

    mutex.lock();

    if(imesh != NULL) delete imesh;
    imesh = new GNG3DInputMesh(scene->mMeshes[0]);

    if(smesh != NULL) delete smesh;
    smesh = new GNG3DSimplifiedMesh(scene->mMeshes[0]);

    mutex.unlock();

    return true;
}

// execute a certain number of steps
int GNG3DHandler::Step(int s, GNG3DResult* result)
{
    if(s < 0)         return 0;
    if(smesh == NULL) return 0;
    if(imesh == NULL) return 0;

    mutex.lock();

    int steps = 0;
    for(int _s = 0 ; _s < s; _s++)
    {
        for(int _lambda_counter = 0; _lambda_counter < config->lambda; _lambda_counter++)
        {
            // (1) generate input signal

            int r = ((double)rand()*(double)imesh->vertices_count-1.0)/((double)RAND_MAX);

            struct Vec3* signal = &imesh->vertices[r];
            result->signal = r;

            // (2) find nearest and second nearest node

            smesh->SetSignal(signal);
            smesh->FindWinners();

            // (3) increment age of all edges emanating from winner node,
            // if age of edge reached alpha_max, mark it in order to be removed
            // later

            smesh->IncrementWinnerEdgeAges();

            // (4) increment activation counter and add error to winner node

            smesh->IncrementWinnerActivationAndAddError();

            // (5) move winner and winner2 nodes

            smesh->MoveNodes(config->epsilon_b, config->epsilon_n);

            // (6) check connection between winner and winner2

            smesh->EdgeResetOrCreate();

            // (7) remove old edges and nodes

            smesh->RemoveOldEdgesAndNodes(config->alpha_max);

            // (8) decrease error of all nodes

            smesh->MultiplyNodeErrorCounters(config->d);

            // increment mu_counter
            mu_counter++;

            // (10)
            smesh->ResetNodeActivationCounters(config->mu, &mu_counter);
        }

        // (9) create new node or delete node

        smesh->CreateOrDeleteNode((int)(imesh->vertices_count*((float)config->N/100.0f)), config->alpha);

        steps++;
    }

    smesh->UpdateVertices();
    smesh->UpdateEdges();
    smesh->UpdateResult(result);

    mutex.unlock();

    return steps;
}

int GNG3DHandler::StepsUntilFinished(GNG3DResult* result, int N_goal)
{
    int step_counter =   0;
    int steps        =   1;
    int bounds       =   1;

    for(int i = 0; i < 100; i++)
    {
        mutex.lock();
        config->N = N_goal;
        if(config->N < 3) config->N = 3;

        int _N = ((float) this->smesh->vertices_count / (float) this->imesh->vertices_count) * 100.0f;
        if(N_goal == _N)
        {
            mutex.unlock();
            break;
        }
        else mutex.unlock();

        step_counter += Step(steps, result);
    }

    return step_counter;
}

bool GNG3DHandler::Save(QString* path, QString* extension)
{
    mutex.lock();

    // label all vertices of input mesh with representative node

    for(int i = 0; i < imesh->vertices_count; i++)
    {
        struct GNG3DNode* _node = NULL;
        float distance          = FLT_MAX;

        GNG3DLIST_FOREACH_BEG(&smesh->nodes);
        {
            struct GNG3DNode* node = GNG3DLIST_FOREACH_ITEM(struct GNG3DNode, list);

            struct Vec3 tmp;
            Vec3_sub(&tmp, &node->pos, &imesh->vertices[i]);
            float distance2 = Vec3_len(&tmp);
            if(distance > distance2)
            {
                _node = node;
                distance = distance2;
            }
        }
        GNG3DLIST_FOREACH_END;

        imesh->mesh->mVertices[i].x = _node->pos.x;
        imesh->mesh->mVertices[i].y = _node->pos.y;
        imesh->mesh->mVertices[i].z = _node->pos.z;
    }

    aiReturn ret = exporter.Export(scene, extension->toLatin1().data(), path->toLatin1().data(),
                                   aiProcess_GenSmoothNormals      |
                                   //aiProcess_JoinIdenticalVertices |
                                   aiProcess_Triangulate           |
                                   aiProcess_FixInfacingNormals    |
                                   aiProcess_FindDegenerates       |
                                   aiProcess_OptimizeMeshes        |
                                   aiProcess_OptimizeGraph);
    if(ret == -1)
    {
        mutex.unlock();
        return false;
    }

    mutex.unlock();
    return true;
}

bool GNG3DHandler::Swap(QString* path)
{
    // components that we can exclude
    importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
                                aiComponent_NORMALS                 |
                                aiComponent_CAMERAS                 |
                                aiComponent_LIGHTS                  |
                                aiComponent_TEXCOORDS               |
                                aiComponent_COLORS                  |
                                aiComponent_BONEWEIGHTS             |
                                aiComponent_TANGENTS_AND_BITANGENTS |
                                aiComponent_ANIMATIONS              |
                                aiComponent_TEXTURES                |
                                aiComponent_MATERIALS);

    scene = importer.ReadFile(path->toUtf8().constData(),
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FindInvalidData       |
            aiProcess_RemoveComponent       |
            aiProcess_OptimizeGraph         |
            aiProcess_OptimizeMeshes);
    if(!scene) return false;

    srand(config->seed);

    mutex.lock();

    if(imesh != NULL) delete imesh;
    imesh = new GNG3DInputMesh(scene->mMeshes[0]);

    mutex.unlock();

    return true;
}
