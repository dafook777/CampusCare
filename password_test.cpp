
#include <iostream>
#include <string>
#include <sodium.h>

using namespace std;

int main() {

    if (sodium_init() < 0) {
        cout << "Failed to initialize libsodium.\n";
        return 1;
    }

    string password;

    cout << "Enter a password: ";
    getline(cin, password);

    char hashedPassword[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
            hashedPassword,
            password.c_str(),
            password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE
        ) != 0) {

        cout << "Password hashing failed.\n";
        return 1;
    }

    cout << "\nGenerated password hash:\n";
    cout << hashedPassword << '\n';

    if (crypto_pwhash_str_verify(
            hashedPassword,
            password.c_str(),
            password.length()
        ) == 0) {

        cout << "\nPassword verified successfully!\n";
    }
    else {
        cout << "\nPassword verification failed!\n";
    }

    return 0;
}
