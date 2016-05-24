#pragma once
#include <random>
#include "CMatrix.h"

struct TrainingData
{
   double* adInputData;
   double* adExpectedOutput;
};

class CNetwork
{
public:
   CNetwork( int iNumOfLayers, int aiNodesPerLayer[] );
   ~CNetwork();

   double* FeedFoward( double adInput[] );
   void StochasticGradientDescent( TrainingData astTrainingSet[], int iNumTrainingSet, int iNumOfEpochs, int iMiniBatchSize, double dLearningRate );

private:
   std::default_random_engine       m_Generator;
   std::normal_distribution<double> m_Distribution;
   int         m_iNumOfLayers;
   int*        m_aiNodesPerLayer;
   double**    m_mdBiases;
   double**    m_mdDelBiases;
   double***   m_amdWeights;
   double***   m_amdDelWeights;
   double**    m_amdActivations;
   
   double**    m_mdZVectors;
   double**    m_mdDelta;

   inline double Sigmoid( double dSum );
   inline double SigmoidPrime( double dSum );

   void UpdateMiniBatch( TrainingData astTrainingDataSet[], int iNumTrainingSet, double dLearningRate );
   void BackPropagation( TrainingData stTrainingData );
};
