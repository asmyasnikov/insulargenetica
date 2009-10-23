/****************************************************************
Copyright (C) 2009 Aleksey Myasnikov
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
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
 * @file    CFitnessSafeThreadFunction.h
 * @brief   Class CFitnessSafeThreadFunction implement safe-thread
 *          fitness function with user-value non safe-thread implementation
 * @date    23/03/2009
 * @version 3.3
**/
#ifndef C_FITNESS_SAFE_THREAD_FUNCTION_H_INCLUDED
#define C_FITNESS_SAFE_THREAD_FUNCTION_H_INCLUDED
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qmutex.h>
#else
    #include <QtCore/QMutex>
#endif
#include "../idl/IFitness.h"
namespace InsularGenetica
{
    /**
     * @brief   Class CFitnessSafeThreadFunction implement safe-thread
     *          fitness function with user-value non safe-thread implementation
    **/
    struct CFitnessSafeThreadFunction : public IFitness
    {
        /**
         * @brief Constructor
         * @param fitness - main fitness function
        **/
        CFitnessSafeThreadFunction(IFitness*fitness)
        {
            m_fitness = fitness;
        };
        ~CFitnessSafeThreadFunction()
        {
            m_fitness = NULL;
        };
        /**
         * @brief   Getting name of fitness function
         * @return  Name of fitness function
        **/
        const QString name() const
        {
            QMutexLocker locker(&m_mutex);
            return m_fitness->name();
        };
        /**
         * @brief   Getting number of fitness function calling
         * @return  Number of fitness function calling
        **/
        unsigned int count() const
        {
            QMutexLocker locker(&m_mutex);
            return m_fitness->count();
        };
        /**
         * @brief   Method of comparing two chromosomes
         * @param   chr1 - first chromosome
         * @param   chr2 - second chromosome
         * @return  true, if first chromosome is better
         *          of second chromosome
        **/
        virtual bool compare(const CChromosome& chr1,
                             const CChromosome& chr2) const
        {
            Q_ASSERT(m_fitness);
            QMutexLocker locker(&m_mutex);
            return m_fitness->compare(chr1,chr2);
        };
    protected:
        /**
         * @brief   Method of function value calculating
         * @return  value of fitness function
        **/
        virtual double calc(const CChromosome& chr) const
        {
            return 0.;
        };
    private:
        IFitness*m_fitness;
        mutable QMutex   m_mutex;
    };
};
#endif // C_FITNESS_SAFE_THREAD_FUNCTION_H_INCLUDED
