#pragma once

#include <chrono>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

/**
 * @brief Структура сообщения для очереди
 * 
 * Сообщения хранятся в формате JSON и могут быть сериализованы/десериализованы
 */
struct Message {
  nlohmann::json data;
  std::string topic;
  std::chrono::system_clock::time_point timestamp;
  std::string sender_id;

  Message() : timestamp(std::chrono::system_clock::now()) {}

  explicit Message(const nlohmann::json& json_data, const std::string& msg_topic = "", const std::string& sender = "")
      : data(json_data), topic(msg_topic), timestamp(std::chrono::system_clock::now()), sender_id(sender) {}

  /**
     * @brief Сериализует сообщение в бинарный формат
     * @return Вектор байтов
     */
  std::vector<uint8_t> serialize() const {
    nlohmann::json full_msg;
    full_msg["data"] = data;
    full_msg["topic"] = topic;
    full_msg["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()).count();
    full_msg["sender_id"] = sender_id;

    std::string json_str = full_msg.dump();
    return std::vector<uint8_t>(json_str.begin(), json_str.end());
  }

  /**
     * @brief Десериализует сообщение из бинарного формата
     * @param bytes Вектор байтов
     * @return Десериализованное сообщение
     */
  static Message deserialize(const std::vector<uint8_t>& bytes) {
    std::string json_str(bytes.begin(), bytes.end());
    nlohmann::json full_msg = nlohmann::json::parse(json_str);

    Message msg;
    msg.data = full_msg["data"];
    msg.topic = full_msg["topic"];
    msg.sender_id = full_msg["sender_id"];

    auto timestamp_ms = full_msg["timestamp"].get<long long>();
    msg.timestamp = std::chrono::system_clock::time_point(std::chrono::milliseconds(timestamp_ms));

    return msg;
  }

  /**
     * @brief Возвращает строковое представление сообщения
     * @return JSON строка с форматированием
     */
  std::string toString() const {
    nlohmann::json result;
    result["topic"] = topic;
    result["sender_id"] = sender_id;
    result["data"] = data;
    return result.dump(2);
  }
};
