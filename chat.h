#pragma once

#include "sha1.h"
#include <iostream>
#include <cstring>
#include <cmath>

#define TABLE_SIZE 10
#define LOGIN_LENGTH 10

class Chat {
public:
    Chat();
    void reg(const char _login[LOGIN_LENGTH], const char _pass[], int pass_length);
    bool login(const char _login[LOGIN_LENGTH], const char _pass[], int pass_length);
    void remove(const char _login[LOGIN_LENGTH]);  // ����� ����� ��� ��������

private:
    struct AuthData {
        char login[LOGIN_LENGTH] = "";
        uint* pass_sha1_hash;
        bool occupied;  // ���� ��������� ������
        bool deleted;   // ���� ��� ��������� ������

        AuthData() : pass_sha1_hash(nullptr), occupied(false), deleted(false) {}
        ~AuthData() {
            if (pass_sha1_hash != nullptr) {
                delete[] pass_sha1_hash;
            }
        }
    };

    AuthData data[TABLE_SIZE];

    int hash(const char* key);  // ���-�������
    int quadraticProbe(int index, int i);  // ������������ ������������
};