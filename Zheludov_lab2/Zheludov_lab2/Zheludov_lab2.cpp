#include <iostream> 
#include <fstream> 
#include <vector> 
#include <algorithm> 
#include <windows.h>
#include <string>
#include <ctime>
#include <limits>

using namespace std;


enum class LogLevel {
    INFO,
    WARNING,
    ERR0R
};

class Logger {
public:
    Logger(const std::string& logFileName) {
        logFile.open(logFileName, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Ошибка открытия файла лога: " << logFileName << std::endl;
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(LogLevel level, const std::string& message) {
        std::string levelStr;
        switch (level) {
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERR0R:
            levelStr = "ERROR";
            break;
        }


        string timeStr;
        SYSTEMTIME st;
        GetSystemTime(&st);
        timeStr = to_string(st.wMonth) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond);

        if (logFile.is_open()) {
            logFile << "[" << timeStr << "] [" << levelStr << "] " << message << std::endl;
        }
    }

private:
    std::ofstream logFile;
};

int isint(int min, int max, Logger& logger);
bool y_or_n(Logger& logger);
int giveID(Logger& logger);
double persent(Logger& logger);
vector<int> unicID;

// Структура для представления трубы 
struct Pipe {
    int id;
    std::string name;
    int length;
    int diameter;
    bool inRepair;

    void readFromConsole(Logger& logger) {
        id = giveID(logger);
        std::cout << "Введите название трубы: ";
        std::cin >> name;
        logger.log(LogLevel::INFO, "Введено значение: " + name);
        std::cout << "\nВведите длину трубы (в километрах): ";
        length = isint(1, INT_MAX, logger);
        std::cout << "Введите диаметр трубы: ";
        diameter = isint(1, INT_MAX, logger);
        std::cout << "Труба в ремонте? (y/n): ";
        inRepair = y_or_n(logger);
        logger.log(LogLevel::INFO, "Добавлена труба: " + name);
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
    int id;
    std::string name;
    int workshopCount;
    int activeWorkshops;
    double efficiency;

    void readFromConsole(Logger& logger) {
        id = giveID(logger);
        std::cout << "Введите название КС: ";
        std::cin >> name;
        logger.log(LogLevel::INFO, "Введено значение: " + name);
        std::cout << "\nВведите количество цехов: ";
        workshopCount = isint(1, INT_MAX, logger);
        std::cout << "Введите количество активных цехов: ";
        activeWorkshops = isint(1, workshopCount, logger);
        std::cout << "Введите показатель эффективности: ";
        efficiency = persent(logger);
        logger.log(LogLevel::INFO, "Добавлено КС: " + name);
    }

    void printToConsole() {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Название: " << name << std::endl;
        std::cout << "Количество цехов: " << workshopCount << std::endl;
        std::cout << "Активных цехов: " << activeWorkshops << std::endl;
        std::cout << "Эффективность: " << efficiency << std::endl;
        std::cout << endl;
    }
};
//Функция присваивания ID
int giveID(Logger& logger) {
    int size = unicID.size();
    if (size == 0 || unicID[0] != 0) {
        unicID.push_back(0);
        logger.log(LogLevel::INFO, "Присвоен ID: " + unicID.back());
        return 0;
    }
    else if (size == 1) {
        unicID.push_back(1);
        logger.log(LogLevel::INFO, "Присвоен ID: " + unicID.back());
        return 1;
    }
    else {
        for (int i = 0; i < size-1; ++i) {
            if (unicID[i+1] - unicID[i] != 1) {
                unicID.insert(unicID.begin() + i + 1, unicID[i] + 1);
                logger.log(LogLevel::INFO, "Присвоен ID: " + unicID[i + 1]);
                return unicID[i+1];
            }
        }
        unicID.push_back(unicID.back() + 1);
        logger.log(LogLevel::INFO, "Присвоен ID: " + unicID.back());
        return unicID.back();
    }
}

//Функция проверки на число
int isint(int min, int max, Logger& logger) {
    int num;
    while ((cin >> num).fail() || (num < min) || (num > max)) {
        cout << "Введено не верное значение" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        //cout << "\n";
    }
    cout << endl;
    logger.log(LogLevel::INFO, "Введено значение: " + to_string(num));
    return num;
}
//Функция проверки y/n
bool y_or_n(Logger& logger) {
    string result;
    cin >> result;
    if (result != "y" && result != "n") {
        cout << "Введено не верное значение (Должно быть значение y/n)" << endl;
        return y_or_n(logger);
    }
    logger.log(LogLevel::INFO, "Введено значение: " + result);
    return result == "y"?true:false;
}
//Функция проверки процента
double persent(Logger& logger) {
    double num;
    while ((cin >> num).fail() || (num < 0) || (num > 100)) {
        cout << "Введено не верное значение" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        //cout << "\n";
    }
    cout << endl;
    logger.log(LogLevel::INFO, "Введено значение: " + to_string(num));
    return num;
}

//Функция поиска паттерна в слове
bool searchPattern(const std::string& word, const std::string& pattern) {
    if (word.empty() || pattern.empty()) {
        return false;
    }
    size_t pos = word.find(pattern);
    return pos != std::string::npos;
}

void replaceElementsPipe(std::vector<Pipe>& target, const std::vector<Pipe>& replacement) {
    for (size_t i = 0; i < target.size(); ++i) {
        for (size_t j = 0; j < replacement.size(); ++j) {
            if (target[i].id == replacement[j].id) {
                target[i] = replacement[j];
            }
        }
    }
}
void replaceElementsKS(std::vector<CompressorStation>& target, const std::vector<CompressorStation>& replacement) {
    for (size_t i = 0; i < target.size(); ++i) {
        for (size_t j = 0; j < replacement.size(); ++j) {
            if (target[i].id == replacement[j].id) {
                target[i] = replacement[j];
            }
        }
    }
}
//Вывод меню
int menu_input(Logger& logger) {
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
    result = isint(0, 7, logger);
    return result;
}
// Функция для сохранения данных в файл 
void saveData(const std::vector<Pipe>& pipes, const std::vector<CompressorStation>& stations, const std::string& filename, Logger& logger) {
    logger.log(LogLevel::INFO, "Начало сохранения в файл.");
    std::ofstream file(filename);
    if (file.is_open()) {
        file << pipes.size() << std::endl;
        for (const auto& pipe : pipes) {
            //file << pipe.id << std::endl;
            file << pipe.name << std::endl;
            file << pipe.length << std::endl;
            file << pipe.diameter << std::endl;
            file << pipe.inRepair << std::endl;
        }

        file << stations.size() << std::endl;
        for (const auto& station : stations) {
            //file << station.id << std::endl;
            file << station.name << std::endl;
            file << station.workshopCount << std::endl;
            file << station.activeWorkshops << std::endl;
            file << station.efficiency << std::endl;
        }

        file.close();
        std::cout << "Данные сохранены в файл." << std::endl;
        logger.log(LogLevel::INFO, "Данные сохранены в файл.");
    }
    else {
        std::cout << "Не удалось открыть файл для сохранения данных." << std::endl;
        logger.log(LogLevel::ERR0R, "Не удалось открыть файл для сохранения данных.");
    }
}

// Функция для загрузки данных из файла 
void loadData(std::vector<Pipe>& pipes, std::vector<CompressorStation>& stations, const std::string& filename, Logger& logger) {
    logger.log(LogLevel::INFO, "Начало загрузки из файла.");
    std::ifstream file(filename);
    unicID = {};
    if (file.is_open()) {
        pipes.clear();
        stations.clear();

        int pipeCount;
        file >> pipeCount;
        file.ignore();

        for (int i = 0; i < pipeCount; ++i) {
            Pipe pipe;
            pipe.id = giveID(logger);
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
            station.id = giveID(logger);
            file >> station.name;
            file >> station.workshopCount;
            file >> station.activeWorkshops;
            file >> station.efficiency;
            file.ignore();
            stations.push_back(station);
        }

        std::cout << "Данные загружены из файла." << std::endl;
        logger.log(LogLevel::INFO, "Данные загружены из файла.");
    }
    else {
        std::cout << "Не удалось открыть файл для загрузки данных." << std::endl;
        logger.log(LogLevel::ERR0R, "Не удалось открыть файл для загрузки данных.");
    }
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

// Функция для поиска КС по проценту незадействованных цехов 
std::vector<CompressorStation> findStationsByUnusedWorkshops(const std::vector<CompressorStation>& stations, double unusedPercentage) {
    std::vector<CompressorStation> foundStations;
    for (const auto& station : stations) {
        double unusedWorkshops = station.workshopCount - station.activeWorkshops;
        double percentage = (unusedWorkshops / station.workshopCount) * 100.0;
        if (percentage <= unusedPercentage) {
            foundStations.push_back(station);
        }
    }
    return foundStations;
}
//Вывод труб
void PipeOnConsole(vector<Pipe> pipes) {
    std::cout << "Трубы:" << std::endl;
    for (auto& pipe : pipes) {
        pipe.printToConsole();
        std::cout << std::endl;
    }
}
//Вывод кс
void KSOnConsole(vector<CompressorStation> stations) {
    std::cout << "Компрессорные станции:" << std::endl;
    for (auto& station : stations) {
        station.printToConsole();
        std::cout << std::endl;
    }
}

// Функция для пакетного редактирования труб 
void editPipes(std::vector<Pipe>& pipes, Logger& logger) {
    logger.log(LogLevel::INFO, "Начало изменения труб");

    std::vector<Pipe> foundPipes;
    std::string name;
    int ID;
    int choice;
    bool repair;
    std::cout << "Искать трубы " << endl;
    std::cout << "1. По названию " << endl;
    std::cout << "2. По состоянию ремонта" << endl;
    std::cout << "0. Выход" << endl;
    choice = isint(0, 2, logger);
    switch (choice) {
    case 1:
        std::cout << "Введите название труб для редактирования: ";
        std::cin >> name;
        logger.log(LogLevel::INFO, "Введено значение: " + name);
        for (Pipe& pipe : pipes) {
            if (searchPattern(pipe.name, name)) {
                foundPipes.push_back(pipe);
            }
        }
        if (foundPipes.empty()) {
            std::cout << "Трубы с указанным названием не найдены." << std::endl;
            logger.log(LogLevel::WARNING, "Трубы с указанным названием не найдены.");
            return;
        }
        break;
    case 2:
        std::cout << "Состояние в ремонте (y/n): ";
        repair = y_or_n(logger);
        foundPipes = findPipesInRepair(pipes, repair);
        if (foundPipes.empty()) {
            std::cout << "Трубы с указанным состоянием не найдены." << std::endl;
            logger.log(LogLevel::WARNING, "Трубы с указанным состоянием не найдены.");
            return;
        }
        break;
    default:
        return;
        break;
    }
    std::cout << "Найденые объекты:" << "\n\n";
    PipeOnConsole(foundPipes);
    std::cout << "Выберите действие для редактирования:" << std::endl;
    std::cout << "1. Изменить длину" << std::endl;
    std::cout << "2. Изменить диаметр" << std::endl;
    std::cout << "3. Изменить статус ремонта" << std::endl;
    std::cout << "4. Удалить" << std::endl;
    std::cout << "0. Назад" << std::endl;
    std::cout << "Выберите действие: ";
    choice = isint(0,4, logger);

    switch (choice) {
    case 1:
        int newLength;
        std::cout << "Введите новую длину: ";
        newLength = isint(1, INT_MAX, logger);

        for (auto& pipe : foundPipes) {
            pipe.length = newLength;
        }
        replaceElementsPipe(pipes, foundPipes);
        logger.log(LogLevel::INFO, "Трубы успешно изменены");
        break;
    case 2:
        int newDiameter;
        std::cout << "Введите новый диаметр: ";
        newDiameter = isint(1, INT_MAX, logger);

        for (auto& pipe : foundPipes) {
            pipe.diameter = newDiameter;
        }
        replaceElementsPipe(pipes, foundPipes);
        logger.log(LogLevel::INFO, "Трубы успешно изменены");
        break;
    case 3:
        bool newInRepair;
        std::cout << "Труба в ремонте? (y/n): ";
        newInRepair = y_or_n(logger);

        for (auto& pipe : foundPipes) {
            pipe.inRepair = newInRepair;
        }
        replaceElementsPipe(pipes, foundPipes);
        logger.log(LogLevel::INFO, "Трубы успешно изменены");
        break;
    case 4:
        // Удалить найденные трубы 
        for (Pipe& pipe : foundPipes) {
            ID = pipe.id;
            unicID.erase(std::remove_if(unicID.begin(), unicID.end(), [&ID](const int& id) { return id == ID; }), unicID.end());
            pipes.erase(std::remove_if(pipes.begin(), pipes.end(), [&ID](const Pipe& pipe) { return pipe.id == ID; }), pipes.end());
        }
        
        cout << "Трубы успешно удалены" << endl;
        logger.log(LogLevel::INFO, "Трубы успешно удалены");
        break;
    case 0:
        return editPipes(pipes,logger);
        break;
    }
}
// Функция для пакетного редактирования кс
void editKS(std::vector<CompressorStation>& KSes, Logger& logger) {
    logger.log(LogLevel::INFO, "Начало изменения кс");
    std::vector<CompressorStation> foundKSes;
    int ID;
    std::string name;
    std::string names;
    int choice;
    double unusedPercentage;

    std::cout << "Искать станции " << endl;
    std::cout << "1. По названию " << endl;
    std::cout << "2. По проценту незадействованных цехов" << endl;
    std::cout << "0. Выход" << endl;
    choice = isint(0, 2, logger);
    switch (choice) {
    case 1:
        std::cout << "Введите название станций для редактирования: ";
        std::cin >> name;
        logger.log(LogLevel::INFO, "Введено значение: " + name);
        for (CompressorStation& KS : KSes) {
            if (searchPattern(KS.name, name)) {
                foundKSes.push_back(KS);
                names += KS.name;
            }
        }
        if (foundKSes.empty()) {
            std::cout << "Станции с указанным названием не найдены." << std::endl;
            logger.log(LogLevel::WARNING, "Станции с указанным названием не найдены.");
            return;
        }
        break;
    case 2:
        std::cout << "Введите процент незадействованных цехов" << endl;
        unusedPercentage = persent(logger);
        foundKSes = findStationsByUnusedWorkshops(KSes, unusedPercentage);
        if (foundKSes.empty()) {
            std::cout << "Станции с указанным состоянием не найдены." << std::endl;
            logger.log(LogLevel::WARNING, "Станции с указанным состоянием не найдены.");
            return;
        }
        break;
    default:
        return;
        break;
    }
    std::cout << "Найденые объекты:" << "\n\n";
    KSOnConsole(foundKSes);

    std::cout << "Выберите действие для редактирования:" << std::endl;
    std::cout << "1. Изменить количество цехов (И активных цехов)" << std::endl;
    std::cout << "2. Изменить Эфективность" << std::endl;
    std::cout << "3. Удалить" << std::endl;
    std::cout << "0. Назад" << std::endl;
    std::cout << "Выберите действие: ";
    choice = isint(0, 3, logger);

    switch (choice) {
    case 1:
        int newWorkshops;
        std::cout << "Введите новое количество цехов: ";
        newWorkshops = isint(1, INT_MAX, logger);

        for (auto& KS : foundKSes) {
            KS.workshopCount = newWorkshops;
        }
        int newactivWorkshops;
        std::cout << "Введите новое количество активных цехов: ";
        newactivWorkshops = isint(0, newWorkshops, logger);

        for (auto& KS : foundKSes) {
            KS.activeWorkshops = newactivWorkshops;
        }
        
        replaceElementsKS(KSes, foundKSes);
        logger.log(LogLevel::INFO, "КС успешно изменены");
        break;
    case 2:
        double newefficiency;
        std::cout << "Введите новую эфективность: ";
        newefficiency = persent(logger);

        for (auto& KS : foundKSes) {
            KS.efficiency = newefficiency;
        }
        replaceElementsKS(KSes, foundKSes);
        logger.log(LogLevel::INFO, "КС успешно изменены");
        break;
    case 3:
        for (CompressorStation& KS : foundKSes) {
            ID = KS.id;
            unicID.erase(std::remove_if(unicID.begin(), unicID.end(), [&ID](const int& id) { return id == ID; }), unicID.end());
            KSes.erase(std::remove_if(KSes.begin(), KSes.end(), [&ID](const CompressorStation& KS) { return KS.id == ID; }), KSes.end());
        }
        
        cout << "Станции успешно удалены" << endl;
        logger.log(LogLevel::INFO, "КС успешно удалены");
        break;
    case 0:
        return editKS(KSes,logger);
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
    std::string filename,logFileName;
    bool flag = true;

    string timeStr;
    SYSTEMTIME st;
    GetSystemTime(&st);
    timeStr = to_string(st.wYear) + "." + to_string(st.wMonth) + "." + to_string(st.wDay);
    logFileName = "logfile" + timeStr + ".txt";
    Logger logger(logFileName);
    logger.log(LogLevel::INFO, "Начало работы.");


    int choice;
    while (flag) {
        choice = menu_input(logger);

        switch (choice) {
        case 1:
            pipe.readFromConsole(logger);
            pipes.push_back(pipe);
            break;
        case 2:
            station.readFromConsole(logger);
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
            editPipes(pipes,logger);
            break;
        case 5:
            editKS(stations,logger);
            break;
        case 6:
            std::cout << "Введите имя файла для сохранения: ";
            std::cin >> filename;
            saveData(pipes, stations, filename,logger);
            break;
        case 7:
            std::cout << "Введите имя файла для загрузки: ";
            std::cin >> filename;
            loadData(pipes, stations, filename,logger);
            break;
        case 0:
            flag = false;
            break;
        default:
            std::cout << "Неверный выбор." << std::endl;
            logger.log(LogLevel::WARNING, "Неверный выбор.");
            break;
        }
    }
    std::cout << "Завершение работы...";
    logger.log(LogLevel::INFO, "Завершение работы.");
    return 0;
}