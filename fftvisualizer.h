#ifndef FFTVISUALIZER_H
#define FFTVISUALIZER_H

#include <QObject>
#include <QVector>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QAudioEncoderSettings>
#include <QDebug>
#include <QObject>
#include "fftcalc.h"
#include "fft.h"
#include <QAudioRecorder>
#include "beatdetector.h"
#include "beatestimation.h"


class fftVisualizer : public QObject
{
    Q_OBJECT

    FFTCalc *m_fftCalculator;
    QAudioProbe *m_audioProbe;
    QAudioRecorder *m_recorder;
    QVector<double> sample;
    QVector<double> B;
    QVector<double> E;
    QVector<double> H0;
    // output vector with spectrum
    QVector<double> m_spectrum;

    double m_LevelLeft, m_LevelRight;
    beatDetector *m_beatDetectoKick;
    beatDetector *m_beatDetectorSnare;
    beatEstimation *m_beatEstimation;


public:
    explicit fftVisualizer(QObject *parent = nullptr);

    void start();
    void stop();


    Q_INVOKABLE double getSubandInstantEnergy(const int &subBandNo);
private slots:
   void processAudioBuffer(QAudioBuffer buffer);
   void spectrumCalculated(QVector<double> spectrum);

signals:

   void newInstantEnergyAvailable();
   void beatDetected(int n);
   void clearBeatDetect();

public slots:



};

#endif // FFTVISUALIZER_H
