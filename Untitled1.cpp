#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    int door[3] = {0};
    const int totalTime = 1000000;
    float count = 0;
    
    for(int i = 0; i < totalTime; i ++)
    {
         for(int k = 0; k < 3; k++)
         {
             door[k] = 0;
         }
         int bmw = rand()%3;
         door[bmw] = 1;
         int select = rand()%3;
         if(select == bmw)count++;
    }
    cout << (float)count/totalTime << endl;

    system("pause");
    return 0;
}
