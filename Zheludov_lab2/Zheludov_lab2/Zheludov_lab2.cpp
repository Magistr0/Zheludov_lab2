#include <iostream> 
#include <fstream> 
#include <vector> 
#include <algorithm> 
#include <windows.h>
#include <string>

using namespace std;
int isint(int min, int max);
bool y_or_n();

// Структура для представления трубы 
struct Pipe {
    std::string id;
    std::string name;
    int length;
    int diameter;
    bool inRepair;

    void readFromConsole() {
        std::cout << "Введите ID трубы: ";
        std::cin >> id;
        std::cout << "Введите название трубы: ";
        std::cin >> name;
        std::cout << "Введите длину трубы (в километрах): ";
        length = isint(1, INT_MAX);
        std::cout << "Введите диаметр трубы: ";
        diameter = isint(1, INT_MAX);
        std::cout << "Труба в ремонте? (y/n): ";
        inRepair = y_or_n();
    }

    void printToConsole() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Название: " << name << std::endl;
        std::cout << "Длина (км): " << length << std::endl;
        std::cout << "Диаметр: " << diameter << std::endl;
        std::cout << "В ремонте: " << (inRepair ? "Да" : "Нет") << std::endl;
    }
};

// Структура для представления КС 
struct CompressorStation {
    std::string id;
    std::string name;
    int workshopCount;
    int activeWorkshops;
    int efficiency;

    void readFromConsole() {
        std::cout << "Введите ID КС: ";
        std::cin >> id;
        std::cout << "Введите название КС: ";
        std::cin >> name;
        std::cout << "Введите количество цехов: ";
        workshopCount = isint(1, INT_MAX);
        std::cout << "Введите количество активных цехов: ";
        activeWorkshops = isint(1, workshopCount);
        std::cout << "Введите показатель эффективности: ";
        efficiency = isint(0, 100);
    }

    void printToConsole() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Название: " << name << std::endl;
        std::cout << "Количество цехов: " << workshopCount << std::endl;
        std::cout << "Активных цехов: " << activeWorkshops << std::endl;
        std::cout << "Эффективность: " << efficiency << std::endl;
    }
};

int isint(int min, int max) {
    int num;
    while ((cin >> num).fail() || (num < min) || (num > max)) {
        cout << "Введено не верное значение" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\n";
    }
    cout << endl;
    return num;
}

bool y_or_n() {
    string result;
    cin >> result;
    if (result != "y" && result != "n") {
        cout << "Введено не верное значение (Должно быть значение y/n)" << endl;
        return y_or_n();
    }
    return result == "y"?true:false;
}
int menu_input() {
    int result;

    cout << "_________________________" << endl << endl;
    cout << "Главное меню:" << endl;
    std::cout << "1. Добавить трубу" << std::endl;
    std::cout << "2. Добавить КС" << std::endl;
    std::cout << "3. Просмотр всех объектов" << std::endl;
    std::cout << "4. Редактировать трубу" << std::endl;
    std::cout << "5. Редактировать КС" << std::endl;
    std::cout << "6. Сохранить" << std::endl;
    std::cout << "7. Загрузить" << std::endl;
    std::cout << "0. Выход" << std::endl;
    cout << "_________________________" << endl << endl;
    result = isint(0, 7);
    return result;
}
// Функция для сохранения данных в файл 
void saveData(const std::vector<Pipe>& pipes, const std::vector<CompressorStation>& stations, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << pipes.size() << std::endl;
        for (const auto& pipe : pipes) {
            file << pipe.id << std::endl;
            file << pipe.name << std::endl;
            file << pipe.length << std::endl;
            file << pipe.diameter << std::endl;
            file << pipe.inRepair << std::endl;
        }

        file << stations.size() << std::endl;
        for (const auto& station : stations) {
            file << station.id << std::endl;
            file << station.name << std::endl;
            file << station.workshopCount << std::endl;
            file << station.activeWorkshops << std::endl;
            file << station.efficiency << std::endl;
        }

        file.close();
        std::cout << "Данные сохранены в файл." << std::endl;
    }
    else {
        std::cout << "Не удалось открыть файл для сохранения данных." << std::endl;
    }
}

// Функция для загрузки данных из файла 
void loadData(std::vector<Pipe>& pipes, std::vector<CompressorStation>& stations, const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        pipes.clear();
        stations.clear();

        int pipeCount;
        file >> pipeCount;
        file.ignore();

        for (int i = 0; i < pipeCount; ++i) {
            Pipe pipe;
            file >> pipe.id;
            file >> pipe.name;  
            file >> pipe.length;
            file >> pipe.diameter;
            file >> pipe.inRepair;
            file.ignore();
            pipes.push_back(pipe);
        }

        int stationCount;
        file >> stationCount;
        file.ignore();

        for (int i = 0; i < stationCount; ++i) {
            CompressorStation station;
            file >> station.id;
            file >> station.name;
            file >> station.workshopCount;
            file >> station.activeWorkshops;
            file >> station.efficiency;
            file.ignore();
            stations.push_back(station);
        }

        std::cout << "Данные загружены из файла." << std::endl;
    }
    else {
        std::cout << "Не удалось открыть файл для загрузки данных." << std::endl;
    }
}
// Функция для поиска трубы по названию 
std::vector<Pipe> findPipesByName(const std::vector<Pipe>& pipes, const std::string& name) {
    std::vector<Pipe> foundPipes;
    for (const auto& pipe : pipes) {
        if (pipe.name == name) {
            foundPipes.push_back(pipe);
        }
    }
    return foundPipes;
}

// Функция для поиска трубы по признаку "в ремонте" 
std::vector<Pipe> findPipesInRepair(const std::vector<Pipe>& pipes, bool inRepair) {
    std::vector<Pipe> foundPipes;
    for (const auto& pipe : pipes) {
        if (pipe.inRepair == inRepair) {
            foundPipes.push_back(pipe);
        }
    }
    return foundPipes;
}

// Функция для поиска КС по названию 
std::vector<CompressorStation> findStationsByName(const std::vector<CompressorStation>& stations, const std::string& name) {
    std::vector<CompressorStation> foundStations;
    for (const auto& station : stations) {
        if (station.name == name) {
            foundStations.push_back(station);
        }
    }
    return foundStations;
}

// Функция для поиска КС по проценту незадействованных цехов 
std::vector<CompressorStation> findStationsByUnusedWorkshops(const std::vector<CompressorStation>& stations, double unusedPercentage) {
    std::vector<CompressorStation> foundStations;
    for (const auto& station : stations) {
        double unusedWorkshops = station.workshopCount - station.activeWorkshops;
        double percentage = (unusedWorkshops / station.workshopCount) * 100.0;
        if (percentage >= unusedPercentage) {
            foundStations.push_back(station);
        }
    }
    return foundStations;
}

void PipeOnConsole(vector<Pipe> pipes) {
    std::cout << "Трубы:" << std::endl;
    for (auto& pipe : pipes) {
        pipe.printToConsole();
        std::cout << std::endl;
    }
}

void KSOnConsole(vector<CompressorStation> stations) {
    std::cout << "Компрессорные станции:" << std::endl;
    for (auto& station : stations) {
        station.printToConsole();
        std::cout << std::endl;
    }
}

// Функция для пакетного редактирования труб 
void editPipes(std::vector<Pipe>& pipes) {
    std::vector<Pipe> foundPipes;
    std::string name;
    int choice;
    bool repair;
    std::cout << "Искать трубы " << endl;
    std::cout << "1. По названию " << endl;
    std::cout << "2. По состоянию ремонта" << endl;
    std::cout << "0. Выход" << endl;
    choice = isint(0, 2);
    switch (choice) {
    case 1:
        std::cout << "Введите название труб для редактирования: ";
        std::cin >> name;

        foundPipes = findPipesByName(pipes, name);
        if (foundPipes.empty()) {
            std::cout << "Трубы с указанным названием не найдены." << std::endl;
            return;
        }
        break;
    case 2:
        std::cout << "Состояние в ремонте (y/n)" << endl;
        repair = y_or_n();
        foundPipes = findPipesInRepair(pipes, repair);
        if (foundPipes.empty()) {
            std::cout << "Трубы с указанным состоянием не найдены не найдены." << std::endl;
            return;
        }
        break;
    default:
        return;
        break;
    }
    std::cout << "Найденые объекты:" << std::endl;
    PipeOnConsole(foundPipes);
    std::cout << "Выберите действие для редактирования:" << std::endl;
    std::cout << "1. Изменить длину" << std::endl;
    std::cout << "2. Изменить диаметр" << std::endl;
    std::cout << "3. Изменить статус ремонта" << std::endl;
    std::cout << "4. Удалить" << std::endl;
    std::cout << "0. Назад" << std::endl;
    std::cout << "Выберите действие: ";
    choice = isint(0,4);

    switch (choice) {
    case 1:
        int newLength;
        std::cout << "Введите новую длину: ";
        newLength = isint(1, INT_MAX);

        for (auto& pipe : foundPipes) {
            pipe.length = newLength;
        }
        break;
    case 2:
        int newDiameter;
        std::cout << "Введите новый диаметр: ";
        newDiameter = isint(1, INT_MAX);

        for (auto& pipe : foundPipes) {
            pipe.diameter = newDiameter;
        }
        break;
    case 3:
        bool newInRepair;
        std::cout << "Труба в ремонте? (y/n): ";
        newInRepair = y_or_n();

        for (auto& pipe : foundPipes) {
            pipe.inRepair = newInRepair;
        }
        break;
    case 4:
        // Удалить найденные трубы 
        pipes.erase(std::remove_if(pipes.begin(), pipes.end(), [&name](const Pipe& pipe) { return pipe.name == name; }), pipes.end());
        break;
    case 0:
        return editPipes(pipes);
        break;
    }
}

void editKS(std::vector<CompressorStation>& KSes) {
    std::string name;
    std::cout << "Введите название КС для редактирования: ";
    std::cin >> name;

    std::vector<CompressorStation> foundKSes = findStationsByName(KSes, name);
    if (foundKSes.empty()) {
        std::cout << "КС с указанным названием не найдены." << std::endl;
        return;
    }

    int choice;
    std::cout << "Выберите действие для редактирования:" << std::endl;
    std::cout << "1. Изменить количество цехов" << std::endl;
    std::cout << "2. Изменить Эфективность" << std::endl;
    std::cout << "3. Удалить" << std::endl;
    std::cout << "0. Назад" << std::endl;
    std::cout << "Выберите действие: ";
    choice = isint(1, 4);

    switch (choice) {
    case 1:
        int newWorkshops;
        std::cout << "Введите новое количество цехов: ";
        newWorkshops = isint(1, INT_MAX);

        for (auto& KS : foundKSes) {
            KS.workshopCount = newWorkshops;
        }
        int newactivWorkshops;
        std::cout << "Введите новое количество активных цехов: ";
        newactivWorkshops = isint(0, newWorkshops);

        for (auto& KS : foundKSes) {
            KS.activeWorkshops = newactivWorkshops;
        }
        break;
    case 2:
        int newefficiency;
        std::cout << "Введите новую эфективность: ";
        newefficiency = isint(0, 100);

        for (auto& KS : foundKSes) {
            KS.efficiency = newefficiency;
        }
        break;
    case 3:
        KSes.erase(std::remove_if(KSes.begin(), KSes.end(), [&name](const CompressorStation& KS) { return KS.name == name; }), KSes.end());
        break;
    case 0:
        std::cout << "Неверный выбор." << std::endl;
        break;
    }
}


int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Pipe> pipes;
    Pipe pipe;
    CompressorStation station;
    std::vector<CompressorStation> stations;
    std::string filename;
    bool flag = true;

    int choice;
    while (flag) {
        choice = menu_input();

        switch (choice) {
        case 1:
            pipe.readFromConsole();
            pipes.push_back(pipe);
            break;
        case 2:
            station.readFromConsole();
            stations.push_back(station);
            break;
        case 3:
            // Вывод всех объектов 
            if (pipes.size() == 0 && stations.size() == 0) {
                std::cout << "У вас нет объектов"<<endl;
                break;
            }
            PipeOnConsole(pipes);
            KSOnConsole(stations);
            break;
        case 4:
            editPipes(pipes);
            break;
        case 5:
            editKS(stations);
            break;
        case 6:
            std::cout << "Введите имя файла для сохранения: ";
            std::cin >> filename;
            saveData(pipes, stations, filename);
            break;
        case 7:
            std::cout << "Введите имя файла для загрузки: ";
            std::cin >> filename;
            loadData(pipes, stations, filename);
            break;
        case 0:
            flag = false;
            break;
        default:
            std::cout << "Неверный выбор." << std::endl;
            break;
        }
    }
    std::cout << "Завершение работы..." << std::endl;
    return 0;
}