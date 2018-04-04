#include <iostream>
#include <cstdlib>
#include <math.h>


using namespace std;
#define N 8
#define k 3
#define LevelNumber 3
#define startLevel 0

int stationsArray[N]={0};
int success=0;
int collisions=0;
int idles=0;

int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

void populateArray(){
    int random_int;
    //while number of stations unpopulated(i) is less than number of stations to transmit(k)
    for(int i=1; i<=k; i++){
        random_int = random(0,(N-1));

        if(stationsArray[random_int] == 0){
            printf("GENERATED: %d\n", random_int);
            stationsArray[random_int] = 1;
        }
        //unless its been visited already
        else if(stationsArray[random_int]==1){
            random_int = random(0,(N-1));
            printf("GENERATED: %d\n", random_int);
            stationsArray[random_int] = 1;
        }

    }
}

// void levelCheck(){
//     //probe certain size of the array at a time depending on startLevel
//     int last=0;
//     int chunkSize = N / (pow(2, startLevel));
//     for(int first = 0; last<=N; first+=chunkSize){
//         last+=chunkSize;
//         probeAlgorithm(first, last)
//     }
// }

void probeAlgorithm(int first, int last){
    //only run while all successful stations have not been visited
    if (success<k) {
        printf("FIRST: %d, LAST: %d\n",first, last);
        //1. if any of the probes in the range cause collisions, further subdivide recursively
        int localSuccess=0;
        int localIdle=0;
        int localCollision=0;
        for(int i=first; i<=last; i++){
            printf("i = %d\n", i);
            //while there are less than 2 successes we know no collisions occured
            if(stationsArray[i]==1){
                if(localSuccess<1){
                    localSuccess+=1;
                }
                //there is a collision
                else{
                    localCollision+=1;
                    collisions+=1;
                    break;
                }
            }
        }
        //Now that we are out of the loop, if there were collisions, split and recursively search both halves
        if(localCollision>0){
            printf("Local collisions: %d, going deep\n", localCollision);
            int mid = (first+last)/2;
            probeAlgorithm(first, mid);
            probeAlgorithm((mid+1), last);
        }
        //Else if there was 1 local success without any collisions we know a station transmitted without issues, increment success counter
        else if(localSuccess<2 && localSuccess>0){
            success+=1;
            printf("Success: %d\n", success);

        }
        //Else, Idle, and no
        else{
            idles+=1;
            printf("Idle++, Idle = %d\n",idles);
        }
    }

}

int main()
{
    // populateArray(0, N);
    stationsArray[1] = 1;
    stationsArray[5] = 1;
    stationsArray[6] = 1;


    probeAlgorithm(0,N-1);

    printf("\nSUCCESSES: %d\n", success);
    printf("IDLES: %d\n", idles);
    printf("COLLISIONS: %d\n", collisions);
    printf("--------------\nTOTAL PROBES: %d\n",success+idles+collisions);

    return 0;
}
