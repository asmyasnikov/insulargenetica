/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser GPL as publishedby
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details. You should have received a
copy of the GNU Lesser GPL in file LICENSE along with this
program.  If not, see <http://www.gnu.org/licenses/lgpl.txt>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for N FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers/users, no
additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CGeneticController.h
 * @brief   Class CGeneticController provide controller of
 *          start and end of evalutions, organizate islands
 *          cooperation
 * @author  Aleksey Myasnikov
 * @date    23/03/2009
 * @version 3.3
**/
#ifndef C_GENETIC_CONTROLLER_H_INCLUDED
#define C_GENETIC_CONTROLLER_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qthread.h>
    #include <qvaluelist.h>
#else
    #include <QtCore/QThread>
    #include <QtCore/QSet>
#endif
#include "CPopulation.h"
#include "../idl/ICancelService.h"
namespace InsularGenetica
{
    class CGeneticAlgorithm;
    class IGeneticOperator;
    /**
     * @brief Main class of insulargenetica genetic algorithm
    **/
    struct Q_DECL_EXPORT CGeneticController : public ICancelService
    {
        /**
         * @brief Destructor
        **/
        ~CGeneticController(void);
        /**
         * @brief Getting result of evalutions
        **/
        CPopulation getBestSolutions(int size);
        /**
         * @brief Static method of evaluate
         * @param fitness - fitness function,
         * @param chromosom - size of chromosome,
         * @param population - size of population,
         * @param minutes - limit of evalution time [minutes],
         * @param island - number of islands.
         *                 If (island = -1) number of islands automatically
         *                 detecting by 2*number of processors in system
         * @param cancel_service - service of cancelling evalutions
        **/
        static CPopulation calc(IFitness*       fitness,
                                unsigned int    chromosom,
                                unsigned int    population = 128,
                                unsigned int    minutes = 60,
                                int             island = -1,
                                ICancelService* cancel_service = NULL);
        /**
         * @brief Static method of constructing calculator
         * @param fitness - fitness function,
         * @param chromosom - size of chromosome,
         * @param population - size of population,
         * @param minutes - limit of evalution time [minutes],
         * @param island - number of islands.
         *                 If (island = -1) number of islands automatically
         *                 detecting by 2*number of processors in system
         * @param cancel_service - service of cancelling evalutions
        **/
        static CGeneticController*getCalculator(IFitness*       fitness,
                                                unsigned int    chromosom,
                                                unsigned int    population = 128,
                                                unsigned int    minutes = 60,
                                                int             island = -1,
                                                ICancelService* cancel_service = NULL);
        /**
         * @brief   This method provides canceling of evaluations.
         * @return  cancel status
        **/
        bool isCanceled();
    private:
        /**
         * @brief Constructor
         * @param fitness - fitness function,
         * @param population - size of population,
         * @param minutes - limit of evalution time [minutes],
         * @param island - number of islands.
         *                 If (island = -1) number of islands automatically
         *                 detecting by 2*number of processors in system
         * @param cancel_service - service of cancelling evalutions
        **/
        CGeneticController(IFitness*       fitness,
                           unsigned int    population,
                           unsigned int    island,
                           unsigned long   minutes,
                           ICancelService* cancel_service = NULL);
        /**
         * @brief Calculate method
        **/
        void calculate();
    private:
        enum OperatorType
        {
            Accepting,
            Grouping,
            Reproduction,
            Selection,
            Mutation
        };
#if QT_VERSION < 0x040000
        ///<! Single islands
        QValueList <CGeneticAlgorithm*>m_algorithms;
        ///<! Genetic operators
        QValueList <IGeneticOperator*> m_operators;
#else
        ///<! Single islands
        QList <CGeneticAlgorithm*>     m_algorithms;
        ///<! Genetic operators
        QList <IGeneticOperator*>      m_operators;
#endif
        ///<! Result population
        CPopulation                    m_best_solutions;
        ///<! Size of result population
        unsigned long                  m_best_solutions_size;
        ///<! Limit of evalution time [minutes]
        unsigned long                  m_minutes;
        ///<! Fitness function
        IFitness*                      m_function;
        ///<! Cancel service pointer
        ICancelService*                m_cancel_service;
        ///<! Calculate status
        bool                           m_is_calculate;
        ///<! Locker
        QMutex                         m_mutex;
    };
};
#endif // C_GENETIC_CONTROLLER_H_INCLUDED
