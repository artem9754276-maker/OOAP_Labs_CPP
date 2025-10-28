#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
using namespace std;

// ===============================================
//     ПРОТОТИП: ОБЛІК КІМНАТ У ГУРТОЖИТКУ
// ===============================================

// ===== КЛАС Resident (мешканець) =====
class Resident {
private:
    string name;
    string birthDate;
    string faculty;
    string group;
    string form;  // форма навчання (державна / контракт)
public:
    Resident(string n = "", string b = "", string f = "", string g = "", string fm = "")
        : name(n), birthDate(b), faculty(f), group(g), form(fm) {
    }

    void show() const {
        cout << "  ПІБ: " << name << endl
            << "  Дата народження: " << birthDate << endl
            << "  Факультет: " << faculty << endl
            << "  Група: " << group << endl
            << "  Форма навчання: " << form << endl;
    }

    string toString() const {
        return "ПІБ: " + name + "\nДата народження: " + birthDate +
            "\nФакультет: " + faculty + "\nГрупа: " + group +
            "\nФорма навчання: " + form + "\n";
    }
};

// ===== АБСТРАКТНИЙ ПРОТОТИП Room =====
class Room {
protected:
    string type; // тип кімнати
    vector<Resident> residents;
public:
    virtual ~Room() {}
    virtual unique_ptr<Room> clone() const = 0; // метод клонування (Prototype)
    virtual void addResident(const Resident& r) { residents.push_back(r); }

    virtual void showInfo() const {
        cout << "Тип кімнати: " << type << endl;
        cout << "Кількість мешканців: " << residents.size() << endl;
        for (size_t i = 0; i < residents.size(); ++i) {
            cout << "\n-- Мешканець " << i + 1 << " --" << endl;
            residents[i].show();
        }
    }

    virtual string toString() const {
        string info = "Тип кімнати: " + type + "\n";
        info += "Кількість мешканців: " + to_string(residents.size()) + "\n";
        for (size_t i = 0; i < residents.size(); ++i) {
            info += "\n-- Мешканець " + to_string(i + 1) + " --\n";
            info += residents[i].toString();
        }
        info += "===============================\n";
        return info;
    }
};

// ===== Конкретний прототип: двомісна кімната =====
class DoubleRoom : public Room {
public:
    DoubleRoom() { type = "Двомісна"; }
    DoubleRoom(const DoubleRoom& other) { *this = other; } // копіювання

    unique_ptr<Room> clone() const override {
        return make_unique<DoubleRoom>(*this);
    }
};

// ===== Конкретний прототип: тримісна кімната =====
class TripleRoom : public Room {
public:
    TripleRoom() { type = "Тримісна"; }
    TripleRoom(const TripleRoom& other) { *this = other; }

    unique_ptr<Room> clone() const override {
        return make_unique<TripleRoom>(*this);
    }
};

// ===== Клас Hostel (контейнер для кімнат) =====
class Hostel {
private:
    vector<unique_ptr<Room>> rooms;
public:
    void addRoom(unique_ptr<Room> room) {
        rooms.push_back(move(room));
    }

    void showAll() const {
        cout << "=== ІНФОРМАЦІЯ ПРО ВСІ КІМНАТИ ===" << endl;
        for (const auto& r : rooms) {
            r->showInfo();
            cout << "-------------------------------\n";
        }
    }

    void generateReport(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cerr << "Помилка відкриття файлу!" << endl;
            return;
        }
        for (const auto& r : rooms)
            out << r->toString();
        out.close();
        cout << "Звіт збережено у файлі: " << filename << endl;
    }
};

// ===== MAIN =====
int main() {
	setlocale(LC_ALL, "UKRAINIAN");
    // Створюємо прототипи кімнат
    DoubleRoom doubleProto;
    TripleRoom tripleProto;

    // Створюємо об'єкт гуртожитку
    Hostel hostel;

    // --- Кімната 1 (на основі двомісного прототипу) ---
    auto room1 = doubleProto.clone();
    room1->addResident(Resident("Іваненко Іван", "2003-02-11", "ІТ", "КНз-21", "державна"));
    room1->addResident(Resident("Петренко Марія", "2004-04-21", "ІТ", "КНз-21", "контракт"));
    hostel.addRoom(move(room1));

    // --- Кімната 2 (на основі тримісного прототипу) ---
    auto room2 = tripleProto.clone();
    room2->addResident(Resident("Сидоренко Олег", "2002-12-10", "Лісогосп.", "ЛГ-11", "державна"));
    room2->addResident(Resident("Білик Анна", "2003-07-05", "Лісогосп.", "ЛГ-11", "контракт"));
    room2->addResident(Resident("Демчук Павло", "2002-10-09", "Лісогосп.", "ЛГ-11", "державна"));
    hostel.addRoom(move(room2));

    // --- Кімната 3 (ще один клон двомісної) ---
    auto room3 = doubleProto.clone();
    room3->addResident(Resident("Коваль Юлія", "2003-03-03", "Економіка", "ЕК-12", "контракт"));
    room3->addResident(Resident("Гринюк Назар", "2003-06-20", "Економіка", "ЕК-12", "контракт"));
    hostel.addRoom(move(room3));

    // Виведення інформації у консоль
    hostel.showAll();

    // Формування текстового звіту
    hostel.generateReport("HostelReport.txt");

    return 0;
}
