#include "chat.h"

int main() {
    Chat chat;

    // Регистрация новых пользователей
    const char user1[] = "user1";
    const char pass1[] = "password123";
    chat.reg(user1, pass1, strlen(pass1));

    const char user2[] = "user2";
    const char pass2[] = "mysecurepassword";
    chat.reg(user2, pass2, strlen(pass2));

    // Попытка входа в систему
    if (chat.login(user1, pass1, strlen(pass1))) {
        std::cout << "User 1 logged in successfully!" << std::endl;
    }
    else {
        std::cout << "User 1 login failed." << std::endl;
    }

    if (chat.login(user2, pass2, strlen(pass2))) {
        std::cout << "User 2 logged in successfully!" << std::endl;
    }
    else {
        std::cout << "User 2 login failed." << std::endl;
    }

    // Удаление пользователя
    chat.remove(user1);
    std::cout << "User 1 has been removed." << std::endl;

    // Проверка входа после удаления
    if (chat.login(user1, pass1, strlen(pass1))) {
        std::cout << "User 1 logged in successfully!" << std::endl;
    }
    else {
        std::cout << "User 1 login failed after removal." << std::endl;
    }

    return 0;
}