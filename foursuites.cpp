#include "foursuites.h"

void FourSuites::extract_token(QByteArray &jsonData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    // Check if the JSON data is valid
    if (!jsonDoc.isNull())
    {
        // Extract the root JSON object
        QJsonObject rootObject = jsonDoc.object();

        // Extract the nested "data" object
        QJsonValue dataValue = rootObject.value("data");
        if (dataValue.isObject())
        {
            QJsonObject dataObject = dataValue.toObject();
            // Extract the value of the nested key "access_token"
            QJsonValue tokenValue = dataObject.value("access_token");
            if (tokenValue.isString())
            {
                QString token = tokenValue.toString();
                qDebug() << "Token: " << token;
                token_ = token;
            }
        }
    }
}

FourSuites::FourSuites(QObject *parent) : QObject(parent)
{ // Create the network manager
    networkManager = new QNetworkAccessManager(this);
}

void FourSuites::set_credentials(const QString &email, const QString &password)
{
    email_ = email;
    password_ = password;
}
void FourSuites::authenticate()
{
    // Create a JSON object with the desired data
    QJsonObject jsonObject;
    jsonObject["email"] = email_;
    jsonObject["password"] = password_;

    // Convert the JSON object to a QByteArray
    QJsonDocument jsonDocument(jsonObject);
    QByteArray jsonData = jsonDocument.toJson();

    // Create a QNetworkRequest and set the URL
    QNetworkRequest request(QUrl(this->login_url));

    // Set the request method to POST
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Make the POST request
    reply = networkManager->post(request, jsonData); // Connect the finished signal to handle the response

    QObject::connect(reply, &QNetworkReply::finished, [=]()
                     {
        if (reply->error() == QNetworkReply::NoError) {
            // Read the response data
            QByteArray responseData = reply->readAll();
//            qDebug() << "Response: " << responseData;
            extract_token(responseData);
        } else {
            // Handle the error
            qDebug() << "Error: " << reply->errorString();
            qDebug() << "Network error code: " << reply->error();
        }

        // Clean up the reply object
        reply->deleteLater(); });
}

void FourSuites::get_doors()
{
    // Create a QNetworkRequest and set the URL
    QNetworkRequest request(QUrl(this->accessible_doors_url));

    // Set the Authorization header with the Bearer token
    QByteArray authorizationHeader = "Bearer " + token_.toUtf8();
    request.setRawHeader("Authorization", authorizationHeader);

    // Make the GET request
    reply = networkManager->get(request);

    // Connect the finished signal to handle the response
    QObject::connect(reply, &QNetworkReply::finished, [&]()
                     {
        if (reply->error() == QNetworkReply::NoError) {
            // Read the response data
            QByteArray responseData = reply->readAll();
            qDebug() << "Response: " << responseData;
        } else {
            // Handle the error
            qDebug() << "Error: " << reply->errorString();
        }

        // Clean up the reply object
        reply->deleteLater(); });
}

void FourSuites::open_door(Doors d)
{
    QString d_str = QString::number(d);
    QString door_str = d_str + "/open";
    QString requested_door_url = joinUrls(open_doors_url, door_str);

    qDebug() << requested_door_url;
    // Create a QNetworkRequest and set the URL
    QNetworkRequest request((QUrl(requested_door_url)));

    // Set the request method to POST
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token_).toUtf8());

    // Make the POST request
    reply = networkManager->post(request, QByteArray());

    // Synchronously wait for the request to finish
    // TODO: When we do this asynchronously (check previous snippets),
    // we get segfaults... WHY?
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check for errors
    if (reply->error() == QNetworkReply::NoError)
    {
        // Read the response data
        QByteArray responseData = reply->readAll();
        qDebug() << "Response: " << responseData;
    }
    else
    {
        // Handle the error
        qDebug() << "Error: " << reply->errorString();
    }
    // Clean up the reply object
    reply->deleteLater();
}

QString joinUrls(const QString &baseUrl, const QString &relativeUrl)
{
    QUrl base(baseUrl);
    QUrl relative(relativeUrl);

    QUrl joined = base.resolved(relative);

    return joined.toString();
}
