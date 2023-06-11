#ifndef FOURSUITES_H
#define FOURSUITES_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QObject>
#include <QEventLoop>

// TODO: Make a dict in the style of
// { "Door Name" : "id" , ...} and hook it to get_doors()
//
// You can get the IDs for your building from the doors/accessible endpoint:
// https://developer.4suiteshq.com/#/accessible-get
enum Doors
{
    MAIN_ENTRANCE = 1616,
    MALL = 1618,
    BIKE_PARKING = 1619,
    METER_CABINETS = 1624
};

QString joinUrls(const QString &baseUrl, const QString &relativeUrl);

class FourSuites : public QObject
{
    Q_OBJECT
public:
    explicit FourSuites(QObject *parent = nullptr);
    FourSuites(QString &token_);
    void authenticate();
    void get_doors();
    void open_door(Doors d);
    void set_credentials(const QString &email, const QString &password);
    void extract_token(QByteArray &jsonData);

private:
    inline static const QString base_url = "https://api.4suites.nl/v1/";
    inline static const QString login_url = joinUrls(base_url, "auth/login");
    inline static const QString accessible_doors_url = joinUrls(base_url, "doors/accessible");
    inline static const QString open_doors_url = joinUrls(base_url, "doors/");

    QString email_;
    QString password_;

    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QString token_;
};

#endif // FOURSUITES_H
