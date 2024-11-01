#include "chat.h"

Chat::Chat() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        data[i].occupied = false;  // »значально все €чейки не зан€ты
        data[i].deleted = false;    // »значально все €чейки не удалены
    }
}

int Chat::hash(const char* key) {
    // ћетод умножени€ дл€ хешировани€
    float A = 0.6180339887;  // «олотое число
    unsigned int hashValue = static_cast<unsigned int>(TABLE_SIZE * (std::strlen(key) * A - std::floor(std::strlen(key) * A)));
    return hashValue % TABLE_SIZE;
}

int Chat::quadraticProbe(int index, int i) {
    return (index + i * i) % TABLE_SIZE;  //  вадратичное пробирование
}

void Chat::reg(const char _login[LOGIN_LENGTH], const char _pass[], int pass_length) {
    uint* password_hash = sha1(const_cast<char*>(_pass), pass_length);
    int hashedIndex = hash(_login);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = quadraticProbe(hashedIndex, i);
        if (!data[index].occupied || data[index].deleted) {
            strcpy_s(data[index].login, sizeof(data[index].login), _login);
            data[index].pass_sha1_hash = password_hash;
            data[index].occupied = true;
            data[index].deleted = false; // ќтметим, что €чейка зан€та
            return;
        }
    }

    std::cerr << "Error: Hash table is full." << std::endl;
}

bool Chat::login(const char _login[LOGIN_LENGTH], const char _pass[], int pass_length) {
    int hashedIndex = hash(_login);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = quadraticProbe(hashedIndex, i);
        if (!data[index].occupied) {
            return false; // ≈сли встретили пустую €чейку, то логин не найден
        }
        if (data[index].deleted) {
            continue; // ѕропускаем удаленные €чейки
        }
        if (strcmp(data[index].login, _login) == 0) {
            uint* digest = sha1(const_cast<char*>(_pass), pass_length);
            bool result = !memcmp(data[index].pass_sha1_hash, digest, SHA1HASHLENGTHBYTES);
            delete[] digest;  // ќчищаем временный хеш
            return result;
        }
    }
    return false; // Ћогин не найден
}

void Chat::remove(const char _login[LOGIN_LENGTH]) {
    int hashedIndex = hash(_login);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = quadraticProbe(hashedIndex, i);
        if (!data[index].occupied) {
            return; // Ћогин не найден
        }
        if (data[index].deleted) {
            continue; // ѕропускаем удаленные €чейки
        }
        if (strcmp(data[index].login, _login) == 0) {
            data[index].deleted = true; // ќтмечаем €чейку как удаленную
            delete[] data[index].pass_sha1_hash; // ќчищаем хеш
            data[index].pass_sha1_hash = nullptr; // ”станавливаем nullptr
            return;
        }
    }
}