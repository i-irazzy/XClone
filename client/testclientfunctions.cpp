#include <QtTest>
#include "functionsforclient.h"
#include "clientapi.h"

class MockClientAPI : public ClientAPI {
public:
    QByteArray query_to_server(const QString &request) {
        qDebug() << "[Mock API] Received request:" << request;
        if (request.startsWith("CREATE")) {
            qDebug() << "[Mock API] Returning OK for create post";
            return "OK";
        } else if (request.startsWith("DELETE_POST")) {
            qDebug() << "[Mock API] Returning OK for delete post";
            return "OK";
        }

        qDebug() << "[Mock API] Returning ERROR";
        return "ERROR";
    }

};

class TestClientFunctions : public QObject {
    Q_OBJECT

private slots:

    void testCreateNewPost() {
        MockClientAPI mockApi;
        ClientAPI::setInstance(&mockApi);
        bool result = createNewPost("user1", "Title1", "Text1");
        qDebug() << "CreateNewPost result:" << result;
        QVERIFY(result);
    }

    void testDeletePost() {
        MockClientAPI mockApi;
        ClientAPI::setInstance(&mockApi);

        bool result = deletePost("user1", 123);
        qDebug() << "DeletePost result:" << result;
        QVERIFY(result);
    }
};

QTEST_MAIN(TestClientFunctions)
#include "testclientfunctions.moc"

