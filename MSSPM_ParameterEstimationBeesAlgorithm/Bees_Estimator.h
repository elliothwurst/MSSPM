/**
 * @file Bees_Estimator.h
 * @brief Class definition for the Bees_Estimator API
 *
 * This file contains the class definition for the Bees_Estimator API. This
 * API acts as an interface class between the Main calling program and the lower-level
 * Bess Algorithm implementation.
 *
 * @copyright
 * Public Domain Notice\n
 *
 * National Oceanic And Atmospheric Administration\n\n
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.\n\n
 *
 * Please cite the author(s) in any work or product based on this material.
 */

#pragma once

#include "boost/random.hpp"

#include <cmath>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <memory>
#include "time.h"
#include "nmfConstantsMSSPM.h"
#include "nmfUtils.h"
//#include "nmfRandom.h"

#include "BeesAlgorithm.h"
#include "BeesStats.h"

#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include <chrono>
#include <thread>

typedef boost::uniform_real<> NumberDistribution;
typedef boost::mt19937 RandomNumberGenerator;
typedef boost::variate_generator<RandomNumberGenerator&,NumberDistribution> Generator;

/**
 * @brief This class acts as an interface class to the Bees algorithm implementation.
 */
class Bees_Estimator : public QObject
{

    Q_OBJECT

private:
    std::vector<double>                   m_InitialCarryingCapacities;
    double                                m_EstSystemCarryingCapacity;
    std::vector<double>                   m_EstGrowthRates;
    std::vector<double>                   m_EstCarryingCapacities;
//  std::vector<double>                   m_EstExploitationRates;
    std::vector<double>                   m_EstCatchability;
    std::vector<double>                   m_EstExponent;
    boost::numeric::ublas::matrix<double> m_EstAlpha;
    boost::numeric::ublas::matrix<double> m_EstBetaSpecies;
    boost::numeric::ublas::matrix<double> m_EstBetaGuilds;
    boost::numeric::ublas::matrix<double> m_EstPredation;
    boost::numeric::ublas::matrix<double> m_EstHandling;

    void createOutputStr(const int&         numTotalParameters,
                         const int&         numEstParameters,
                         const int&         numSubRuns,
                         const double&      bestFitness,
                         const double&      fitnessStdDev,
                         const Data_Struct& beeStruct,
                         std::string&       bestFitnessStr);
    std::string convertValues1DToOutputStr(const std::string& label,
                                           const std::vector<double>& valuesVec,
                                           const bool& includeTotal);
    std::string convertValues2DToOutputStr(const std::string& label,
                                           const boost::numeric::ublas::matrix<double>& valuesMatrix);
    void outputProgressData(std::string msg);
    void printBee(std::string msg,
                  double &fitness,
                  std::vector<double> &parameters);
    void stopRun(const std::string &elapsedTimeStr,
                 const std::string &fitnessStr);
    bool wasStoppedByUser();

signals:
    /**
     * @brief Signal emitted to update the calling program of an error in the Bees algorithm
     * @param errorMsg : string value the error message from the Bees algorithm
     */
    void ErrorFound(std::string errorMsg);
    /**
     * @brief Signal emitted with the Run has completed
     * @param bestFitness : string value representing the best fitness value
     * @param showDiagnosticsChart : boolean signfying whether the
     * diagnostic 3d chart should be displayed after the run completes
     */
    void RunCompleted(std::string bestFitness, bool showDiagnosticsChart);
    /**
     * @brief Signal emitted when a sub run is completed
     * @param RunNum : the number of the parent run
     * @param SubRun : the number of the child sub run
     * @param NumSubRuns : number of sub runs in the parent run
     */
    void SubRunCompleted(int RunNum, int SubRun, int NumSubRuns);
//  void UpdateProgressData(int NumSpecies, int NumParams, QString elapsedTime);

public:
    /**
     * @brief Class constructor for the Bees Estimation Algorithm interface class
     */
    Bees_Estimator();
   ~Bees_Estimator();

    /**
     * @brief The main routine that runs the Bees Estimation algorithm
     * @param BeeStruct : data structure containing parameters needed for the Bees algorithm
     * @param RunNum : the run number
     */
    void estimateParameters(Data_Struct &BeeStruct,int RunNum);
    /**
     * @brief Gets the estimated carrying capacity values per species
     * @param EstCarryingCapacity : vector of carrying capacities per species
     */
    void getEstimatedCarryingCapacities(std::vector<double> &EstCarryingCapacity);
    /**
     * @brief Gets the estimated catchability values per species
     * @param EstCatchability : vector of catchability values per species
     */
    void getEstimatedCatchability(std::vector<double> &EstCatchability);
    /**
     * @brief Gets the estimated alpha interaction values per species
     * @param EstInteraction : vector of alpha interaction values per species
     */
    void getEstimatedCompetitionAlpha(boost::numeric::ublas::matrix<double> &EstInteraction);
    /**
     * @brief Gets the estimated beta competition values per guild
     * @param EstCompGuilds : vector of beta competition values per guild
     */
    void getEstimatedCompetitionBetaGuilds(boost::numeric::ublas::matrix<double> &EstCompGuilds);
    /**
     * @brief Gets the estimated beta competition values per species
     * @param EstCompSpecies : vector of beta competition values per species
     */
    void getEstimatedCompetitionBetaSpecies(boost::numeric::ublas::matrix<double> &EstCompSpecies);
    /**
     * @brief Gets the estimated exponent values per species
     * @param EstExponent : vector of exponent values per species
     */
    void getEstimatedExponent(std::vector<double> &EstExponent);
    /**
     * @brief Gets the estimated growth rate values per species
     * @param EstGrowthRates : vector of growth rate values per species
     */
    void getEstimatedGrowthRates(std::vector<double> &EstGrowthRates);
    /**
     * @brief Gets the estimated handling values per species
     * @param EstHandling : vector of handling values per species
     */
    void getEstimatedHandling(boost::numeric::ublas::matrix<double> &EstHandling);
    /**
     * @brief Gets the estimated predation values per species
     * @param EstPredation : vector of predation values per species
     */
    void getEstimatedPredation(boost::numeric::ublas::matrix<double> &EstPredation);

};


