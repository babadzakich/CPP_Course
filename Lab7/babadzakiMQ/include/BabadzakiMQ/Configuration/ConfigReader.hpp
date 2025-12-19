#pragma once

#include <yaml-cpp/yaml.h>
#include <BabadzakiMQ/Configuration/Config.hpp>
#include <vector>

class ConfigReader
{
private:
    YAML::Node config_node;
public:
    ConfigReader(const std::string& filepath) : config_node(YAML::LoadFile(filepath)) {}
    std::vector<Config> readQueues() const;
};
