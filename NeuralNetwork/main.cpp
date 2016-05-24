#include <stdio.h>
#include <stdlib.h>
#include "CNetwork.h"

int main( int argc, char* argv[] )
{
   /*int      aiNodesPerLayer[] = { 2, 2, 2, 1 };
   CNetwork objNet( sizeof(aiNodesPerLayer)/sizeof(int), aiNodesPerLayer );

   TrainingData   TrainindSet[4];
   TrainindSet[0].adInputData = new double[2];
   TrainindSet[0].adInputData[0] = 0;
   TrainindSet[0].adInputData[1] = 0;
   TrainindSet[0].adExpectedOutput = new double[1];
   TrainindSet[0].adExpectedOutput[0] = 0;

   TrainindSet[1].adInputData = new double[2];
   TrainindSet[1].adInputData[0] = 0;
   TrainindSet[1].adInputData[1] = 1;
   TrainindSet[1].adExpectedOutput = new double[1];
   TrainindSet[1].adExpectedOutput[0] = 1;

   TrainindSet[2].adInputData = new double[2];
   TrainindSet[2].adInputData[0] = 1;
   TrainindSet[2].adInputData[1] = 0;
   TrainindSet[2].adExpectedOutput = new double[1];
   TrainindSet[2].adExpectedOutput[0] = 1;

   TrainindSet[3].adInputData = new double[2];
   TrainindSet[3].adInputData[0] = 1;
   TrainindSet[3].adInputData[1] = 1;
   TrainindSet[3].adExpectedOutput = new double[1];
   TrainindSet[3].adExpectedOutput[0] = 0;

   double*  aOutput;

   objNet.StochasticGradientDescent( TrainindSet, 4, 10000, 1, 2.0 );

   for( int j = 0; j < 4; ++j )
   {
      aOutput = objNet.FeedFoward( TrainindSet[j].adInputData );
      for( int i = 0; i < aiNodesPerLayer[ sizeof(aiNodesPerLayer)/sizeof(int) - 1 ]; ++i )
      {
         printf( "Test %d: aOutput[%d] = %f\n", j, i, aOutput[i] );
      }
   }*/

	CMatrix objMatrix(2, 3, 1.0);
	CMatrix objMatrix2(3, 2, 3.0);

	objMatrix(0, 0) = 1.0;
	objMatrix(0, 1) = 2.0;
	objMatrix(0, 2) = 3.0;
	objMatrix(1, 0) = 4.0;
	objMatrix(1, 1) = 5.0;
	objMatrix(1, 2) = 6.0;

	objMatrix.Print();
	printf("\n");

	objMatrix2.Print();
	printf("\n");

	objMatrix = objMatrix * objMatrix2;

	objMatrix.Print();
	printf("\n");

   system( "pause" );
   return 0;
}