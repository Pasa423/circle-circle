#include<cmath>
#include<iostream>
#include<vector>
int main()
{
  float prevTemp[100][100],nextTemp[100][100];
  int Rows, Columns,i,j,NoofIterrations;
  float sum =0;
  int count =0;
  std::cout<<"write the length of the square"<<std::endl;
  std::cin>>Rows;
  Columns = Rows;
  std::cout<<"Write the no of iterations"<<std::endl;
  std::cin>>NoofIterrations;
  for(i=0;i<Rows;i++)
  {
    for(j=0;j<Columns;j++)
    {
      std::cout<<"write the value of Temp["<<i+1<<"]"<<"["<<j+1<<"]"<<std::endl;
      std::cin>>prevTemp[i][j];
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
    }
  for(int k=0;k<NoofIterrations;k++)
  {
    for(i=0;i<Rows;i++)
    {
      for(j=0;j<Columns;j++)
      {
       for(int di=-1;di<2;di++)
       {
         for(int dj=-1;dj<2;dj++)
         {
          if(i+di>-1 && i+di<Rows && j+dj>-1 && j+dj<Columns )
          {
            sum = sum + prevTemp[i+di][j+dj];
            count++;
          }
         }
       }
       nextTemp[i][j]= sum/count;
       sum = 0;
       count =0;
      }
    }
    for(i=0;i<Rows;i++)
    {
      for(j=0;j<Columns;j++)
      {
        prevTemp[i][j]=nextTemp[i][j];
      }
    }
    std::cout << "Temperature Grid of "<<k<<"iterations"<<std::endl;

    for (i = 0; i < Rows; i++)
    {
        for (j = 0; j < Columns; j++)
        {
            std::cout << nextTemp[i][j] <<" ";
        }
    std::cout <<""<<std::endl;
    }
  }
  return 0;
}
