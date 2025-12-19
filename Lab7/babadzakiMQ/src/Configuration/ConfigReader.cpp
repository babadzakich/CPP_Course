#include <BabadzakiMQ/Configuration/ConfigReader.hpp>

std::vector<Config> ConfigReader::readQueues() const
{
    std::vector<Config> queues;
    
    if (!config_node["queues"]) {
        throw std::runtime_error("No 'queues' section found in config file");
    }
    
    const YAML::Node& queues_node = config_node["queues"];
    
    if (!queues_node.IsSequence()) {
        throw std::runtime_error("'queues' must be a sequence/array");
    }
    
    for (const auto& queue_node : queues_node) {
        std::string name = queue_node["name"].as<std::string>();
        std::string durability = queue_node["durability"] ? queue_node["durability"].as<std::string>() : "disk";
        std::string ordering = queue_node["order"] ? queue_node["order"].as<std::string>() : "fifo";
        time_t message_ttl = queue_node["message_ttl"] ? queue_node["message_ttl"].as<time_t>() : 0;
        
        queues.emplace_back(name, durability, ordering, message_ttl);
    }
    
    return queues;
}
