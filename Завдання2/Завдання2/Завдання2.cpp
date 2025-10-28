#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

class IPhone {
public:
    virtual ~IPhone() = default;
    virtual string modelName() const = 0;
    virtual string country() const = 0;
    virtual double price() const = 0;
    virtual int deliveryDays() const = 0;

    virtual void print() const {
        cout << "Модель: " << modelName()
            << " | Виробник: " << country()
            << " | Ціна: " << fixed << setprecision(2) << price() << " $"
            << " | Доставка: " << deliveryDays() << " днів" << endl;
    }
};

class PhoneBase : public IPhone {
protected:
    string _model;
    string _country;
    double _basePrice;
    double _coef;
    int _delivery;
public:
    PhoneBase(string model, string country, double basePrice, double coef, int delivery)
        : _model(model), _country(country), _basePrice(basePrice), _coef(coef), _delivery(delivery) {
    }

    string modelName() const override { return _model; }
    string country() const override { return _country; }
    double price() const override { return _basePrice * _coef; }
    int deliveryDays() const override { return _delivery; }
};

class PhoneA : public PhoneBase {
public:
    PhoneA(string country, double coef, int delivery)
        : PhoneBase("IPhone 17", country, 799.0, coef, delivery) {
    }
};

class PhoneB : public PhoneBase {
public:
    PhoneB(string country, double coef, int delivery)
        : PhoneBase("IPhone 17 Air", country, 999.0, coef, delivery) {
    }
};

class PhoneC : public PhoneBase {
public:
    PhoneC(string country, double coef, int delivery)
        : PhoneBase("IPhone 17 Pro", country, 1099.0, coef, delivery) {
    }
};

class PhoneD : public PhoneBase {
public:
    PhoneD(string country, double coef, int delivery)
        : PhoneBase("IPhone 17 Pro Max", country, 1199.0, coef, delivery) {
    }
};

class IPhoneFactory {
public:
    virtual ~IPhoneFactory() = default;
    virtual unique_ptr<IPhone> createA() const = 0;
    virtual unique_ptr<IPhone> createB() const = 0;
    virtual unique_ptr<IPhone> createC() const = 0;
    virtual unique_ptr<IPhone> createD() const = 0;
    virtual string countryName() const = 0;
    virtual int deliveryDays() const = 0;
    virtual double coef() const = 0;
};

class USFactory : public IPhoneFactory {
public:
    string countryName() const override { return "USA"; }
    int deliveryDays() const override { return 3; }
    double coef() const override { return 1.20; } // +20%
    unique_ptr<IPhone> createA() const override { return make_unique<PhoneA>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createB() const override { return make_unique<PhoneB>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createC() const override { return make_unique<PhoneC>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createD() const override { return make_unique<PhoneD>(countryName(), coef(), deliveryDays()); }
};

class IsraelFactory : public IPhoneFactory {
public:
    string countryName() const override { return "Israel"; }
    int deliveryDays() const override { return 10; }
    double coef() const override { return 0.85; } // -15%
    unique_ptr<IPhone> createA() const override { return make_unique<PhoneA>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createB() const override { return make_unique<PhoneB>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createC() const override { return make_unique<PhoneC>(countryName(), coef(), deliveryDays()); }
    unique_ptr<IPhone> createD() const override { return make_unique<PhoneD>(countryName(), coef(), deliveryDays()); }
};

class Store {
    const IPhoneFactory& factory;
public:
    explicit Store(const IPhoneFactory& f) : factory(f) {}
    unique_ptr<IPhone> order(const int id) const {
        switch (id) {
        case 1: return factory.createA();
        case 2: return factory.createB();
        case 3: return factory.createC();
        case 4: return factory.createD();
        default:
            throw runtime_error("Невідомий вибір моделі");
        }
    }
};

int main() {
    setlocale(LC_ALL, "Ukrainian");
    cout << "=== Інтернет-магазин телефонів - Arterium ===\n";
    cout << "Оберіть країну виробника:\n";
    cout << "1 - USA (Ціна більша, швидка доставка)\n";
    cout << "2 - Israel (Ціна нижче, повільна доставка)\n> ";

    int choice;
    cin >> choice;

    unique_ptr<IPhoneFactory> factory;
    if (choice == 1) factory = make_unique<USFactory>();
    else if (choice == 2) factory = make_unique<IsraelFactory>();
    else {
        cout << "Невірний вибір країни\n";
        return 0;
    }

    cout << "\nОберіть модель:\n";
    cout << "1 - IPhone 17\n";
    cout << "2 - IPhone 17 Air\n";
    cout << "3 - IPhone 17 Pro\n";
    cout << "4 - IPhone 17 Pro Max\n> ";

    int modelId;
    cin >> modelId;

    Store shop(*factory);

    try {
        auto phone = shop.order(modelId);
        cout << "\nВаш вибір:\n";
        phone->print();
    }
    catch (const exception& e) {
        cout << "Помилка: " << e.what() << endl;
    }

    return 0;
}
