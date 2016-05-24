#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>
#include <algorithm>
#include <math.h>
#include "CNetwork.h"

using namespace std;

CNetwork::CNetwork( int iNumOfLayers, int aiNodesPerLayer[] ) : m_Generator( (unsigned int)chrono::system_clock::now().time_since_epoch().count() ),
                                                                m_Distribution( 0.0, 1.0 ),
                                                                m_aiNodesPerLayer( NULL ),
                                                                m_mdBiases( NULL ),
                                                                m_mdDelBiases( NULL ),
                                                                m_amdWeights( NULL ),
                                                                m_amdDelWeights( NULL ),
                                                                m_amdActivations( NULL ),
                                                                m_mdDelta( NULL )
{
   m_iNumOfLayers = iNumOfLayers;

   //------[ Initialize the layer structure of the network ]------
   m_aiNodesPerLayer = new int[ m_iNumOfLayers ];
   memcpy( m_aiNodesPerLayer, aiNodesPerLayer, (m_iNumOfLayers * sizeof(int)) );

   //------[ Initialize the biases and Z vectors for each layer ]------
   // The first layer is the input layer, so there are no biases for that
   m_mdBiases    = new double*[ m_iNumOfLayers - 1 ];
   m_mdDelBiases = new double*[ m_iNumOfLayers - 1 ];
   m_mdZVectors  = new double*[ m_iNumOfLayers - 1 ];
   for( int i = 0; i < (m_iNumOfLayers - 1); ++i )
   {
      m_mdBiases[i]    = new double[ m_aiNodesPerLayer[i + 1] ];
      m_mdDelBiases[i] = new double[ m_aiNodesPerLayer[i + 1] ];
      m_mdZVectors[i]  = new double[ m_aiNodesPerLayer[i + 1] ];
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_mdBiases[i][j]    = m_Distribution( m_Generator );
         m_mdDelBiases[i][j] = 0;
         m_mdZVectors[i][j]  = 0;
      }
   }

   //------[ Initialize the weights for each layer ]------
   // The weights only exists for each layer transition, so, for example, a 3 layer network will only have 2 weight matrices
   m_amdWeights    = new double**[ m_iNumOfLayers - 1 ];
   m_amdDelWeights = new double**[ m_iNumOfLayers - 1 ];
   printf( "m_amdDelWeights[] = %d\n", m_iNumOfLayers - 1 );
   for( int i = 0; i < (m_iNumOfLayers - 1); ++i )
   {  // Initialize each weight matrix
      m_amdWeights[i]    = new double*[ m_aiNodesPerLayer[i + 1] ];     // Lines
      m_amdDelWeights[i] = new double*[ m_aiNodesPerLayer[i + 1] ];
      printf( "\tm_amdDelWeights[][] = %d\n", m_aiNodesPerLayer[i + 1] );
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_amdWeights[i][j]    = new double[ m_aiNodesPerLayer[i] ];   // Collumn
         m_amdDelWeights[i][j] = new double[ m_aiNodesPerLayer[i] ];   // Collumn
         printf( "\t\tm_amdDelWeights[][][] = %d\n", m_aiNodesPerLayer[i] );
         for( int z = 0; z < m_aiNodesPerLayer[i] ; ++z )
         {
            m_amdWeights[i][j][z]    = m_Distribution( m_Generator );
            m_amdDelWeights[i][j][z] = 0;
         }
      }
   }

   //------[ Initialize the output vector for each layer ]------
   m_amdActivations = new double*[ m_iNumOfLayers - 1 ];
   for( int i = 0; i < m_iNumOfLayers - 1; i++ )
   {
      m_amdActivations[i] = new double[ m_aiNodesPerLayer[i + 1] ];
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_amdActivations[i][j] = 0;
      }
   }

   m_mdDelta = new double*[ m_iNumOfLayers - 1 ];                 // Layers
   for( int i = 0; i < (m_iNumOfLayers - 1); ++i )
   {
      m_mdDelta[i] = new double[ m_aiNodesPerLayer[i + 1] ];      // Lines
   }
}

CNetwork::~CNetwork()
{
   if( NULL != m_mdDelta )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_mdDelta[i] )
         {
            delete [] m_mdDelta[i];
            m_mdDelta[i] = NULL;
         }
      }
      delete [] m_mdDelta;
      m_mdDelta = NULL;
   }

   if( NULL != m_amdActivations )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_amdActivations[i] )
         {
            delete [] m_amdActivations[i];
            m_amdActivations[i] = NULL;
         }
      }
      delete [] m_amdActivations;
      m_amdActivations = NULL;
   }

   if( NULL != m_amdWeights )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_amdWeights[i] )
         {
            for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
            {
               delete [] m_amdWeights[i][j];
               m_amdWeights[i][j] = NULL;
            }
            delete [] m_amdWeights[i];
            m_amdWeights[i] = NULL;
         }
      }
      delete [] m_amdWeights;
      m_amdWeights = NULL;
   }

   if( NULL != m_amdDelWeights )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_amdDelWeights[i] )
         {
            for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
            {
               delete [] m_amdDelWeights[i][j];
               m_amdDelWeights[i][j] = NULL;
            }
            delete [] m_amdDelWeights[i];
            m_amdDelWeights[i] = NULL;
         }
      }
      delete [] m_amdDelWeights;
      m_amdDelWeights = NULL;
   }

   if( NULL != m_mdBiases )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_mdBiases[i] )
         {
            delete [] m_mdBiases[i];
            m_mdBiases[i] = NULL;
         }
      }
      delete [] m_mdBiases;
      m_mdBiases = NULL;
   }

   if( NULL != m_mdDelBiases )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_mdDelBiases[i] )
         {
            delete [] m_mdDelBiases[i];
            m_mdDelBiases[i] = NULL;
         }
      }
      delete [] m_mdDelBiases;
      m_mdDelBiases = NULL;
   }

   if( NULL != m_mdZVectors )
   {
      for(int i = 0; i < m_iNumOfLayers - 1; ++i)
      {
         if( NULL != m_mdZVectors[i] )
         {
            delete [] m_mdZVectors[i];
            m_mdZVectors[i] = NULL;
         }
      }
      delete [] m_mdZVectors;
      m_mdZVectors = NULL;
   }

   if( NULL != m_aiNodesPerLayer )
   {
      delete [] m_aiNodesPerLayer;
      m_aiNodesPerLayer = NULL;
   }
}

double* CNetwork::FeedFoward( double adInput[] )
{
   //------[ First layer uses adInput as the input values ]------
   for( int j = 0; j < m_aiNodesPerLayer[1]; ++j )
   {
      m_mdZVectors[0][j] = 0;
      for( int z = 0; z < m_aiNodesPerLayer[0]; ++z )
      {
         m_mdZVectors[0][j] += m_amdWeights[0][j][z] * adInput[z];
      }
      m_mdZVectors[0][j] += m_mdBiases[0][j];
      m_amdActivations[0][j] = Sigmoid( m_mdZVectors[0][j] );
   }

   //------[ Other layers uses the previous layer output as input ]------
   for( int i = 1; i < m_iNumOfLayers - 1; i++ )
   {  // Do the matrix multiplication for each layer os the network
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_mdZVectors[i][j] = 0;
         for( int z = 0; z < m_aiNodesPerLayer[i]; ++z )
         {
            m_mdZVectors[i][j] += m_amdWeights[i][j][z] * m_amdActivations[i-1][z];
         }
         m_mdZVectors[i][j] += m_mdBiases[i][j];
         m_amdActivations[i][j] = Sigmoid( m_mdZVectors[i][j] );
      }
   }

   return m_amdActivations[ m_iNumOfLayers - 2 ];
}

void CNetwork::StochasticGradientDescent( TrainingData astTrainingSet[], int iNumTrainingSet, int iNumOfEpochs, int iMiniBatchSize, double dLearningRate )
{
   for( int i = 0; i < iNumOfEpochs; ++i )
   {
      //random_shuffle( astTrainingSet, astTrainingSet + iNumTrainingSet );

      for( int j = 0; j < iNumTrainingSet; j += iMiniBatchSize )
      {
         if( (iNumTrainingSet - j) >= iMiniBatchSize )
         {
            UpdateMiniBatch( &astTrainingSet[j], iMiniBatchSize, dLearningRate );
         }
         else
         {
            UpdateMiniBatch( &astTrainingSet[j], (iNumTrainingSet - j), dLearningRate );   
         }
      }
      //printf( "Epoch %d of %d complete\n", i, iNumOfEpochs );
   }
}

double CNetwork::Sigmoid( double dSum )
{  // The sigmoid function
   return 1.0 / ( 1.0 + exp( -dSum ) );
}

double CNetwork::SigmoidPrime( double dSum )
{  // Derivative of the sigmoid function
   double dSigmoidResult = Sigmoid(dSum);
   return ( 1.0 - dSigmoidResult ) * dSigmoidResult;
}

void CNetwork::UpdateMiniBatch( TrainingData astTrainingDataSet[], int iNumTrainingSet, double dLearningRate )
{
   //------[ Zero all the deltas ]------
   for( int i = 0; i < (m_iNumOfLayers - 1); ++i )
   {
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_mdDelBiases[i][j] = 0;
         for( int z = 0; z < m_aiNodesPerLayer[i] ; ++z )
         {
            m_amdDelWeights[i][j][z] = 0;
         }
      }
   }

   //------[ Calculate the deltas based on the training set given ]------
   for( int index = 0; index < iNumTrainingSet; ++index )
   {
      BackPropagation( astTrainingDataSet[index] );
   }

   //------[ Update the network weigths and biases ]------
   dLearningRate = dLearningRate / iNumTrainingSet;      // Scale the learning rate to the training set length

   for( int i = 0; i < (m_iNumOfLayers - 1); ++i )
   {
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         m_mdBiases[i][j] -= dLearningRate * m_mdDelBiases[i][j];
         //printf( "m_mdBiases[%d][%d] = %f\n", i, j, m_mdBiases[i][j] );
         for( int z = 0; z < m_aiNodesPerLayer[i] ; ++z )
         {
            m_amdWeights[i][j][z] -= dLearningRate * m_amdDelWeights[i][j][z];
            //printf( "m_amdWeights[%d][%d][%d] = %f\n", i, j, z, m_amdWeights[i][j][z] );
         }
      }
   }

   //printf( "\n" );
}

void CNetwork::BackPropagation( TrainingData stTrainingData )
{
   

   //------[ Compute the output error, estoring the intermidiate values ]------
   FeedFoward( stTrainingData.adInputData );             // Compute the output. This will also update the intermidiate values we need

   for( int i = (m_iNumOfLayers - 2); i >= 0; --i )
   {
      for( int j = 0; j < m_aiNodesPerLayer[i + 1]; ++j )
      {
         if( (m_iNumOfLayers - 2) == i )
         {
            m_mdDelta[i][j] = (m_amdActivations[i][j] - stTrainingData.adExpectedOutput[j]) * SigmoidPrime( m_mdZVectors[i][j] );
            //printf( "dDelta[%d][%d] = %f\n", i, j, dDelta[i][j] );
         }
         else
         {
            m_mdDelta[i][j] = 0;
            for( int z = 0; z < m_aiNodesPerLayer[i + 2]; ++z )
            {
               m_mdDelta[i][j] += m_amdWeights[i + 2][z][j] * m_mdDelta[i+1][j];
            }
            m_mdDelta[i][j] *= SigmoidPrime( m_mdZVectors[i][j] );
         }

         m_mdDelBiases[i][j] += m_mdDelta[i][j];
         printf( "m_mdDelBiases[%d][%d] = %f\n", i, j, m_mdDelBiases[i][j] );
         
         for( int z = 0; z < m_aiNodesPerLayer[i] ; ++z )
         {
            if( 0 == i )
            {
               m_amdDelWeights[i][j][z] += m_mdDelta[i][j] * stTrainingData.adInputData[z];
               //printf( "m_amdDelWeights[%d][%d][%d] = %f  >  Input[%d]=%f\n", i, j, z, m_amdDelWeights[i][j][z], z, stTrainingData.adInputData[z] );
            }
            else
            {
               m_amdDelWeights[i][j][z] += m_mdDelta[i][j] * m_amdActivations[i - 1][z];
            }
         }
      }
   }
}
