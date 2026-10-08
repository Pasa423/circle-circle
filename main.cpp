#include<iostream>
#include "raylib.h"
using namespace std;
#include <cmath>
int circle(int x1=0, int y1=0, float t=0)
    {
      float r1 = 100*sin(t);
      if (r1<0)
            {
              r1=-r1;
            }
            DrawCircle(x1, y1, r1, WHITE);
            DrawCircle(x1,y1,r1-5, BLACK);
            return 0;
    }
int main()
{
    InitWindow(1000, 600, "Wavey");
    SetTargetFPS(60);
    int a[10],b[10];
    int i,c = 0;
    bool p = false;
    float t = 0;
    while (!WindowShouldClose()) {
    Vector2 m = GetMousePosition();
    t=t+GetFrameTime();
        if(IsKeyPressed(KEY_SPACE)&&c<10)
        {
          a[c]=m.x;
          b[c]=m.y;
          p = true;
          c++;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for(i=0;i<c;i++)
        {
        if(p)
        {
          circle(a[i],b[i],t);
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
