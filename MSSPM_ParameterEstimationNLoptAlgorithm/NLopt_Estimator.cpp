#include "NLopt_Estimator.h"

#include <iomanip>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>

bool m_Quit;
//int NLopt_Estimator::m_NLoptIters    = 0;
int NLopt_Estimator::m_NLoptFcnEvals = 0;
int NLopt_Estimator::m_NumObjFcnCalls = 0;
//int NLopt_Estimator::m_Counter       = 0;
int NLopt_Estimator::m_RunNum        = 0;
nlopt::opt       NLopt_Estimator::m_Optimizer;

std::unique_ptr<nmfGrowthForm>      NLoptGrowthForm;
std::unique_ptr<nmfHarvestForm>     NLoptHarvestForm;
std::unique_ptr<nmfCompetitionForm> NLoptCompetitionForm;
std::unique_ptr<nmfPredationForm>   NLoptPredationForm;


NLopt_Estimator::NLopt_Estimator()
{
    m_Quit = false;
//  m_Counter = 0;
    m_MinimizerToEnum.clear();

    // Load Minimizer Name Map with global algorithms
    m_MinimizerToEnum["GN_ORIG_DIRECT_L"] = nlopt::GN_ORIG_DIRECT_L;
    m_MinimizerToEnum["GN_DIRECT_L"]      = nlopt::GN_DIRECT_L;
    m_MinimizerToEnum["GN_DIRECT_L_RAND"] = nlopt::GN_DIRECT_L_RAND;
    m_MinimizerToEnum["GN_CRS2_LM"]       = nlopt::GN_CRS2_LM;
    m_MinimizerToEnum["GD_StoGO"]         = nlopt::GD_STOGO;

    // Load Minimizer Name Map with local algorithms
    m_MinimizerToEnum["LN_COBYLA"]        = nlopt::LN_COBYLA;
    m_MinimizerToEnum["LN_BOBYQA"]        = nlopt::LN_BOBYQA;
    m_MinimizerToEnum["LN_PRAXIS"]        = nlopt::LN_PRAXIS;
    m_MinimizerToEnum["LN_NELDERMEAD"]    = nlopt::LN_NELDERMEAD;
    m_MinimizerToEnum["LN_SBPLX"]         = nlopt::LN_SBPLX;
    m_MinimizerToEnum["LD_MMA"]           = nlopt::LD_MMA;
    m_MinimizerToEnum["LD_SLSQP"]         = nlopt::LD_SLSQP;
    m_MinimizerToEnum["LD_LBFGS"]         = nlopt::LD_LBFGS;

}

NLopt_Estimator::~NLopt_Estimator()
{
}

QString
NLopt_Estimator::getVersion()
{
    int major;
    int minor;
    int bugfix;

    nlopt::version(major,minor,bugfix);

    return (QString::number(major) + "." +
            QString::number(minor) + "." +
            QString::number(bugfix));
}

void
NLopt_Estimator::extractParameters(const Data_Struct& NLoptDataStruct,
                                   const double *EstParameters,
                                   std::vector<double>& growthRate,
                                   std::vector<double>& carryingCapacity,
                                   std::vector<double>& catchabilityRate,
                                   boost::numeric::ublas::matrix<double>& competitionAlpha,
                                   boost::numeric::ublas::matrix<double>& competitionBetaSpecies,
                                   boost::numeric::ublas::matrix<double>& competitionBetaGuilds,
                                   boost::numeric::ublas::matrix<double>& predation,
                                   boost::numeric::ublas::matrix<double>& handling,
                                   std::vector<double>& exponent)
{
    bool isLogistic     = (NLoptDataStruct.GrowthForm      == "Logistic");
    bool isCatchability = (NLoptDataStruct.HarvestForm     == "Effort (qE)");
    bool isAlpha        = (NLoptDataStruct.CompetitionForm == "NO_K");
    bool isMSPROD       = (NLoptDataStruct.CompetitionForm == "MS-PROD");
    bool isAGGPROD      = (NLoptDataStruct.CompetitionForm == "AGG-PROD");
    bool isRho          = (NLoptDataStruct.PredationForm   == "Type I") ||
                          (NLoptDataStruct.PredationForm   == "Type II") ||
                          (NLoptDataStruct.PredationForm   == "Type III");
    bool isHandling     = (NLoptDataStruct.PredationForm   == "Type II") ||
                          (NLoptDataStruct.PredationForm   == "Type III");
    bool isExponent     = (NLoptDataStruct.PredationForm   == "Type III");
    int m;
    int offset = 0;
    int NumSpecies = NLoptDataStruct.NumSpecies;
    int NumGuilds  = NLoptDataStruct.NumGuilds;
    int NumSpeciesOrGuilds = (isAGGPROD) ? NumGuilds : NumSpecies;
    int MatrixSize = NumSpeciesOrGuilds*NumSpeciesOrGuilds;

    growthRate.clear();
    carryingCapacity.clear();
    catchabilityRate.clear();
    competitionAlpha.clear();
    competitionBetaSpecies.clear();
    competitionBetaGuilds.clear();
    predation.clear();
    handling.clear();
    exponent.clear();

    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        growthRate.emplace_back(EstParameters[i]);
    }
    offset += NumSpeciesOrGuilds;

    if (isLogistic) {
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            carryingCapacity.emplace_back(EstParameters[offset+i]);
        }
        offset += NumSpeciesOrGuilds;
    }

    if (isCatchability) {
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            catchabilityRate.emplace_back(EstParameters[offset+i]);
        }
        offset += NumSpeciesOrGuilds;
    }

    if (isAlpha) {
        m = 0;
        nmfUtils::initialize(competitionAlpha,NumSpeciesOrGuilds,NumSpeciesOrGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                competitionAlpha(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += MatrixSize;
    }

    if (isMSPROD) {
        m = 0;
        nmfUtils::initialize(competitionBetaSpecies,NumSpeciesOrGuilds,NumSpeciesOrGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                competitionBetaSpecies(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += MatrixSize;
        m = 0;
        nmfUtils::initialize(competitionBetaGuilds, NumSpeciesOrGuilds,NumGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumGuilds; ++j) {
                competitionBetaGuilds(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += NumSpeciesOrGuilds*NumGuilds;
    }

    if (isAGGPROD) {
        m = 0;
        nmfUtils::initialize(competitionBetaGuilds, NumSpeciesOrGuilds,NumGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumGuilds; ++j) {
                competitionBetaGuilds(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += NumSpeciesOrGuilds*NumGuilds;
    }

    if (isRho) {
        m = 0;
        nmfUtils::initialize(predation,NumSpeciesOrGuilds,NumSpeciesOrGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                predation(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += MatrixSize;
    }

    if (isHandling) {
        m = 0;
        nmfUtils::initialize(handling,NumSpeciesOrGuilds,NumSpeciesOrGuilds);
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                handling(i,j) = EstParameters[offset + (m++)];
            }
        }
        offset += MatrixSize;
    }

    if (isExponent) {
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            exponent.emplace_back(EstParameters[offset+i]);
        }
        offset += NumSpeciesOrGuilds;
    }
}



double
NLopt_Estimator::objectiveFunction(unsigned n,
                                   const double* EstParameters,
                                   double* gradient,
                                   void* dataPtr)
{
    const int DefaultFitness = 99999;
    Data_Struct NLoptDataStruct = *((Data_Struct *)dataPtr);
    bool isAggProd = (NLoptDataStruct.CompetitionForm == "AGG-PROD");
    double EstBiomassVal;
    double GrowthTerm;
    double HarvestTerm;
    double CompetitionTerm;
    double PredationTerm;
    double systemCarryingCapacity;
    double guildK;
    double fitness=0;
    int timeMinus1;
    int NumYears   = NLoptDataStruct.RunLength+1;
    int NumSpecies = NLoptDataStruct.NumSpecies;
    int NumGuilds  = NLoptDataStruct.NumGuilds;
    int guildNum = 0;
    int NumSpeciesOrGuilds;
    std::vector<double> growthRate;
    std::vector<double> carryingCapacity;
    std::vector<double> guildCarryingCapacity;
    std::vector<double> exponent;
    std::vector<double> catchabilityRate;
    boost::numeric::ublas::matrix<double> EstBiomassSpecies;
    boost::numeric::ublas::matrix<double> EstBiomassGuilds;
    boost::numeric::ublas::matrix<double> EstBiomassRescaled;
    boost::numeric::ublas::matrix<double> ObsBiomassBySpeciesOrGuildsRescaled;
    boost::numeric::ublas::matrix<double> competitionAlpha;
    boost::numeric::ublas::matrix<double> competitionBetaSpecies;
    boost::numeric::ublas::matrix<double> competitionBetaGuilds;
    boost::numeric::ublas::matrix<double> predation;
    boost::numeric::ublas::matrix<double> handling;
    boost::numeric::ublas::matrix<double> ObsBiomassBySpeciesOrGuilds;
    boost::numeric::ublas::matrix<double> Catch        = NLoptDataStruct.Catch;
    boost::numeric::ublas::matrix<double> Effort       = NLoptDataStruct.Effort;
    boost::numeric::ublas::matrix<double> Exploitation = NLoptDataStruct.Exploitation;
    std::map<int,std::vector<int> > GuildSpecies = NLoptDataStruct.GuildSpecies;
    std::string MSSPMName = "Run " + std::to_string(m_RunNum) + "-1";

    if (m_Quit) {
       throw nlopt::forced_stop();
    }

    if (isAggProd) {
        NumSpeciesOrGuilds = NumGuilds;
        ObsBiomassBySpeciesOrGuilds = NLoptDataStruct.ObservedBiomassByGuilds;
    } else {
        NumSpeciesOrGuilds = NumSpecies;
        ObsBiomassBySpeciesOrGuilds = NLoptDataStruct.ObservedBiomassBySpecies;
    }
    nmfUtils::initialize(EstBiomassSpecies,                   NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(EstBiomassGuilds,                    NumYears,           NumGuilds);
    nmfUtils::initialize(EstBiomassRescaled,                  NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(ObsBiomassBySpeciesOrGuildsRescaled, NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionAlpha,                    NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionBetaSpecies,              NumSpecies,         NumSpecies);
    nmfUtils::initialize(competitionBetaGuilds,               NumSpeciesOrGuilds, NumGuilds);
    nmfUtils::initialize(predation,                           NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(handling,                            NumSpeciesOrGuilds, NumSpeciesOrGuilds);

    extractParameters(NLoptDataStruct, EstParameters,
                      growthRate,carryingCapacity,catchabilityRate,
                      competitionAlpha,competitionBetaSpecies,competitionBetaGuilds,
                      predation,handling,exponent);

    // Calculate carrying capacity for all guilds
    systemCarryingCapacity = 0;
    for (int i=0; i<NumGuilds; ++i) {
        guildK = 0;
        for (unsigned j=0; j<GuildSpecies[i].size(); ++j) {
            guildK += carryingCapacity[GuildSpecies[i][j]];
            systemCarryingCapacity += guildK;
        }
        guildCarryingCapacity.push_back(guildK);
    }

    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        EstBiomassSpecies(0,i) = NLoptDataStruct.ObservedBiomassBySpecies(0,i);
    }
    for (int i=0; i<NumGuilds; ++i) {
        EstBiomassGuilds(0,i)  = NLoptDataStruct.ObservedBiomassByGuilds(0,i); // Remember there's only initial guild biomass data.
    }

    if (NLoptGrowthForm == nullptr) {
        incrementObjectiveFunctionCounter(MSSPMName,-1.0,NLoptDataStruct);
        return -1;
    }
    for (int time=1; time<NumYears; ++time) {
        timeMinus1 = time - 1;
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            EstBiomassVal   = EstBiomassSpecies(timeMinus1,i);
            GrowthTerm      = NLoptGrowthForm->evaluate(i,EstBiomassVal,
                                                     growthRate,carryingCapacity);
            HarvestTerm     = NLoptHarvestForm->evaluate(timeMinus1,i,
                                                      Catch,Effort,Exploitation,
                                                      EstBiomassVal,catchabilityRate);
            CompetitionTerm = NLoptCompetitionForm->evaluate(
                                   timeMinus1,i,EstBiomassVal,
                                   systemCarryingCapacity,
                                   growthRate,
                                   guildCarryingCapacity[guildNum],
                                   competitionAlpha,
                                   competitionBetaSpecies,
                                   competitionBetaGuilds,
                                   EstBiomassSpecies,
                                   EstBiomassGuilds);
            PredationTerm   = NLoptPredationForm->evaluate(
                                   timeMinus1,i,
                                   predation,handling,exponent,
                                   EstBiomassSpecies,EstBiomassVal);

            EstBiomassVal  += GrowthTerm - HarvestTerm - CompetitionTerm - PredationTerm;

//std::cout << "EstBiomassVal: " << EstBiomassVal <<
//             ", g: " << GrowthTerm <<
//             ", h: " << HarvestTerm <<
//             ", c: " << CompetitionTerm <<
//             ", p: " << PredationTerm << std::endl;

            if ((EstBiomassVal < 0) || (std::isnan(std::fabs(EstBiomassVal)))) {
                incrementObjectiveFunctionCounter(MSSPMName,(double)DefaultFitness,NLoptDataStruct);
                return DefaultFitness;
            }

            EstBiomassSpecies(time,i) = EstBiomassVal;

            // update EstBiomassGuilds for next time step
            for (int i=0; i<NumGuilds; ++i) {
                for (unsigned j=0; j<GuildSpecies[i].size(); ++j) {
                    EstBiomassGuilds(time,i) += EstBiomassSpecies(time,GuildSpecies[i][j]);
                }
            }
        } // end i
    } // end time

    // Scale the data
    std::string m_Scaling = NLoptDataStruct.Scaling;
    if (m_Scaling == "Min Max") {
        rescaleMinMax(EstBiomassSpecies, EstBiomassRescaled);
        rescaleMinMax(ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_Scaling == "Mean") {
        rescaleMean(EstBiomassSpecies, EstBiomassRescaled);
        rescaleMean(ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    } else {
//      std::cout << "Error: No Scaling Algorithm detected. Defaulting to Min Max." << std::endl;
        rescaleMinMax(EstBiomassSpecies, EstBiomassRescaled);
        rescaleMinMax(ObsBiomassBySpeciesOrGuilds, ObsBiomassBySpeciesOrGuildsRescaled);
    }

    // Calculate fitness using the appropriate objective criterion
    if (NLoptDataStruct.ObjectiveCriterion == "Least Squares") {

        fitness =  nmfUtilsStatistics::calculateSumOfSquares(
                    EstBiomassRescaled,
                    ObsBiomassBySpeciesOrGuildsRescaled);
    } else if (NLoptDataStruct.ObjectiveCriterion == "Model Efficiency") {

        // Negate the MEF here since the ranges is from -inf to 1, where 1 is best.  So we negate it,
        // then minimize that, and then negate and plot the resulting value.
        fitness = -nmfUtilsStatistics::calculateModelEfficiency(
                    EstBiomassRescaled,
                    ObsBiomassBySpeciesOrGuildsRescaled);
    } else if (NLoptDataStruct.ObjectiveCriterion == "Maximum Likelihood") {
        // The maximum likelihood calculations must use the unscaled data or else the
        // results will be incorrect.
        fitness =  nmfUtilsStatistics::calculateMaximumLikelihoodNoRescale(
                    EstBiomassSpecies,
                    ObsBiomassBySpeciesOrGuilds);
     }

    incrementObjectiveFunctionCounter(MSSPMName,fitness,NLoptDataStruct);

    return fitness;
}


void
NLopt_Estimator::incrementObjectiveFunctionCounter(std::string MSSPMName,
                                                   double fitness,
                                                   Data_Struct NLoptDataStruct)
{
    int unused = -1;

    // Update progress output file
    // RSK - comment out for now, some algorithms yield 0 evals while they're calculating
//    m_NLoptFcnEvals = m_Optimizer.get_numevals();

    ++m_NumObjFcnCalls;
//std::cout << "x,y: " << m_NumObjFcnCalls << "," << fitness << std::endl;
    if (m_NumObjFcnCalls%1000 == 0) {

    writeCurrentLoopFile(MSSPMName,
                         m_NumObjFcnCalls,
                         fitness,
                         NLoptDataStruct.ObjectiveCriterion,
                         unused);
}
//    if (m_NLoptFcnEvals == 0) {
//        if (m_NumObjFcnCalls%10 == 0) {
//            if (NLoptDataStruct.NLoptUseStopAfterIter) {
//                if (m_NumObjFcnCalls <= m_Optimizer.get_maxeval())
//                {
//                    writeCurrentLoopFile(MSSPMName,
//                                         m_NumObjFcnCalls,
//                                         fitness,
//                                         NLoptDataStruct.ObjectiveCriterion,
//                                         unused);
//                }
//            } else {
//                writeCurrentLoopFile(MSSPMName,
//                                     m_NumObjFcnCalls,
//                                     fitness,
//                                     NLoptDataStruct.ObjectiveCriterion,
//                                     unused);
//            }
//        }
//    }
//    else {
//        if (m_NLoptFcnEvals%10 == 0) {
//            if (NLoptDataStruct.NLoptUseStopAfterIter) {
//                if (m_NLoptFcnEvals <= m_Optimizer.get_maxeval())
//                {
//                    writeCurrentLoopFile(MSSPMName,
//                                         m_NLoptFcnEvals,
//                                         fitness,
//                                         NLoptDataStruct.ObjectiveCriterion,
//                                         unused);
//                }
//            } else {
//                writeCurrentLoopFile(MSSPMName,
//                                     m_NLoptFcnEvals,
//                                     fitness,
//                                     NLoptDataStruct.ObjectiveCriterion,
//                                     unused);
//            }
//        }
//    }



}

void
NLopt_Estimator::writeCurrentLoopFile(std::string &MSSPMName,
                                      int         &NumGens,
                                      double      &BestFitness,
                                      std::string &ObjectiveCriterion,
                                      int         &NumGensSinceBestFit)
{
    double adjustedBestFitness; // May need negating if ObjCrit is Model Efficiency
    std::ofstream outputFile(nmfConstantsMSSPM::MSSPMProgressChartFile,
                             std::ios::out|std::ios::app);

    adjustedBestFitness = BestFitness;
    //
    // Model Efficiency is to be maximized instead of minimized.  The
    // best value is 1. Since the code is set up to minimize for Least
    // Squares, I just negated the fitness and ran the minimization code.
    // Now, I just need to negate the fitness again so the plot will
    // show the fitness approaching +1.
    //
    if (ObjectiveCriterion == "Model Efficiency") {
        adjustedBestFitness = -adjustedBestFitness;
    }

    outputFile << MSSPMName   << ", "
               << NumGens     << ", "
               << adjustedBestFitness << ", "
               << NumGensSinceBestFit << std::endl;

    outputFile.close();
}


/*
 *	double dnorm4(double x, double mu, double sigma, int give_log)
 *	      {dnorm (..) is synonymous and preferred inside R}
 *
 *	Compute the density of the normal distribution.
 */

//double
//NLopt_Estimator::dnorm4(double x, double mu, double sigma, int give_log)
//{

//    x = (x - mu) / sigma;
//    x = fabs (x);
//    if (give_log) {
//        return -(M_LN_SQRT_2PI + 0.5 * x * x + log(sigma));
//    }
//    //  M_1_SQRT_2PI = 1 / sqrt(2 * pi)
// //#ifdef MATHLIB_FAST_dnorm
// //    return M_1_SQRT_2PI * exp(-0.5 * x * x) / sigma;
// //#else
//    // more accurate, less fast :
//    if (x < 5) {
//        return M_1_SQRT_2PI * exp(-0.5 * x * x) / sigma;
//    }

//    double x1 = ldexp( R_forceint(ldexp(x, 16)), -16);
//    double x2 = x - x1;
//    return M_1_SQRT_2PI / sigma * ( exp(-0.5 *x1*x1)*exp((-0.5*x2-x1)*x2) );
//#endif
//}




void
NLopt_Estimator::estimateParameters(Data_Struct &NLoptStruct, int RunNum)
{
    int NumEstParameters;
    int NumSubRuns = NLoptStruct.BeesNumRepetitions;
    double fitnessStdDev   = 0;
    std::chrono::_V2::system_clock::time_point startTime = nmfUtils::startTimer();
    std::chrono::_V2::system_clock::time_point startTimeSpecies;
    std::string bestFitnessStr = "TBD";
    std::vector<std::pair<double,double> > ParameterRanges;
    std::string MaxOrMin;

    startTimeSpecies = nmfUtils::startTimer();

//  m_NLoptIters    = 0;
    m_NLoptFcnEvals = 0;
    m_NumObjFcnCalls = 0;
//  m_Counter       = 0;
    m_Quit          = false;
    m_RunNum       += 1;

    // Define forms
    NLoptGrowthForm      = std::make_unique<nmfGrowthForm>(     NLoptStruct.GrowthForm);
    NLoptHarvestForm     = std::make_unique<nmfHarvestForm>(    NLoptStruct.HarvestForm);
    NLoptCompetitionForm = std::make_unique<nmfCompetitionForm>(NLoptStruct.CompetitionForm);
    NLoptPredationForm   = std::make_unique<nmfPredationForm>(  NLoptStruct.PredationForm);

    // Load parameter ranges
    NLoptGrowthForm->loadParameterRanges(     ParameterRanges, NLoptStruct);
    NLoptHarvestForm->loadParameterRanges(    ParameterRanges, NLoptStruct);
    NLoptCompetitionForm->loadParameterRanges(ParameterRanges, NLoptStruct);
    NLoptPredationForm->loadParameterRanges(  ParameterRanges, NLoptStruct);

    NumEstParameters = ParameterRanges.size();
    std::vector<double> lowerBounds(NumEstParameters);
    std::vector<double> upperBounds(NumEstParameters);

    // Initialize the optimizer with the appropriate algorithm
//std::cout << "minimizer: " << NLoptStruct.Minimizer << std::endl;
//std::cout << "m_MinimizerToEnum: " << m_MinimizerToEnum[NLoptStruct.Minimizer] << std::endl;
//std::cout << "NumEstParam: " << NumEstParameters << std::endl;
    m_Optimizer = nlopt::opt(m_MinimizerToEnum[NLoptStruct.Minimizer],NumEstParameters);
//  nlopt::opt opt(m_MinimizerToEnum[NLoptStruct.Minimizer],NumEstParameters);

    // Set parameter bounds for all parameters
    for (int i=0; i<NumEstParameters; ++i) {
        lowerBounds[i] = ParameterRanges[i].first;
        upperBounds[i] = ParameterRanges[i].second;
//std::cout << "Bounds: " << lowerBounds[i] << "," << upperBounds[i] << std::endl;
    }
    m_Optimizer.set_lower_bounds(lowerBounds);
    m_Optimizer.set_upper_bounds(upperBounds);

    // Set starting points for all parameters
    for (int i=0; i<NumEstParameters; ++i) {
        if (lowerBounds[i] == upperBounds[i]) {
            m_Parameters.push_back(lowerBounds[i]);
        } else {
            m_Parameters.push_back(lowerBounds[i] + (upperBounds[i]-lowerBounds[i])/2.0);
        }
    }
    NLoptStruct.Parameters = m_Parameters;

    // Call the appropriate Objective Function
    if (NLoptStruct.ObjectiveCriterion == "Least Squares") {
        MaxOrMin = "minimum";
        m_Optimizer.set_min_objective(objectiveFunction, &NLoptStruct);
    } else if (NLoptStruct.ObjectiveCriterion == "Maximum Likelihood") {
        MaxOrMin = "minimum";
        m_Optimizer.set_min_objective(objectiveFunction, &NLoptStruct);
    } else if (NLoptStruct.ObjectiveCriterion == "Model Efficiency") {
        MaxOrMin = "maximum";
        m_Optimizer.set_max_objective(objectiveFunction, &NLoptStruct);
    }

    // Set Stopping Criteria
    if (NLoptStruct.NLoptUseStopVal) {
        std::cout << "Setting stop fitness value: " << NLoptStruct.NLoptStopVal << std::endl;
        m_Optimizer.set_stopval(NLoptStruct.NLoptStopVal);
    }
    if (NLoptStruct.NLoptUseStopAfterTime) {
        std::cout << "Setting max run time: " << NLoptStruct.NLoptStopAfterTime << std::endl;
        m_Optimizer.set_maxtime(NLoptStruct.NLoptStopAfterTime);
    }
    if (NLoptStruct.NLoptUseStopAfterIter) {
        std::cout << "Setting max num function evaluations: " << NLoptStruct.NLoptStopAfterIter << std::endl;
        m_Optimizer.set_maxeval(NLoptStruct.NLoptStopAfterIter);
    }

    //
    // Run the Optimizer using the previously defined objective function
    //
    nlopt::result result;
    try {
        double minf=0;
        try {
            //------------------------------------------------
            result = m_Optimizer.optimize(m_Parameters, minf);
            //------------------------------------------------
            std::cout << "\nOptimizer return code: " << returnCode(result) << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Exception thrown: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "Error: Unknown error from NLopt_Estimator::estimateParameters m_Optimizer.optimize()" << std::endl;
        }

        std::cout << "Found " + MaxOrMin + " fitness of: " << minf << std::endl;
        for (unsigned i=0; i<m_Parameters.size(); ++i) {
            std::cout << "  Est Param[" << i << "]: " << m_Parameters[i] << std::endl;
        }

        extractParameters(NLoptStruct, &m_Parameters[0],
                          m_EstGrowthRates, m_EstCarryingCapacities,
                          m_EstCatchability, m_EstAlpha,
                          m_EstBetaSpecies, m_EstBetaGuilds,
                          m_EstPredation, m_EstHandling, m_EstExponent);

        createOutputStr(NLoptStruct.TotalNumberParameters,
                        m_Parameters.size(),NumSubRuns,
                        minf,fitnessStdDev,NLoptStruct,bestFitnessStr);

        emit RunCompleted(bestFitnessStr,NLoptStruct.showDiagnosticChart);

    }
    catch (nlopt::forced_stop &e) {
        std::cout << "User terminated application: " << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << "nlopt failed: " << e.what() << std::endl;
    }

    std::string elapsedTimeStr = "Elapsed runtime: " + nmfUtils::elapsedTime(startTime);
    std::cout << elapsedTimeStr << std::endl;

    stopRun(elapsedTimeStr,bestFitnessStr);

//std::cout << "throwing nlopt::forced_stop()" << std::endl;
//    throw nlopt::forced_stop();

}

void
NLopt_Estimator::callback_StopTheOptimizer()
{
   m_Quit = true;
}

void
NLopt_Estimator::createOutputStr(const int&         numTotalParameters,
                                 const int&         numEstParameters,
                                 const int&         numSubRuns,
                                 const double&      bestFitness,
                                 const double&      fitnessStdDev,
                                 const Data_Struct& NLoptStruct,
                                 std::string&       bestFitnessStr)
{
    std::string growthForm      = NLoptStruct.GrowthForm;
    std::string harvestForm     = NLoptStruct.HarvestForm;
    std::string competitionForm = NLoptStruct.CompetitionForm;
    std::string predationForm   = NLoptStruct.PredationForm;

    std::cout << "Est'd Parameters: " << numEstParameters << std::endl;
    std::cout << "Total Parameters: " << numTotalParameters << std::endl;
    std::cout << "Fitness std dev: "  << fitnessStdDev << std::endl;

    // Write to Stop file
    bestFitnessStr  = "Est'd Parameters:&nbsp;"     + std::to_string(numEstParameters);
    bestFitnessStr += "<br>Total Parameters:&nbsp;" + std::to_string(numTotalParameters);

    bestFitnessStr += "<br><br>Number of Runs:&nbsp;&nbsp;&nbsp;" + std::to_string(numSubRuns);
    bestFitnessStr += "<br>Best Fitness (SSE) value of all runs:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + std::to_string(bestFitness);
    bestFitnessStr += "<br>Std dev of Best Fitness values from all runs:&nbsp;&nbsp;" + std::to_string(fitnessStdDev);

    if (growthForm == "Logistic") {
        bestFitnessStr += "<br><br>Initial Parameters:";
        bestFitnessStr += convertValues1DToOutputStr("Carrying Capacity",m_InitialCarryingCapacities,true);
    }
    bestFitnessStr += "<br><br>Estimated Parameters:";
    bestFitnessStr += convertValues1DToOutputStr("Growth Rate",m_EstGrowthRates,false);
    if (growthForm == "Logistic") {
        bestFitnessStr += convertValues1DToOutputStr("Carrying Capacity",m_EstCarryingCapacities,true);
    }

    if (harvestForm == "Effort (qE)") {
        bestFitnessStr += convertValues1DToOutputStr("Catchability",m_EstCatchability,false);
    }
    if (competitionForm == "NO_K") {
        bestFitnessStr += convertValues2DToOutputStr("Competition (alpha)",m_EstAlpha);
    } else if ((competitionForm == "MS-PROD") ||
               (competitionForm == "AGG-PROD"))
    {
        if (competitionForm == "MS-PROD") {
            bestFitnessStr += convertValues2DToOutputStr("Competition (beta::species)",m_EstBetaSpecies);
        }
        bestFitnessStr += convertValues2DToOutputStr("Competition (beta::guilds)",m_EstBetaSpecies);
    }

    if ((predationForm == "Type I")  ||
        (predationForm == "Type II") ||
        (predationForm == "Type III"))
    {
        bestFitnessStr += convertValues2DToOutputStr("Predation (rho)",m_EstPredation);
    }
    if ((predationForm == "Type II") ||
        (predationForm == "Type III"))
    {
        bestFitnessStr += convertValues2DToOutputStr("Handling",m_EstHandling);
    }
    if (predationForm == "Type III")
    {
        bestFitnessStr += "<br>&nbsp;&nbsp;";
        bestFitnessStr += convertValues1DToOutputStr("Predation Exponent",m_EstExponent,false);
    }
}


std::string
NLopt_Estimator::convertValues1DToOutputStr(const std::string& label,
                                            const std::vector<double> &Values,
                                            const bool& includeTotal)
{
    double val;
    double totalVal = 0;
    std::string bestFitnessStr = "";

    bestFitnessStr += "<br>&nbsp;&nbsp;" + label + ":&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    for (unsigned i=0; i<Values.size(); ++i) {
        val = Values[i];
        bestFitnessStr += nmfUtils::convertToScientificNotation(val) + "&nbsp;&nbsp;";
        totalVal += val;
    }
    if (includeTotal) {
        bestFitnessStr += "<br>&nbsp;&nbsp;Total " + label + ":&nbsp;&nbsp;" +
                nmfUtils::convertToScientificNotation(totalVal);
    }

    return bestFitnessStr;
}


std::string
NLopt_Estimator::convertValues2DToOutputStr(const std::string& label,
                                            const boost::numeric::ublas::matrix<double> &matrix)
{
    std::string bestFitnessStr = "";

    for (unsigned i=0; i<matrix.size1(); ++i) {
        bestFitnessStr += "<br>&nbsp;&nbsp;";
        for (unsigned j=0; j<matrix.size2(); ++j) {
            if ((i == 0) && (j == 0)) {
                bestFitnessStr += "&nbsp;&nbsp;" + label + ":<br>&nbsp;&nbsp;";
            }
            bestFitnessStr += "&nbsp;&nbsp;&nbsp;" + nmfUtils::convertToScientificNotation(matrix(i,j));
        }
    }

    return bestFitnessStr;
}


void
NLopt_Estimator::stopRun(const std::string &elapsedTimeStr,
                         const std::string &fitnessStr)
{
std::cout << "StopRun..." << std::endl;
    std::ofstream outputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
    outputFile << "Stop" << std::endl;         // cmd
    outputFile << "" << std::endl;             // run name
    outputFile << elapsedTimeStr << std::endl; // elapsed time
    outputFile << fitnessStr << std::endl;     // fitness

    outputFile.close();
std::cout << "Run Stopped" << std::endl;
}


void
NLopt_Estimator::getEstGrowthRates(std::vector<double>& estGrowthRates)
{
    estGrowthRates = m_EstGrowthRates;
}

void
NLopt_Estimator::getEstCarryingCapacities(std::vector<double>& estCarryingCapacities)
{
    estCarryingCapacities = m_EstCarryingCapacities;
}

void
NLopt_Estimator::getEstCatchability(std::vector<double> &estCatchability)
{
    estCatchability = m_EstCatchability;
}

void
NLopt_Estimator::getEstCompetitionAlpha(boost::numeric::ublas::matrix<double> &estInteraction)
{
    estInteraction = m_EstAlpha;
}

void
NLopt_Estimator::getEstCompetitionBetaSpecies(boost::numeric::ublas::matrix<double> &estCompSpecies)
{
    estCompSpecies = m_EstBetaSpecies;
}

void
NLopt_Estimator::getEstCompetitionBetaGuilds(boost::numeric::ublas::matrix<double> &estCompGuilds)
{
    estCompGuilds = m_EstBetaGuilds;
}

void
NLopt_Estimator::getEstPredation(boost::numeric::ublas::matrix<double> &estPredation)
{
    estPredation = m_EstPredation;
}

void
NLopt_Estimator::getEstHandling(boost::numeric::ublas::matrix<double> &estHandling)
{
    estHandling = m_EstHandling;
}

void
NLopt_Estimator::getEstExponent(std::vector<double> &estExponent)
{
    estExponent = m_EstExponent;
}


std::string
NLopt_Estimator::returnCode(int result)
{
    std::string retv;

    switch (result) {
        case 1:
            retv = "NLOPT_SUCCESS";
            break;
        case 2:
            retv = "NLOPT_STOPVAL_REACHED";
            break;
        case 3:
            retv = "NLOPT_FTOL_REACHED";
            break;
        case 4:
            retv = "NLOPT_XTOL_REACHED";
            break;
        case 5:
            retv = "NLOPT_MAXEVAL_REACHED";
            break;
        case 6:
            retv = "NLOPT_MAXTIME_REACHED";
            break;
        case -1:
            retv = "NLOPT_FAILURE";
            break;
        case -2:
            retv = "NLOPT_INVALID_ARGS";
            break;
        case -3:
            retv = "NLOPT_OUT_OF_MEMORY";
            break;
        case -4:
            retv = "NLOPT_ROUNDOFF_LIMITED";
            break;
        case -5:
            retv = "NLOPT_FORCED_STOP";
            break;
    }
    return retv;
}


void
NLopt_Estimator::rescaleMinMax(const boost::numeric::ublas::matrix<double> &matrix,
                                     boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = matrix.size1();
    int numSpecies = matrix.size2();
    double den;
    double minVal;
    double maxVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> tmp(numYears,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        for (int time=0; time<numYears; ++time) {
            tmp[time] = matrix(time,species);
        }
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
    }

    // Rescale each column of the matrix with (x - min)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        for (int time=0; time<numYears; ++time) {
            rescaledMatrix(time,species) = (matrix(time,species) - minVal) / den;  // min max normalization
        }
    }
}


void
NLopt_Estimator::rescaleMean(const boost::numeric::ublas::matrix<double> &matrix,
                                   boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = matrix.size1();
    int numSpecies = matrix.size2();
    double den;
    double minVal;
    double maxVal;
    double avgVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> avgValues(numSpecies,0);
    std::vector<double> tmp(numYears,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        avgVal = 0;
        for (int time=0; time<numYears; ++time) {
            tmp[time] = matrix(time,species);
            avgVal += tmp[time];
        }
        avgVal /= numYears;
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
        avgValues[species] = avgVal;
    }

    // Rescale each column of the matrix with (x - ave)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        avgVal = avgValues[species];
        for (int time=0; time<numYears; ++time) {
            rescaledMatrix(time,species) = (matrix(time,species) - avgVal) / den; // mean normalization
        }
    }
}
