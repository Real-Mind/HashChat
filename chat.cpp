#include "chat.h"

Chat::Chat() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        data[i].occupied = false;  // ���������� ��� ������ �� ������
        data[i].deleted = false;    // ���������� ��� ������ �� �������
    }
}

int Chat::hash(const char* key) {
    // ����� ��������� ��� �����������
    float A = 0.6180339887;  // ������� �����
    unsigned int hashValue = static_cast<unsigned int>(TABLE_SIZE * (std::strlen(key) * A - std::floor(std::strlen(key) * A)));
    return hashValue % TABLE_SIZE;
}

int Chat::quadraticProbe(int index, int i) {
    return (index + i * i) % TABLE_SIZE;  // ������������ ������������
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
            data[index].deleted = false; // �������, ��� ������ ������
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
            return false; // ���� ��������� ������ ������, �� ����� �� ������
        }
        if (data[index].deleted) {
            continue; // ���������� ��������� ������
        }
        if (strcmp(data[index].login, _login) == 0) {
            uint* digest = sha1(const_cast<char*>(_pass), pass_length);
            bool result = !memcmp(data[index].pass_sha1_hash, digest, SHA1HASHLENGTHBYTES);
            delete[] digest;  // ������� ��������� ���
            return result;
        }
    }
    return false; // ����� �� ������
}

void Chat::remove(const char _login[LOGIN_LENGTH]) {
    int hashedIndex = hash(_login);

    for (int i = 0; i < TABLE_SIZE; i++) {
        int index = quadraticProbe(hashedIndex, i);
        if (!data[index].occupied) {
            return; // ����� �� ������
        }
        if (data[index].deleted) {
            continue; // ���������� ��������� ������
        }
        if (strcmp(data[index].login, _login) == 0) {
            data[index].deleted = true; // �������� ������ ��� ���������
            delete[] data[index].pass_sha1_hash; // ������� ���
            data[index].pass_sha1_hash = nullptr; // ������������� nullptr
            return;
        }
    }
}