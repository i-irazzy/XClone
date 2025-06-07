#include <QtTest>
#include <QString>
#include <QByteArray>
#include <QDebug>

// 🔹 MockAPI — имитация сервера
class MockAPI {
public:
    static MockAPI* getInstance() {
        static MockAPI instance;
        return &instance;
    }

    QByteArray query_to_server(const QString& request) {
        qDebug() << "Mock запрос: " << request;

        if (request.startsWith("AUTH|")) {
            return request.contains("testuser|password123") ? "log+" : "error";
        }
        if (request.startsWith("REG|")) {
            return request.contains("newuser") ? "reg+" : "error";
        }
        if (request.startsWith("CREATE_POST|")) {
            return "post+";
        }
        if (request.startsWith("GET_POSTS_BY_USER|")) {
            return request.contains("testuser") ? "testuser|Заголовок|Текст\n" : "";
        }
        if (request.startsWith("GET_POSTS_BY_TEXT|")) {
            return request.contains("Текст") ? "testuser|Заголовок|Текст\n" : "";
        }
        if (request.startsWith("DELETE_POST|")) {
            int postId = request.split("|")[1].toInt();
            return (postId > 0) ? "OK" : "error";  // ✔️ Теперь `-1` вернёт "error"
        }


        return "error";
    }
};

// 🔹 Структура поста
struct Post {
    QString username;
    QString header;
    QString text;
};

// 🔹 Глобальная переменная для текущего пользователя
QString currentUsernameT = "";

// 🔹 Функции клиента (используют MockAPI)
bool authT(QString login, QString password) {
    QString request = QString("AUTH|%1|%2").arg(login, password);
    QByteArray response = MockAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "log+";
}

bool regT(QString login, QString password, QString email) {
    QString request = QString("REG|%1|%2|%3").arg(login, password, email);
    QByteArray response = MockAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "reg+";
}

bool createNewPostT(const QString& header, const QString& text) {
    if (currentUsernameT.isEmpty()) return false;
    QString request = QString("CREATE_POST|%1|%2|%3").arg(currentUsernameT, header, text);
    QByteArray response = MockAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "post+";
}

QList<Post> getPostsByUserT(const QString &username) {
    QList<Post> result;
    QByteArray response = MockAPI::getInstance()->query_to_server("GET_POSTS_BY_USER|" + username);
    QList<QByteArray> lines = response.split('\n');
    for (const QByteArray& line : lines) {
        QList<QByteArray> parts = line.split('|');
        if (parts.size() == 3) {
            Post post;
            post.username = QString::fromUtf8(parts[0]);
            post.header   = QString::fromUtf8(parts[1]);
            post.text     = QString::fromUtf8(parts[2]);
            result.append(post);
        }
    }
    return result;
}

bool deletePostT(int postId) {
    QByteArray response = MockAPI::getInstance()->query_to_server("DELETE_POST|" + QString::number(postId));
    return response.trimmed() == "OK";
}

// 🔹 Unit-тесты
class TestFunctionsForClient : public QObject {
    Q_OBJECT

private slots:
    void testAuth();
    void testReg();
    void testCreateNewPost();
    void testGetPostsByUser();
    void testDeletePost();
};

// Тест авторизации
void TestFunctionsForClient::testAuth() {
    QVERIFY(authT("testuser", "password123"));  // Ожидаем успешную авторизацию
    QVERIFY(!authT("wronguser", "wrongpass"));  // Ожидаем провал
}

// Тест регистрации
void TestFunctionsForClient::testReg() {
    QVERIFY(regT("newuser", "password123", "newuser@example.com"));  // Ожидаем успех
    QVERIFY(!regT("", "password123", "invalid@example.com"));  // Ожидаем провал
}

// Тест создания поста
void TestFunctionsForClient::testCreateNewPost() {
    currentUsernameT = "testuser";
    QVERIFY(createNewPostT("Заголовок", "Текст поста"));  // Ожидаем успех
    currentUsernameT = "";
    QVERIFY(!createNewPostT("Заголовок", "Текст поста"));  // Ожидаем провал
}

// Тест получения постов по пользователю
void TestFunctionsForClient::testGetPostsByUser() {
    QList<Post> posts = getPostsByUserT("testuser");
    QVERIFY(!posts.isEmpty());  // Ожидаем, что посты найдены
    QVERIFY(getPostsByUserT("nonexistentuser").isEmpty());  // Ожидаем пустой список
}

// Тест удаления поста
void TestFunctionsForClient::testDeletePost() {
    QVERIFY(deletePostT(1));  // Ожидаем успешное удаление
    QVERIFY(!deletePostT(-1));  // Ожидаем провал
}

// Запуск тестов
QTEST_MAIN(TestFunctionsForClient)
#include "testclientfunctions.moc"

