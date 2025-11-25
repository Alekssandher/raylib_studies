#include "raylib.h"
#include "raymath.h"

#define LOGS

#define MAX_COLUMNS 20

#define GRAVITY         32.0f
#define NORMAL_SPEED       4.0f
#define CROUCH_SPEED     2.0f
#define SPRINT_MULT     2.1f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      150.0f

// Grounded drag
#define FRICTION         0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG         0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL         15.0f
#define CROUCH_HEIGHT    0.0f
#define STAND_HEIGHT     1.0f
#define BOTTOM_HEIGHT    0.5f
#define LEAN_FORCE 4.f

#define NORMALIZE_INPUT  0

#ifdef LOGS
#include "stdio.h"
#endif

typedef struct {
    Vector3 position;
    Vector3 size;
    Vector3 velocity;
    Vector3 dir;
    bool isGrounded;
    bool isColliding;
} Body;

static Vector2 sensitivity = { 0.001f, 0.001f };

static Body player = { 0, .size = { 1.0f, 2.0f, 1.0f } };

static Vector2 lookRotation = { 0 };
static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = STAND_HEIGHT;
static Vector2 lean = { 0 };

static void DrawLevel(void);
static void UpdateCameraFPS(Camera *camera);
static void UpdateBody(Body *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold);

BoundingBox GetBoudingBox(Vector3 pos, Vector3 size) {
    return (BoundingBox){
        .min = (Vector3){ pos.x - size.x/2,
                          pos.y - size.y/2,
                          pos.z - size.z/2 },
        .max = (Vector3){ pos.x + size.x/2,
                          pos.y + size.y/2,
                          pos.z + size.z/2 }
    };
}

typedef struct {
    Vector3 position;
    Vector3 size;
} Obstacle;

// Towers
static Obstacle obstacles[] = {
    {{ 16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}},
    {{-16.0f, 16.0f,  16.0f}, {16.0f, 32.0f, 16.0f}},
    {{-16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}},
    {{ 16.0f, 16.0f, -16.0f}, {16.0f, 32.0f, 16.0f}},
};

static const int numObstacles = sizeof(obstacles)/sizeof(obstacles[0]);



int main(void)
{
    

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera fps");

    Camera camera = { 0 };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.position = (Vector3){
        player.position.x,
        player.position.y + (BOTTOM_HEIGHT + headLerp),
        player.position.z,
    };

    UpdateCameraFPS(&camera);

    DisableCursor(); 
    
    SetTargetFPS(60); 

    Vector3 boxPos = { 5.0f, 1.0f, 0.0f };
    Vector3 boxSize = { 2.0f, 2.0f, 2.0f };

    while(!WindowShouldClose())
    {
        Vector2 mouseDelta = GetMouseDelta();
        lookRotation.x -= mouseDelta.x*sensitivity.x;
        lookRotation.y += mouseDelta.y*sensitivity.y;

        char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
        bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
        UpdateBody(&player, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching);

        float delta = GetFrameTime();
        headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f*delta);
        camera.position = (Vector3){
            player.position.x,
            player.position.y + (BOTTOM_HEIGHT + headLerp),
            player.position.z,
        };

        if (player.isGrounded && ((forward != 0) || (sideway != 0)))
        {
            headTimer += delta*3.0f;
            walkLerp = Lerp(walkLerp, 1.0f, 4.0f*delta);
            camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f*delta);
        }
        else
        {
            walkLerp = Lerp(walkLerp, 0.0f, 4.0f*delta);
            camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f*delta);
        }

        lean.x = Lerp(lean.x, sideway*0.02f, LEAN_FORCE*delta);
        lean.y = Lerp(lean.y, forward*0.015f, LEAN_FORCE*delta);

        

        UpdateCameraFPS(&camera);
        BeginDrawing();

            
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                if(
                CheckCollisionBoxes(
                    GetBoudingBox(player.position, player.size),
                    GetBoudingBox(boxPos, boxSize)
                ) 
                ) {
                    DrawCube(boxPos, boxSize.x, boxSize.y, boxSize.z, RED);

                    DrawCubeWires(boxPos, boxSize.x, boxSize.y, boxSize.z, BLACK);

                    player.isColliding = true;
                }
                else {
                    DrawCube(boxPos, boxSize.x, boxSize.y, boxSize.z, GREEN);

                    DrawCubeWires(boxPos, boxSize.x, boxSize.y, boxSize.z, DARKGREEN);

                    player.isColliding = false;
                    
                }
                DrawLevel();
            EndMode3D();

            // Draw info box
            DrawRectangle(5, 5, 330, 75, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 5, 330, 75, BLUE);

            DrawText("Camera controls:", 15, 15, 10, BLACK);
            DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length((Vector2){ player.velocity.x, player.velocity.z })), 15, 60, 10, BLACK);

        EndDrawing();
    }
    CloseWindow();


    return 0;
}

void ResolveCollision(Body *player, Obstacle *box) {
    BoundingBox pBox = GetBoudingBox(player->position, player->size);
    BoundingBox bBox = (BoundingBox){box->position, box->position};
    bBox.min.x -= box->size.x/2; bBox.min.y -= box->size.y/2; bBox.min.z -= box->size.z/2;
    bBox.max.x += box->size.x/2; bBox.max.y += box->size.y/2; bBox.max.z += box->size.z/2;

    if (!CheckCollisionBoxes(pBox, bBox)) return;

    float px1 = bBox.max.x - pBox.min.x; 
    float px2 = pBox.max.x - bBox.min.x; 
    float py1 = bBox.max.y - pBox.min.y;
    float py2 = pBox.max.y - bBox.min.y;
    float pz1 = bBox.max.z - pBox.min.z;
    float pz2 = pBox.max.z - bBox.min.z;

    float minX = (px1 < px2) ? px1 : -px2;
    float minY = (py1 < py2) ? py1 : -py2;
    float minZ = (pz1 < pz2) ? pz1 : -pz2;

    float absX = fabsf(minX), absY = fabsf(minY), absZ = fabsf(minZ);
    if (absX < absY && absX < absZ) player->position.x += minX;
    else if (absY < absX && absY < absZ) {
        player->position.y += minY;
        player->velocity.y = 0;
        player->isGrounded = (minY > 0);
    }
    else player->position.z += minZ;
}

void UpdateBody(Body *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold)
{
    
#ifdef LOGS
    printf("Char Side: %d\n", side);
    printf("Char Forward: %d\n", forward);
#endif
    Vector2 input = (Vector2){ (float)side, (float)-forward };

#if defined(NORMALIZE_INPUT)
    // Slow down diagonal movement
    if ((side != 0) && (forward != 0)) input = Vector2Normalize(input);
#endif

    float delta = GetFrameTime();

    if (!body->isGrounded) body->velocity.y -= GRAVITY*delta;

    if (body->isGrounded && jumpPressed)
    {
        body->velocity.y = JUMP_FORCE;
        body->isGrounded = false;

        // Sound can be played at this moment
        //SetSoundPitch(fxJump, 1.0f + (GetRandomValue(-100, 100)*0.001));
        //PlaySound(fxJump);
    }

    Vector3 front = (Vector3){ sinf(rot), 0.f, cosf(rot) };
    Vector3 right = (Vector3){ cosf(-rot), 0.f, sinf(-rot) };

    Vector3 desiredDir = (Vector3){ input.x*right.x + input.y*front.x, 0.0f, input.x*right.z + input.y*front.z, };
    body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL*delta);

    float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
    Vector3 hvel = (Vector3){ body->velocity.x*decel, 0.0f, body->velocity.z*decel };

    float hvelLength = Vector3Length(hvel); // Magnitude
    if (hvelLength < (NORMAL_SPEED*0.01f)) hvel = (Vector3){ 0 };

    // This is what creates strafing
    float speed = Vector3DotProduct(hvel, body->dir);

    // Whenever the amount of acceleration to add is clamped by the maximum acceleration constant,
    // a Player can make the speed faster by bringing the direction closer to horizontal velocity angle
    // More info here: https://youtu.be/v3zT3Z5apaM?t=165
    float sprint = IsKeyDown(KEY_LEFT_SHIFT) ? SPRINT_MULT : 1.0f;
    float maxSpeed = (crouchHold ? CROUCH_SPEED : NORMAL_SPEED * sprint);

    float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL*delta);
    hvel.x += body->dir.x*accel;
    hvel.z += body->dir.z*accel;

    body->velocity.x = hvel.x;
    body->velocity.z = hvel.z;

    body->position.x += body->velocity.x*delta;
    body->position.y += body->velocity.y*delta;
    body->position.z += body->velocity.z*delta;

    // Fancy collision system against the floor
    if (body->position.y <= 0.0f)
    {
        body->position.y = 0.0f;
        body->velocity.y = 0.0f;
        body->isGrounded = true; // Enable jumping
    }

    for (int i = 0; i < numObstacles; i++) {
        ResolveCollision(&player, &obstacles[i]);
    }

    Vector3 boxPos = { 5.0f, 1.0f, 0.0f };
    Vector3 boxSize = { 2.0f, 2.0f, 2.0f };

    Obstacle obs = {boxPos, boxSize};
    
    ResolveCollision(&player, &obs);
}

static void UpdateCameraFPS(Camera *camera)
{
    const Vector3 up = (Vector3){ 0.0f, 1.0f, 0.0f };
    const Vector3 targetOffset = (Vector3){ 0.0f, 0.0f, -1.0f };

    // Left and right
    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

    // Clamp view up
    float maxAngleUp = Vector3Angle(up, yaw);
    maxAngleUp -= 0.001f; // Avoid numerical errors
    if ( -(lookRotation.y) > maxAngleUp) { lookRotation.y = -maxAngleUp; }

    // Clamp view down
    float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
    maxAngleDown *= -1.0f; // Downwards angle is negative
    maxAngleDown += 0.001f; // Avoid numerical errors
    if ( -(lookRotation.y) < maxAngleDown) { lookRotation.y = -maxAngleDown; }

    // Up and down
    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

    // Rotate view vector around right axis
    float pitchAngle = -lookRotation.y - lean.y;
    pitchAngle = Clamp(pitchAngle, -PI/2 + 0.0001f, PI/2 - 0.0001f);
    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

    // Head animation
    // Rotate up direction around forward axis
    float headSin = sinf(headTimer*PI);
    float headCos = cosf(headTimer*PI);
    const float stepRotation = 0.0015f;
    camera->up = Vector3RotateByAxisAngle(up, pitch, headSin*stepRotation + lean.x);

    // Camera BOB
    const float bobSide = 0.03f;
    const float bobUp = 0.05f;
    Vector3 bobbing = Vector3Scale(right, headSin*bobSide);
    bobbing.y = fabsf(headCos*bobUp);

    camera->position = Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
    camera->target = Vector3Add(camera->position, pitch);
}


static void DrawLevel(void)
{
    const int floorExtent = 25;
    const float tileSize = 5.0f;
    const Color tileColor1 = (Color){ 150, 200, 200, 255 };

    // Floor tiles
    for (int y = -floorExtent; y < floorExtent; y++)
    {
        for (int x = -floorExtent; x < floorExtent; x++)
        {
            if ((y & 1) && (x & 1))
            {
                DrawPlane((Vector3){ x*tileSize, 0.0f, y*tileSize}, (Vector2){ tileSize, tileSize }, tileColor1);
            }
            else if (!(y & 1) && !(x & 1))
            {
                DrawPlane((Vector3){ x*tileSize, 0.0f, y*tileSize}, (Vector2){ tileSize, tileSize }, LIGHTGRAY);
            }
        }
    }

    for (int i = 0; i < numObstacles; i++)
    {
      
        DrawCube(obstacles[i].position, obstacles[i].size.x, obstacles[i].size.y, obstacles[i].size.z, (Color){ 150, 200, 200, 255 });

        DrawCubeWires(obstacles[i].position, obstacles[i].size.x, obstacles[i].size.y, obstacles[i].size.z, DARKBLUE);

    }
    

    // Red sun
    DrawSphere((Vector3){ 300.0f, 300.0f, 0.0f }, 100.0f, (Color){ 255, 0, 0, 255 });
}