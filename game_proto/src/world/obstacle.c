#include "../world/obstacle.h"
#include "stdlib.h"

extern Texture2D kuruminhaTexture;
extern Texture2D reiTexture;

void InitObstacles(Obstacle **obstacles, int *count) {
    *count = 20;   // aumentei a quantidade
    *obstacles = (Obstacle*)malloc(sizeof(Obstacle) * (*count));
    
    // Objetos originais
    (*obstacles)[0] = (Obstacle){{ 16.0f, 16.0f,  32.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[1] = (Obstacle){{-16.0f, 16.0f,  32.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[2] = (Obstacle){{-16.0f, 16.0f, -32.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[3] = (Obstacle){{ 16.0f, 16.0f, -32.0f}, {16.0f, 32.0f, 16.0f}};

    // Colunas flutuantes
    (*obstacles)[4] = (Obstacle){{ 0.0f, 40.0f,  0.0f}, {4.0f, 40.0f, 4.0f}};
    (*obstacles)[5] = (Obstacle){{ 25.0f, 50.0f, -10.0f}, {3.0f, 50.0f, 3.0f}};
    
    // Anel circular de objetos
    (*obstacles)[6] = (Obstacle){{ 30.0f, 10.0f,  0.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[7] = (Obstacle){{ 21.0f, 10.0f, 21.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[8] = (Obstacle){{  0.0f, 10.0f, 30.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[9] = (Obstacle){{-21.0f, 10.0f, 21.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[10] = (Obstacle){{-30.0f, 10.0f,  0.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[11] = (Obstacle){{-21.0f, 10.0f, -21.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[12] = (Obstacle){{  0.0f, 10.0f, -30.0f}, {8.0f, 8.0f, 8.0f}};
    (*obstacles)[13] = (Obstacle){{ 21.0f, 10.0f, -21.0f}, {8.0f, 8.0f, 8.0f}};

    // Torres altas
    (*obstacles)[14] = (Obstacle){{ 40.0f, 60.0f,  40.0f}, {6.0f, 60.0f, 6.0f}};
    (*obstacles)[15] = (Obstacle){{-40.0f, 60.0f, -40.0f}, {6.0f, 60.0f, 6.0f}};

    // Objetos minúsculos espalhados
    (*obstacles)[16] = (Obstacle){{10.0f, 5.0f, -5.0f}, {2.0f, 2.0f, 2.0f}};
    (*obstacles)[17] = (Obstacle){{-12.0f, 3.0f, 18.0f}, {1.5f, 1.5f, 1.5f}};
    (*obstacles)[18] = (Obstacle){{4.0f, 12.0f, 7.0f}, {2.0f, 2.0f, 2.0f}};

    // Um bloco gigante surreal
    (*obstacles)[19] = (Obstacle){{0.0f, 80.0f, 0.0f}, {50.0f, 5.0f, 50.0f}};
}

void FreeObstacles(Obstacle *obstacles) {
    free(obstacles);
}

Mesh cubeMesh;
Model texturedCube;
Model reiCube;

void InitResources() {
    cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);    // cubo unitário
    texturedCube = LoadModelFromMesh(cubeMesh);
    reiCube = LoadModelFromMesh(cubeMesh);

    // aplica textura ao material
    texturedCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = kuruminhaTexture;
    reiCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = reiTexture;

}

void DrawObstacle(Obstacle *obstacle) {

    Vector3 pos = obstacle->position;
    Vector3 size = obstacle->size;

    // desenha o cubo texturizado
    DrawModelEx(
        texturedCube,
        pos,                 // posição
        (Vector3){1,0,0},    // eixo de rotação
        0.0f,                // ângulo
        size,                // escala para o tamanho desejado
        WHITE
    );

    // opcional: esqueleto
    DrawCubeWires(pos, size.x, size.y, size.z, DARKBLUE);
}

