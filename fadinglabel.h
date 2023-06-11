#ifndef FADINGLABEL_H
#define FADINGLABEL_H
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
class FadingLabel : public QLabel
{
public:
    FadingLabel(QWidget *parent = 0);
    void startFading(int num_fade = -1);
    ~FadingLabel();
    void setFadingOptions(int duration_msec, int interval_msec, QEasingCurve curve_type);
    void setDisabled();

private:
    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_opacityAnimation;
    int m_fadingTimer;
    void timerEvent(QTimerEvent *e);
    int m_durationMsec;
    int m_intervalMsec;
    QEasingCurve m_curveType;
    void fade();
    bool m_from0to1;
    int m_numFade;
    int m_executedFade;
private slots:
    void fadingFinished();
};
#endif // FADINGLABEL_H
