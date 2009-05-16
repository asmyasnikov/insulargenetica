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
 * @file    main.cpp
 * @brief   Основной файл запуска алгоритма, когда алгоритм собран как исполняемый файл
 * @date    04/03/2009
 * @version 1.15
**/

#include <iostream>
#include "../../include/export.h"
#include "../../include/CPopulation.h"
#include "../../include/CFitnessHelper.h"
#include "../../include/CChromosome.h"
#include "../../include/CGeneticController.h"
#include <qglobal.h>
#if QT_VERSION < 0x040000
    #include <qapplication.h>
    #include <qlibrary.h>
    #include <qdir.h>
    #include <qfile.h>
    #include <qtextstream.h>
    #include <qtextcodec.h>
#else
    #include <QtCore/QCoreApplication>
    #include <QtCore/QLibrary>
    #include <QtCore/QDir>
    #include <QtCore/QFile>
    #include <QtCore/QTextStream>
    #include <QtCore/QTextCodec>
#endif
#include <math.h>

using namespace InsularGenetica;

int main(int argc, char**argv)
{
    srand(clock() % 128);
#if QT_VERSION < 0x040000
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif
#ifdef Q_OS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM-866"));
#endif
    QDir dir("fitness");
    dir.setFilter(QDir::Files);
#if QT_VERSION < 0x040000
    dir.setNameFilter("*.so *.dll");
#else
    QStringList filters;
    filters.append("*.so");
    filters.append("*.dll");
    dir.setNameFilters(filters);
#endif
    QStringList list = dir.entryList();
#if QT_VERSION < 0x040000
    QValueList<IFitness*> m_fitness_modules;
    QValueList<QLibrary*> m_fitness_libraries;
#else
    QList<IFitness*> m_fitness_modules;
    QList<QLibrary*> m_fitness_libraries;
#endif
    for (int i = 0; i < list.size(); ++i)
    {
#if QT_VERSION < 0x040000
        QLibrary*lib = new QLibrary(dir.absFilePath(*list.at(i)));
#else
        QLibrary*lib = new QLibrary(dir.absoluteFilePath(list.at(i)));
#endif
        if(!lib->isLoaded() && !lib->load()) continue;
        typedef Interface*(*CreateModule)(void);
        CreateModule createModule = (CreateModule)(lib->resolve("CreateModule"));
        if(createModule)
        {
            Interface*iface = createModule();
            if(iface)
            {
                IFitness* module = dynamic_cast<IFitness*>(iface);
                if(module)
                {
                    m_fitness_modules.append(module);
                    m_fitness_libraries.append(lib);
                    continue;
                }else{
                    qDebug("createModule() return null");
                    delete iface;
                }
            }
        }else{
#if QT_VERSION < 0x040000
            qDebug("createModule() in %s not found", (*list.at(i)).local8Bit().data());
#else
            qDebug("createModule() in %s not found", list.at(i).toLocal8Bit().data());
#endif
        }
        lib->unload();
        delete lib;
    }
    Q_ASSERT(m_fitness_modules.size());
    std::cout << QObject::trUtf8("Select function from list:")
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    for(int i = 0; i < m_fitness_modules.size(); i++)
    {
        std::cout << QObject::trUtf8("%1. Function \'%2\'")
                     .arg(i)
#if QT_VERSION < 0x040000
                     .arg((*m_fitness_modules.at(i))->name())
                     .local8Bit()
#else
                     .arg(m_fitness_modules.at(i)->name())
                     .toLocal8Bit()
#endif
                     .data() << std::endl;
    }
    int fid = m_fitness_modules.size();
    while(fid >= m_fitness_modules.size())
    {
        std::cout << QObject::trUtf8("Select number of function [0..%1] : ")
                     .arg(m_fitness_modules.size()-1)
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> fid;
    }
    int chrsize = 0;
    while(chrsize < 2 || chrsize > 1024)
    {
        std::cout << QObject::trUtf8("Enter length of chromosome [2..1024] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> chrsize;
    }
    int popsize = 0;
    while(popsize < 2 || popsize > 1024)
    {
        std::cout << QObject::trUtf8("Enter size of population [2..1024] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> popsize;
    }
    int limit = 0;
    while(limit < 1 || limit > 10080)
    {
        std::cout << QObject::trUtf8("Enter maximum time of evolution, minutes [1..10080] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> limit;
    }
    int island = -1;
    while(island < 0 || island > 512)
    {
        std::cout << QObject::trUtf8("Enter number of islands [0..512] : ")
#if QT_VERSION < 0x040000
                     .local8Bit()
#else
                     .toLocal8Bit()
#endif
                     .data();
        std::cin  >> island;
    }
#if QT_VERSION < 0x040000
    IFitness*f = *m_fitness_modules.at(fid);
#else
    IFitness*f = m_fitness_modules.at(fid);
#endif
    CGeneticController*calculator = CGeneticController::getCalculator(f,chrsize,popsize,limit,island);
    CPopulation best = calculator->getBestSolutions(3);
    delete calculator;
    std::cout << QObject::trUtf8("Average fitness of population = %1")
                 .arg(best.getAverageFitness())
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    std::cout << QObject::trUtf8("Population homogeneity = %1")
                 .arg(best.getHomogeneity())
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    std::cout << QObject::trUtf8("Population pseudo-homogeneity = %1")
                 .arg(best.getHomogeneity(true))
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data() << std::endl;
    InsularGenetica::CFitnessHelper *helper = dynamic_cast<InsularGenetica::CFitnessHelper*>(f);
    for(int j = 0; j < popsize && j < 3; j++)
    {
        CChromosome chr = best.getChromosome(j);
        if(helper)
        {
            QString arguments;
#if QT_VERSION < 0x040000
            QValueList<double>phenotype = helper->genotype2phenotype(chr);
            for(QValueList<double>::iterator i = phenotype.begin();
                i != phenotype.end(); i++)
            {
                double value = *i;
#else
            QList<double>phenotype = helper->genotype2phenotype(chr);
            foreach(double value, phenotype)
            {
#endif
                if(!arguments.isEmpty()) arguments.append(";");
                arguments.append(QString::number(value));
            }
            qDebug( "f(%s)=%f",
                    arguments.toLocal8Bit().data(),
                    chr.fitness());
        }else{
            char* code = new char[CChromosome::size()+1];
            for(unsigned int i = 0; i < CChromosome::size(); ++i)
            {
                code[i] = chr.getGene(i) ? '1' : '0';
            }
            code[CChromosome::size()] = '\0';
            qDebug( "f(%s)=%f",
                    code,
                    chr.fitness());
            delete[] code;
        }
    }
    for(int i = 0; i < m_fitness_modules.size(); i++)
    {
#if QT_VERSION < 0x040000
        delete (*m_fitness_modules.at(i));
#else
        delete m_fitness_modules.at(i);
#endif
    }
    m_fitness_modules.clear();
    for(int i = 0; i < m_fitness_libraries.size(); i++)
    {
#if QT_VERSION < 0x040000
        (*m_fitness_libraries.at(i))->unload();
        delete (*m_fitness_libraries.at(i));
#else
        m_fitness_libraries.at(i)->unload();
        delete m_fitness_libraries.at(i);
#endif
    }
    m_fitness_libraries.clear();
    qDebug("--------------------------------------------------------------");
    std::cout << QObject::trUtf8("Enter \"ok\" to exit: ")
#if QT_VERSION < 0x040000
                 .local8Bit()
#else
                 .toLocal8Bit()
#endif
                 .data();
    char chr;
    std::cin >> chr;
}
