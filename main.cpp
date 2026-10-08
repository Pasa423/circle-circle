#include<iostream>
#include "raylib.h"
using namespace std;
#include <cmath>
int circle(float x1=0, float y1=0, float SpawnT=0,float t=0)
    {
      t = t - SpawnT;
      float OuterR = 100*t;
      float InnerR=OuterR - 5;
      if (OuterR<0)
            {
              OuterR=-OuterR;
            }
      if(InnerR<0)
      {
        InnerR=-InnerR;
      }
            DrawRing({x1, y1},InnerR,OuterR,0,360,0,  WHITE);
            return 0;
    }
int main()
{
    InitWindow(1000, 600, "Wavey");
    SetTargetFPS(60);
    float a[100],b[100];
    int i,c = 0;
    bool p = false;
    float SpawnT[100],t = 0;
    while (!WindowShouldClose()) {
    Vector2 m = GetMousePosition();
    t=t+GetFrameTime();
        if(IsKeyPressed(KEY_SPACE)&&c<100)
        {
          a[c]=m.x;
          b[c]=m.y;
          p = true;
          SpawnT[c]=t;
          c++;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for(i=0;i<c;i++)
        {
        if(p)
        {
          circle(a[i],b[i],SpawnT[i],t);
        }
        }
/*            float x2 = 200;
            float y2 = 400;
            float r2 = 100*time;
            if (r2<0)
            {
              r2=-r2;
            }
            DrawCircle(x2, y2, r2, WHITE);
            DrawCircle(x2,y2,r2-5, BLACK); */
        EndDrawing();
    }
    CloseWindow();
}
