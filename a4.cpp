#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <time.h>

using namespace std;

int N =1024;
int k =3;
int startLevel =0;
int numStations[] = {1,2,4,8,16,32,64,128,256,512,1024};
int numLevels[] = {0,2,4,6,8,10};
int stationsArray[1024]={0};
int success=0;
int collisions=0;
int idles=0;
int total;
int successTotal;
//Great naming, I know.
int totalTotal;

double successStats[6][11];
//Random number generator, courtesy of stack overflow
void randomGen() {
  for (int i = 0; i < N; i++)
    stationsArray[i] = 0;
  vector<int> randomStationsVec(N);
  for (int i = 0; i < randomStationsVec.size(); i++)
    randomStationsVec[i] = i;
  int numReady = 0;
  for (int h = 0; numReady != k; h++) {
    h = h % randomStationsVec.size();
    int selected = rand() % randomStationsVec.size();
    stationsArray[randomStationsVec[selected]] = 1;
    randomStationsVec.erase(randomStationsVec.begin()+selected);
    numReady++;
  }
}
//Essentially the ATW algorithm
void probeAlgorithm(int first, int last){
        int localSuccess=0;
        int localIdle=0;
        int localCollision=0;
        for(int i=first; i<=last; i++){
            //while there are less than 2 successes we know no collisions occured
            if(stationsArray[i]==1){
                if(localSuccess<1){
                    localSuccess+=1;
                }
                //there is a collision
                else{
                    localCollision+=1;
                    collisions+=1;
                    total+=1;
                    break;
                }
            }
        }
        //Now that we are out of the loop, if there were collisions, split and recursively search both halves
        if(localCollision>0){
            int mid = (first+last)/2;
            probeAlgorithm(first, mid);
            probeAlgorithm((mid+1), last);
        }
        //Else if there was 1 local success without any collisions we know a station transmitted without issues, increment success counter
        else if(localSuccess<2 && localSuccess>0){
            success+=1;
            total+=1;

        }
        //Else, Idle, and no
        else{
            idles+=1;
            total+=1;

        }

}
//handles starting at different levels
void levelCheck(){
    //probe certain size of the array at a time depending on startLevel
    int last=0;
    int chunkSize = N / (pow(2, startLevel));
    for(int first = 0; first<N; first+=chunkSize){
        last+=chunkSize;
        probeAlgorithm(first, last-1);
    }
}
double simulateSuccess(int level, int active_stations){
    double percentageRatio;
    success=0;
    collisions=0;
    idles=0;
    total=0;
    k=active_stations;
    startLevel=level;
    for(int i = 0; i<1; i++){
        randomGen();
        levelCheck();
        memset(stationsArray, 0, sizeof(stationsArray));
    }
    percentageRatio = ((double)success/(double)total);
    return percentageRatio;
}
double simulateIdles(int level, int active_stations){
    double percentageRatio;
    success=0;
    collisions=0;
    idles=0;
    total=0;
    k=active_stations;
    startLevel=level;
    for(int i = 0; i<1; i++){
        randomGen();
        levelCheck();
        memset(stationsArray, 0, sizeof(stationsArray));
    }
    percentageRatio = ((double)idles/(double)total);
    return percentageRatio;
}
double simulateCollisions(int level, int active_stations){
    double percentageRatio;
    success=0;
    collisions=0;
    idles=0;
    total=0;
    k=active_stations;
    startLevel=level;
    for(int i = 0; i<1; i++){
        randomGen();
        levelCheck();
        memset(stationsArray, 0, sizeof(stationsArray));
    }
    percentageRatio = ((double)collisions/(double)total);
    return percentageRatio;
}
void calculateSuccessStats(){
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "       \t\t\t\tSuccess Table: Transmitting Stations  \t\t\t\t      ";
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "Level | ";
    for (int i = 0; i < 11; i++) {
      cout << numStations[i];
      if (i != 10) cout << "\t";
    }
    cout << "\n---------------------------------------------------------------------------------------------\n";
    for(int i=0; i<6;i++){
        cout<<numLevels[i];
        for(int j=0; j<11; j++){
            successStats[i][j] = simulateSuccess(numLevels[i], numStations[j]);
            cout << "\t" << setprecision(3) << fixed << successStats[i][j];
        }
        cout << endl;
    }
}
void calculateIdleStats(){
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "       \t\t\t\tIdle Table: Transmitting Stations  \t\t\t\t      ";
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "Level | ";
    for (int i = 0; i < 11; i++) {
      cout << numStations[i];
      if (i != 10) cout << "\t";
    }
    cout << "\n---------------------------------------------------------------------------------------------\n";
    for(int i=0; i<6;i++){
        cout<<numLevels[i];
        for(int j=0; j<11; j++){
            successStats[i][j] = simulateIdles(numLevels[i], numStations[j]);
            cout << "\t" << setprecision(3) << fixed << successStats[i][j];
        }
        cout << endl;
    }
}
void calculateCollisionStats(){
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "       \t\t\t\tCollision Table: Transmitting Stations  \t\t\t\t      ";
    cout << "\n---------------------------------------------------------------------------------------------\n";
    cout << "Level | ";
    for (int i = 0; i < 11; i++) {
      cout << numStations[i];
      if (i != 10) cout << "\t";
    }
    cout << "\n---------------------------------------------------------------------------------------------\n";
    for(int i=0; i<6;i++){
        cout<<numLevels[i];
        for(int j=0; j<11; j++){
            successStats[i][j] = simulateCollisions(numLevels[i], numStations[j]);
            cout << "\t" << setprecision(3) << fixed << successStats[i][j];
        }
        cout << endl;
    }
}
int main()
{
    calculateSuccessStats();
    calculateIdleStats();
    calculateCollisionStats();
    return 0;
}
