#pragma once

#include <BabadzakiMQ/Brocker/Queues/Message.hpp>
#include <string>

class IQueue {
public:
    virtual ~IQueue() = default;

    /**
     * @brief Добавить сообщение в очередь
     * @param message Сообщение для добавления
     */
    virtual void push(const Message& message) = 0;

    /**
     * @brief Извлечь сообщение из очереди
     * @param message Выходной параметр для сообщения
     * @return true если сообщение было извлечено, false если очередь пуста
     */
    virtual bool pop(Message& message) = 0;

    /**
     * @brief Прочитать сообщение без удаления из очереди (для QoS=0)
     * @param message Выходной параметр для сообщения
     * @param index Индекс сообщения для чтения
     * @return true если сообщение было прочитано
     */
    virtual bool peek(Message& message, size_t index = 0) const = 0;

    /**
     * @brief Проверить, пуста ли очередь
     * @return true если очередь пуста
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Получить размер очереди
     * @return Количество сообщений в очереди
     */
    virtual size_t size() const = 0;

    /**
     * @brief Очистить очередь
     */
    virtual void clear() = 0;

    /**
     * @brief Получить имя/топик очереди
     * @return Имя очереди
     */
    virtual std::string getName() const = 0;

    virtual void cleanupExpiredMessages() = 0;
};