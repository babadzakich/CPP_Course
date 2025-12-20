#include <BabadzakiMQ/Brocker/Queues/DiskQueueNone.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>

DiskQueueNone::DiskQueueNone(const std::string& name, std::chrono::seconds ttl,
                             const std::filesystem::path& storage_path)
    : name_(name), default_ttl_(ttl), next_id_(0) {

  storage_dir_ = storage_path / name;
  std::filesystem::create_directories(storage_dir_);

  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file()) {
      ++next_id_;
    }
  }
}

std::string DiskQueueNone::generateFilename() const {
  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

  std::ostringstream oss;
  oss << timestamp << "_" << next_id_ << ".msg";
  return oss.str();
}

std::chrono::system_clock::time_point DiskQueueNone::parseTimestampFromFilename(const std::string& filename) const {
  size_t underscore_pos = filename.find('_');
  if (underscore_pos == std::string::npos) {
    return std::chrono::system_clock::time_point{};
  }

  try {
    long long timestamp_ms = std::stoll(filename.substr(0, underscore_pos));
    return std::chrono::system_clock::time_point(std::chrono::milliseconds(timestamp_ms));
  } catch (...) {
    return std::chrono::system_clock::time_point{};
  }
}

void DiskQueueNone::push(const Message& message) {
  std::lock_guard<std::mutex> lock(mutex_);

  std::string filename = generateFilename();
  auto filepath = storage_dir_ / filename;

  std::ofstream file(filepath, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Failed to create message file: " + filepath.string());
  }

  auto data = message.serialize();
  file.write(reinterpret_cast<const char*>(data.data()), data.size());
  file.close();

  ++next_id_;
}

bool DiskQueueNone::pop(Message& message) {
  std::lock_guard<std::mutex> lock(mutex_);

  std::filesystem::path any_file;
  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".msg") {
      any_file = entry.path();
      break;
    }
  }

  if (any_file.empty()) {
    return false;
  }

  std::ifstream file(any_file, std::ios::binary);
  if (!file) {
    return false;
  }

  std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  message = Message::deserialize(data);

  std::filesystem::remove(any_file);

  return true;
}

bool DiskQueueNone::peek(Message& message, size_t index) const {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<std::filesystem::path> files;
  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".msg") {
      files.push_back(entry.path());
    }
  }

  std::sort(files.begin(), files.end());

  if (index >= files.size()) {
    return false;
  }

  std::ifstream file(files[index], std::ios::binary);
  if (!file) {
    return false;
  }

  std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();

  message = Message::deserialize(data);

  return true;
}

bool DiskQueueNone::isEmpty() const {
  std::lock_guard<std::mutex> lock(mutex_);

  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".msg") {
      return false;
    }
  }
  return true;
}

size_t DiskQueueNone::size() const {
  std::lock_guard<std::mutex> lock(mutex_);

  size_t count = 0;
  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".msg") {
      ++count;
    }
  }
  return count;
}

void DiskQueueNone::clear() {
  std::lock_guard<std::mutex> lock(mutex_);

  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (entry.is_regular_file() && entry.path().extension() == ".msg") {
      std::filesystem::remove(entry.path());
    }
  }

  next_id_ = 0;
}

std::string DiskQueueNone::getName() const {
  return name_;
}

void DiskQueueNone::cleanupExpiredMessages() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (default_ttl_.count() == 0) {
    return;
  }

  auto now = std::chrono::system_clock::now();

  for (const auto& entry : std::filesystem::directory_iterator(storage_dir_)) {
    if (!entry.is_regular_file() || entry.path().extension() != ".msg") {
      continue;
    }

    std::string filename = entry.path().filename().string();
    auto timestamp = parseTimestampFromFilename(filename);

    auto expiry_time = timestamp + default_ttl_;
    if (timestamp == std::chrono::system_clock::time_point{} || expiry_time < now) {
      std::filesystem::remove(entry.path());
    }
  }
}
