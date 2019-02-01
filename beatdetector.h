#ifndef BEATDETECTOR_H
#define BEATDETECTOR_H

#include <QObject>
#include <QVector>
#include <QDebug>

#define HISTORY_SAMPLE_SIZE 43

//https://www.parallelcube.com/2018/03/30/beat-detection-algorithm/
class beatDetector : public QObject
{
    Q_OBJECT

    QVector<double> m_EnergyHistory;
    int m_lowerRange;
    int m_upperRange;

public:
    explicit beatDetector(QObject *parent = nullptr);
    void setRange(int const &lower, int const &upper);

signals:
    void beatFound();
public slots:

    void detectBeat(QVector<double> const &spectrum);


};

#endif // BEATDETECTOR_H
