#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>
#include <QTcpSocket>   ///< Для установки соединения
#include <QByteArray>   ///< Для работы со строками и байтами
#include <QDebug>       ///< Для вывода отладочной информации
#include <QtNetwork>    ///< Для указания IP-адресации
#include <QString>

class ClientAPI;

/**
 * @brief Класс-уничтожитель для Singleton.
 *
 * Используется для корректного удаления Singleton-экземпляра ClientAPI.
 */
class SingletonDestroyer {
private:
    /**
     * @brief Указатель на экземпляр ClientAPI.
     */
    ClientAPI* p_instance;

public:
    /**
     * @brief Деструктор уничтожителя Singleton.
     */
    ~SingletonDestroyer();

    /**
     * @brief Инициализирует указатель на Singleton экземпляр.
     * @param p Указатель на ClientAPI.
     */
    void initialize(ClientAPI* p);
};

/**
 * @brief Singleton-класс для управления соединением клиента с сервером.
 */
class ClientAPI : public QObject {
    Q_OBJECT

private:
    /**
     * @brief Указатель на Singleton-экземпляр ClientAPI.
     */
    static ClientAPI* p_instance;

    /**
     * @brief Объект уничтожителя Singleton.
     */
    static SingletonDestroyer destroyer;

    /**
     * @brief Указатель на TCP-сокет.
     */
    QTcpSocket* mTcpSocket;

    /**
     * @brief Приватный конструктор (часть паттерна Singleton).
     * @param parent Родительский объект.
     */
    ClientAPI(QObject* parent = nullptr);

    /**
     * @brief Деструктор.
     */
    ~ClientAPI();

// Запрещено копирование и присваивание
   ClientAPI(const ClientAPI&) = delete;
   ClientAPI& operator=(ClientAPI&) = delete;

   /**
    * @brief Для доступа к закрытым членам из SingletonDestroyer.
    */
   friend class SingletonDestroyer;

   /**
    * @brief (Не используется) альтернативный указатель на экземпляр.
    */
   static ClientAPI* instance;

public:
   /**
    * @brief Получить Singleton-экземпляр ClientAPI.
    * @return Указатель на ClientAPI.
    */
   static ClientAPI* getInstance();

   /**
    * @brief Отправить запрос на сервер и получить ответ.
    * @param query Команда или данные, отправляемые на сервер.
    * @return Ответ от сервера в виде QByteArray.
    */
   QByteArray query_to_server(QString query);

   /**
   * @brief Установить Singleton-экземпляр вручную.
   * @param instance Указатель на новый экземпляр.
   */
  static void setInstance(ClientAPI* instance);

signals:
  /**
   * @brief Сигнал об отключении от сервера.
   */
  void disconnected();

public slots:
  /**
   * @brief Слот вызывается при отключении от сервера.
   */
  void slotServerDisconnection();
};


#endif // CLIENTAPI_H
