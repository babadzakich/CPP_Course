#include <iostream>
#include <fstream>
#include <vector>
#include <3DEngine/Utility/Vec3.hpp>
#include <3DEngine/Utility/Particle.hpp>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <bin or not bin>\n";
        return EXIT_FAILURE;
    }

    const std::string input_file(argv[1]);
    const bool binary = std::string(argv[2]) == "bin";
    std::ifstream file(input_file, binary ? std::ios::binary : std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input_file << "\n";
        return EXIT_FAILURE;
    }

    std::vector<Particle> particles;
    Particle p;
    if (!binary) {
        while (file >> p.id >> p.pos.x >> p.pos.y >> p.pos.z
                >> p.vel.x >> p.vel.y >> p.vel.z
                >> p.mass >> p.radius) {
            particles.push_back(p);
        }
    } else {
        while (file.read(reinterpret_cast<char*>(&p), sizeof(Particle))) {
            particles.push_back(p);
        }
    }

    if (particles.empty()) {
        std::cerr << "No particles loaded from file: " << input_file << "\n";
        return EXIT_FAILURE;
    }
    std::string output_filename = !binary ? "output_particles.bin" : "output_particles.txt";

    std::ofstream output_file(output_filename, binary ? std::ios::out : std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Could not open output file for writing\n";
        return EXIT_FAILURE;
    }

    if (!binary) {
        for (const auto& particle : particles) {
            output_file.write(reinterpret_cast<const char*>(&particle), sizeof(Particle));
        }
    } else {
        for (const auto& particle : particles) {
            output_file << particle.id << " "
                        << particle.pos.x << " " << particle.pos.y << " " << particle.pos.z << " "
                        << particle.vel.x << " " << particle.vel.y << " " << particle.vel.z << " "
                        << particle.mass << " " << particle.radius << "\n";
        }
    }
}
