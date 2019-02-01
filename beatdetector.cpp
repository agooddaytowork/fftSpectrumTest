#include "beatdetector.h"
#include "QDateTime"

beatDetector::beatDetector(QObject *parent) : QObject(parent), m_lowerRange(0), m_upperRange(512)
{
    m_EnergyHistory.clear();
}


void beatDetector::setRange(const int &lower, const int &upper)
{
    m_lowerRange = lower;
    m_upperRange = upper;
}


void beatDetector::detectBeat(QVector<double> const &spectrum)
{
    double instaneEnergy =0.0;
    double averageEnergy =0.0;
    double varianceHisotry =0.0;
    double threshold = 0.0;

    // CALCULATE INSTANT ENERGY
    for(int i = m_lowerRange; i <= m_upperRange; i++)
    {
        instaneEnergy += spectrum[i];
    }

    instaneEnergy = instaneEnergy / static_cast<double>(m_upperRange-m_lowerRange);

    if(m_EnergyHistory.count() < HISTORY_SAMPLE_SIZE)
    {
        m_EnergyHistory.append(instaneEnergy);
        return;
    }
    else {
        m_EnergyHistory.removeFirst();
        m_EnergyHistory.append(instaneEnergy);
    }


    // CALCULATE AVERAGE ENERGY of the last second
    for(int i = 0; i < HISTORY_SAMPLE_SIZE; i++)
    {
        averageEnergy += m_EnergyHistory[i];
    }


    averageEnergy = averageEnergy / static_cast<double>(HISTORY_SAMPLE_SIZE);

    // CALCULATE THE VARIANCE of the energy history


    for(int i = 0; i < HISTORY_SAMPLE_SIZE; i++)
    {
        double invidiualVariance = (m_EnergyHistory[i] - averageEnergy) * (m_EnergyHistory[i] - averageEnergy);

        varianceHisotry +=  invidiualVariance;
    }
    varianceHisotry = varianceHisotry/ static_cast<double>(HISTORY_SAMPLE_SIZE);

 // CALCULATE THRESHOLD

    threshold = 1.25* varianceHisotry +1.6;
//    threshold = 1.5 * averageEnergy;
//      threshold = (15 * varianceHisotry) +3;

 // Make comparison

//  qDebug() << "E: " << instaneEnergy << " A: " << averageEnergy << " Var: " << varianceHisotry << " TH: " << threshold << "TH*A " << averageEnergy*threshold;
    if(instaneEnergy > threshold*averageEnergy && varianceHisotry > 5e-6)
    {
        static qint64 previousTime =0;

           emit beatFound();
            previousTime = QDateTime::currentMSecsSinceEpoch();




    }

}
