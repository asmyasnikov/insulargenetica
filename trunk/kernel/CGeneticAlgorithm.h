/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser GPL as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser GPL for more details.
You should have received a copy of the GNU Lesser GPL
in file LICENSE along with this program.  If not, see
<http://www.gnu.org/licenses/>
I draw your attention to the fact that the library InsularGenetica
is registered Russian Agency for Patents and Trademarks (ROSPATENT),
which issued the "Certificate of official registration of computer
programs for № FIXME FIXME FIXME FIXME". A copy of the certificate
of registration is presented in the file CERTIFICATE in the root
of the project. It does not impose on the end developers / users,
no additional restrictions other than those prescribed by GNU Lesser
GPL, while maintaining information about developer this library.
****************************************************************/
/**
 * @file    CGeneticAlgorithm.h
 * @brief   Class CGeneticAlgorithm implement business logic of
 *          genetic algorithm, provide control of genetic operators
 * @date    05/03/2009
 * @version 3.3
**/
#ifndef GENETIC_ALGORITHM_FACTORY_H_INCLUDED
#define GENETIC_ALGORITHM_FACTORY_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qthread.h>
    #include <qmutex.h>
    #include <qmap.h>
    #include <qlist.h>
#else
    #include <QtCore/QThread>
    #include <QtCore/QMutex>
    #include <QtCore/QMap>
    #include <QtCore/QList>
#endif
#include "../idl/IGeneticOperator.h"
#include "../idl/ICancelService.h"
#include "../include/CPopulation.h"
class QString;
namespace InsularGenetica
{
    class COperatorStatistics;
    class IFitness;
    class ISelection;
    class IGrouping;
    class IReproduction;
    class IMutation;
    class IAccepting;
    Q_DECL_EXPORT struct CGeneticAlgorithm : public QThread,
                                     virtual public Interface
    {
        enum ResultCode
        {
            NoCode              = -1, // Algorithm not start of evalutions
            Process             =  0, // Evalutions
            MaxTimes            =  1, // Was got maximum time of evolution
            DeadLock            =  2, // Was got dead lock
            GoodHomogeneity     =  3, // Was got 95-% homogeineity
            Cancel              =  4  // Cancel evalutions
        };
        /**
         * @brief Static method of setting population size
         * @param size - size of population
        **/
        static void setPopulationSize(unsigned int size);
        /**
         * @brief Constructor
         * @param operators - genetic operators
         * @param minutes   - maximum minutes of evalutions
        **/
        CGeneticAlgorithm(ICancelService*cancel_service,
#if QT_VERSION < 0x040000
                          QValueList<IGeneticOperator*> operators,
#else
                          QList<IGeneticOperator*>      operators,
#endif
                          unsigned long                 minutes = 168);
        /**
         * @brief Destructor
        **/
        ~CGeneticAlgorithm();
        /**
         * @brief   Getting current code of algorithm
         * @return  current code of algorithm
        **/
        ResultCode result() const;
        /**
         * @brief   Setting neighbour island (genetic algorithm)
         *          This need for "ring" topology
         * @param   neighbour - genetic algorithm
        **/
        void setNeighbourAlgorithm(CGeneticAlgorithm*neighbour);
        /**
         * @brief   Getting best chromosomes
         * @param   size - population size
         * @return  size or less then size chromsomes in population
        **/
        CPopulation getBestChromosomes(int size) const;
        /**
         * @brief Main loop of algorithm
        **/
        void run();
        /**
         * @brief Cancelling evalutions
        **/
        void cancel();
    private:
        /**
         * @brief Initialization of genetic operator frequencies
         *        and parse operators by types
         * @param map       - operators by types
         * @param operators - genetic operators
        **/
        template<typename T>
        void initLibraries(QMap<T*, COperatorStatistics>&   map,
#if QT_VERSION < 0x040000
                           QValueList<IGeneticOperator*>    operators
#else
                           QList<IGeneticOperator*>         operators
#endif
                          );
        /**
         * @brief   Getting genetic operator by current frequency of
         *          genetic operator
         * @param   map - genetic operators for choise
         * @return  operator pointer or NULL (if get error)
        **/
        template<typename T>
        inline T* getGeneticOperator(QMap<T*, COperatorStatistics>&map) const;
        /**
         * @brief Update of genetic operators frequencies
         * @param map       - group of genetic operators
         * @param op        - current genetic operator
         * @param used_time - time of algorithm work with operator op
         * @param best      - number of good chromosomes returned operator op
        **/
        template<typename T>
        inline void
        recalcStatisticalFrequency( QMap<T*, COperatorStatistics>& map,
                                    const T*                       op,
                                    double                         used_time,
                                    unsigned int                   best);
    private:
        ///<! Group of selection genetic operators
        QMap <ISelection*   , COperatorStatistics> m_selections;
        ///<! Group of grouping genetic operators
        QMap <IGrouping*    , COperatorStatistics> m_groupings;
        ///<! Group of reproduction genetic operators
        QMap <IReproduction*, COperatorStatistics> m_reproductions;
        ///<! Group of mutation genetic operators
        QMap <IMutation*    , COperatorStatistics> m_mutations;
        ///<! Group of accepting genetic operators
        QMap <IAccepting*   , COperatorStatistics> m_acceptings;
        ///<! maximum time
        unsigned long                              m_minutes;
        ///<! Code of algorithm
        ResultCode                                 m_result_code;
        ///<! Size of population
        static unsigned int                        m_population_size;
        ///<! Main population
        CPopulation                                m_population;
        ///<! Neighbour genetic algorithm
        CGeneticAlgorithm*                         m_neighbour;
        ///<! Mutex
        mutable QMutex                             m_mutex;
        ///<! Cancel service pointer
        ICancelService*                            m_cancel_service;
    };
};
#endif // GENETIC_ALGORITHM_FACTORY_H_INCLUDED
