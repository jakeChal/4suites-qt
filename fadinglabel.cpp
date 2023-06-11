#include "fadinglabel.h"
#include <QTimerEvent>
#include <QTimer>

FadingLabel::FadingLabel(QWidget *parent) : QLabel(parent)
{
    m_executedFade = 0;
    m_opacityAnimation = new QPropertyAnimation(this);
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0);
    this->setGraphicsEffect(m_opacityEffect);
    m_opacityAnimation->setTargetObject(m_opacityEffect);
    m_opacityAnimation->setPropertyName("opacity");

    m_from0to1 = true;
    connect(m_opacityAnimation, &QPropertyAnimation::finished, this, &FadingLabel::fadingFinished);
}

void FadingLabel::setFadingOptions(int duration_msec, int interval_msec, QEasingCurve curve_type)
{
    m_durationMsec = duration_msec;
    m_intervalMsec = interval_msec;
    m_curveType = curve_type;

    m_opacityAnimation->setDuration(m_durationMsec);
    m_opacityAnimation->setEasingCurve(m_curveType);
}

void FadingLabel::setDisabled()
{
    killTimer(m_fadingTimer);
    m_opacityAnimation->stop();
    m_opacityEffect->setOpacity(0);
}

void FadingLabel::startFading(int num_fade)
{
    m_numFade = num_fade;
    m_fadingTimer = startTimer(m_intervalMsec);
    m_from0to1 = true;
    fade();
}

FadingLabel::~FadingLabel()
{
}

void FadingLabel::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_fadingTimer)
        fade();
}

void FadingLabel::fade()
{
    if (m_from0to1)
    {
        m_opacityEffect->setOpacity(0);
        m_opacityAnimation->setStartValue(0);
        m_opacityAnimation->setEndValue(1.0);
        m_from0to1 = false;
    }
    else
    {
        m_opacityEffect->setOpacity(1.0);
        m_opacityAnimation->setStartValue(1.0);
        m_opacityAnimation->setEndValue(0);
        m_from0to1 = true;
    }
    m_opacityAnimation->start();
}

void FadingLabel::fadingFinished()
{
    if (m_from0to1)
        fade();
    else
    {
        if (m_numFade != -1)
        {
            m_executedFade++;
            if (m_executedFade >= m_numFade)
            {
                this->setDisabled();
                //                killTimer(m_fadingTimer);
                //                m_executedFade=0;
            }
        }
    }
}
