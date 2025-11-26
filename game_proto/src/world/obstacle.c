#include "../world/obstacle.h"
#include "stdlib.h"

extern Texture2D kuruminhaTexture;
extern Texture2D reiTexture;

void InitObstacles(Obstacle **obstacles, int *count) {
    *count = 4;
    *obstacles = (Obstacle*)malloc(sizeof(Obstacle) * (*count));
    
    (*obstacles)[0] = (Obstacle){{ 16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[1] = (Obstacle){{-16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[2] = (Obstacle){{-16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}};
    (*obstacles)[3] = (Obstacle){{ 16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}};
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

