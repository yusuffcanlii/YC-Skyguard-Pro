#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <random>
#include <iomanip>
#include <string>  

// Konsol çıktılarının birbirine karışmaması için kilit
std::mutex print_mutex;

struct Target {
    int id;
    double x, y;
    double speed;

    double getDistance() const {
        return std::sqrt(x*x + y*y);
    }
};

class Radar {
private:
    std::vector<Target> targets;
    double max_range;
    bool active;

public:
    Radar(double range) : max_range(range), active(false) {}

    void addRandomTargets(int count) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> loc(-100.0, 100.0);
        std::uniform_real_distribution<> spd(100.0, 900.0);

        for(int i = 0; i < count; ++i) {
            targets.push_back({i + 1, loc(gen), loc(gen), spd(gen)});
        }
    }

    void scan() {
        active = true;
        double angle = 0.0;
        
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << "[RADAR] System active. Scanning range: " << max_range << " km\n";
        }

        while (active && angle <= 360.0) {
            // Radar dönüş hızı simülasyonu
            std::this_thread::sleep_for(std::chrono::milliseconds(150));

            int found_count = 0;
            
            // Basit bir tarama mantığı
            for (const auto& t : targets) {
                if (t.getDistance() <= max_range) {
                    // Sadece belirli açılarda ekrana bas (Log kirliliğini önle)
                    if ((int)angle % 90 == 0) { 
                        std::lock_guard<std::mutex> lock(print_mutex);
                        std::cout << "   -> Target Found: ID " << t.id 
                                  << " | Dist: " << std::fixed << std::setprecision(1) << t.getDistance() << "km\n";
                    }
                    found_count++;
                }
            }

            angle += 10.0;
        }
    }
};

int main() {
    // 120km menzilli radar
    Radar myRadar(120.0);
    
    // Rastgele 10 uçak/hedef ekle
    myRadar.addRandomTargets(10);

    // Taramayı farklı bir thread'de başlat (Main thread kilitlenmesin)
    std::thread radar_thread(&Radar::scan, &myRadar);

    // Ana işlem burada başka işler yapabilir (UI update vs.)
    for(int i=0; i<3; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[MAIN] GUI updating... " << (i+1)*33 << "%\n";
    }

    // Thread'in işini bitirmesini bekle
    if(radar_thread.joinable()) {
        radar_thread.join();
    }

    std::cout << "[MAIN] Simulation finished.\n";
    return 0;
}