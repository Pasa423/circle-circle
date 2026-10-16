#include <cmath>
#include <iostream>
#include <vector>
#include "raylib.h"
int main() {
  float prevTemp[100][100] = {};
  float nextTemp[100][100] = {};
  int Rows, Columns, i, j, k, di, dj;
  float sum = 0;
  int count = 0;
  std::cout << "write the length of the square" << std::endl;
  std::cin >> Rows;
  Columns = Rows;
  int Windowlength = 800;
  int CellSize=Windowlength/Rows;
  /* for(i=0;i<Rows;i++)
   {
     for(j=0;j<Columns;j++)
     {
       std::cout<<"write the value of
   Temp["<<i+1<<"]"<<"["<<j+1<<"]"<<std::endl; std::cin>>prevTemp[i][j];
     }
   }
   std::cout << "Temperature Grid:"<<std::endl;

     for (i = 0; i < Rows; i++)
     {
         for (j = 0; j < Columns; j++)
         {
             std::cout << prevTemp[i][j] <<" ";
         }

     std::cout <<""<<std::endl;
     }  */
  InitWindow(Windowlength, Windowlength, "uhhh");
  SetTargetFPS(60);
  while (!WindowShouldClose()) 
  {
    BeginDrawing();
    ClearBackground(BLACK);
    int mouseX = GetMouseX();
int mouseY = GetMouseY();

int j = mouseX / CellSize;
int i = mouseY / CellSize;

if (i >= 0 && i < Rows && j >= 0 && j < Columns) {
  if (IsKeyDown(KEY_I)) {
    prevTemp[i][j] += 20;
  }

  if (IsKeyDown(KEY_D)) {
    prevTemp[i][j] -= 20;
  }
}
    if(IsKeyDown(KEY_SPACE)) {
      for (i = 0; i < Rows; i++) {
        for (j = 0; j < Columns; j++) {
          for (di = -1; di < 2; di++) {
            for (dj = -1; dj < 2; dj++) {
              if (i + di > -1 && i + di < Rows && j + dj > -1 &&
                  j + dj < Columns) {
                sum = sum + prevTemp[i + di][j + dj];
                count++;
              }
            }
          }
          nextTemp[i][j] = sum / count;
          sum = 0;
          count = 0;
        }
      }
      for (i = 0; i < Rows; i++) {
        for (j = 0; j < Columns; j++) {
          prevTemp[i][j] = nextTemp[i][j];
        }
      }
    }
    for (i = 0; i < Rows; i++) {
      for (j = 0; j < Columns; j++) {
        float t = prevTemp[i][j] / 100.0f;
        if (t > 1) t = 1;
        if (t < 0) t = 0;

        Color c = {
          (unsigned char)(255 * t), 0, (unsigned char)(255 * (1 - t)), 255
                  };

        DrawRectangle(j * CellSize, i * CellSize,
                  CellSize, CellSize, c);
  }
}
    /*  std::cout << "Temperature Grid of " << k << "iterations" << std::endl;

      for (i = 0; i < Rows; i++) {
        for (j = 0; j < Columns; j++) {
          std::cout << nextTemp[i][j] << " ";
        }
        std::cout << "" << std::endl;
      }
    */
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
