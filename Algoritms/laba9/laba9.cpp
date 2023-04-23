#include <sstream> // required for 'stringstream'
#include <iostream> // required for 'cout'
#include <iomanip> // required for 'setfill'
#include <random>
#include <math.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "Source.cpp"

using namespace std;

// функция перевода 16-ого числа в строку 
string uint8_to_hex_string(const uint8_t* v, const size_t s) {
    stringstream ss;

    ss << hex << setfill('0');

    for (int i = 0; i < s; i++) {
        ss << hex << setw(2) << static_cast<int>(v[i]);
    }

    return ss.str();
}

// генерация случайной строки
string generationString(size_t len) {
    string str = "";
    for (int i = 0; i < len; i++) {
        str += (char)(97 + rand() % 26);
    }
    return str;
}

// замена нескольких элементов в строке
string changeString(string str, int count) {
    int* arr = new int[count];
    for (int i = 0; i < count; i++) {
        arr[i] = rand() % str.length();
        if (find(arr, arr + i, arr[i]) != arr + i) {
            i--;
        }
    }
    for (int i = 0; i < count; i++) {
        str[arr[i]] = (char)(((toascii(str[arr[i]]) - 'a' + rand() % 25) % 26) + 'a');
    }
    delete[] arr;
    return str;
}

int main() {
    srand(time(0));

    ofstream out;
    out.open("C:\\Users\\evgen\\Desktop\\Алгоритмы\\data9.txt");

    const size_t str1_len = 128; // длина строки 1
    const size_t str2_len = 128; // длина строки 2
    uint8_t msg1[str1_len]; // строка 1
    uint8_t msg2[str2_len]; // строка 2
    const size_t hash1_len = 20; // длина хэша 1
    const size_t hash2_len = 20; // длина хэша 2
    uint8_t hash1[hash1_len]; // хэш 1
    uint8_t hash2[hash2_len]; // хэш 2
    string str1, str2;
    int N;
    for (int h = 0; h < 5; h++) {
        N = pow(2, h);
        for (int k = 0; k < 1000; k++) {
            cout << "\nk = " << k << " N = " << N << endl;
            str1 = generationString(str1_len); // генерируем строку 1
            str2 = str1;
            
            str2 = changeString(str2, N); // меняемм в строке 2 несколько символов

            memcpy(msg1, str1.c_str(), sizeof(msg1)); // выделяем память под сообщение
            memcpy(msg2, str2.c_str(), sizeof(msg2));

            ripemd160(msg1, str1_len, hash1); // хэшируем строки
            ripemd160(msg2, str2_len, hash2);

            string hexstr1 = uint8_to_hex_string(hash1, hash1_len); // представляем хэш в строковом виде
            string hexstr2 = uint8_to_hex_string(hash2, hash2_len);

            int count = 0, maxcount = 0;
            
            //поиск одинаковой последовательности символов
            for (int i = 0; i < hexstr1.length(); i++) {
                for (int j = 0; j < hexstr2.length(); j++) {
                    count = 0;
                    // пока символы совпадают
                    while (hexstr1[i] == hexstr2[j] && i < hexstr1.length() && j < hexstr2.length()) {
                        count++;
                        i++;
                        j++;
                    }
                    if (count > maxcount) // проверка на максимальную длину
                        maxcount = count;
                }
            }
            out << maxcount << endl;
        }
    }

    out << "\n\n------------------------\n\n";

    const size_t str_len = 256; // длина строки
    uint8_t msg[str_len]; // строка
    const size_t hash_len = 20; // длина хэша
    uint8_t hash[hash_len]; // хэш
    string str;
    vector<string> vec_strs;
    int count = 0;

    for (int h = 2; h < 7; h++) {
        N = pow(10, h);
        for (int k = 0; k < N; k++) {
            cout << "\nk = " << k << " N = " << N << endl;
            str = generationString(str_len); // генерируем строку
            memcpy(msg, str.c_str(), sizeof(msg)); // выделяем память
            ripemd160(msg, str_len, hash); // хэшируем строку
            string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
            vec_strs.push_back(hexstr); // добавляем хэш в вектор

        }

        sort(vec_strs.begin(), vec_strs.end()); // сортируем по возрастанию вектор
        int count = 0;
        for (int i = 0; i < N - 1; i++) {
            if (vec_strs[i] == vec_strs[i + 1]) // сравниваем соседние элементы
                count++; // коллизия
        }
        out << count << endl;
    }

    out << "\n\n------------------------\n\n";

    const size_t str_len1 = 64; // длина строки
    uint8_t msg_1[str_len1]; // строка
    
    chrono::high_resolution_clock::time_point begin;
    chrono::high_resolution_clock::time_point end;
    chrono::duration<double, milli> milli_diff;

    double all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len1);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_1, str.c_str(), sizeof(msg_1)); // выделяем память
        ripemd160(msg_1, str_len1, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len1 << " " << all_time / 1000 << endl;

    const size_t str_len2 = 128; // длина строки
    uint8_t msg_2[str_len2]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len2);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_2, str.c_str(), sizeof(msg_2)); // выделяем память
        ripemd160(msg_2, str_len2, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len);
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len2 << " " << all_time / 1000 << endl;

    const size_t str_len3 = 256; // длина строки
    uint8_t msg_3[str_len3]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len3);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_3, str.c_str(), sizeof(msg_3)); // выделяем память
        ripemd160(msg_3, str_len3, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len3 << " " << all_time / 1000 << endl;

    const size_t str_len4 = 512; // длина строки
    uint8_t msg_4[str_len4]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len4);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_4, str.c_str(), sizeof(msg_4)); // выделяем память
        ripemd160(msg_4, str_len4, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len4 << " " << all_time / 1000 << endl;

    const size_t str_len5 = 1024; // длина строки
    uint8_t msg_5[str_len5]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len5);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_5, str.c_str(), sizeof(msg_5)); // выделяем память
        ripemd160(msg_5, str_len5, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len5 << " " << all_time / 1000 << endl;

    const size_t str_len6 = 2048; // длина строки
    uint8_t msg_6[str_len6]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len6);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_6, str.c_str(), sizeof(msg_6)); // выделяем память
        ripemd160(msg_6, str_len6, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len6 << " " << all_time / 1000 << endl;

    const size_t str_len7 = 4096; // длина строки
    uint8_t msg_7[str_len7]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len7);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_7, str.c_str(), sizeof(msg_7)); // выделяем память
        ripemd160(msg_7, str_len7, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len7 << " " << all_time / 1000 << endl;

    const size_t str_len8 = 8192; // длина строки
    uint8_t msg_8[str_len8]; // строка

    all_time = 0;
    for (int i = 0; i < 1000; i++) {
        cout << "\ni = " << i << endl;
        str = generationString(str_len8);
        begin = chrono::high_resolution_clock::now(); // стартовое время
        memcpy(msg_8, str.c_str(), sizeof(msg_8)); // выделяем память
        ripemd160(msg_8, str_len8, hash); // хэшируем строку
        string hexstr = uint8_to_hex_string(hash, hash_len); // представляем хэш в строковом виде
        end = chrono::high_resolution_clock::now(); // конечное время
        //milli_diff = end - begin; // время в микросекундах
        all_time += (end - begin).count();
    }
    out << str_len8 << " " << all_time / 1000 << endl;

    out.close();
    return 0;
}
