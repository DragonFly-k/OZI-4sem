#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

int power(int a, int b, int p)// a ^ b ( mod P )
{
    int tmp = a;
    int sum = tmp;
    for (int i = 1; i < b; i++)
    {
        for (int j = 1; j < a; j++)
        {
            sum += tmp;
            if (sum >= p)
                sum -= p;
        }
        tmp = sum;
    }
    return tmp;
}

int mul(int a, int b, int n)// a * b mod n 
{
    int sum = 0;
    for (int i = 0; i < b; i++)
    {
        sum += a;
        if (sum >= n)
            sum -= n;
    }
    return sum;
}

void crypt(int p, int g, int x, string inFileName, string outFileName)
{
    ifstream inf(inFileName.c_str());
    ofstream outf(outFileName.c_str());
    int y = power(g, x, p);    // y = g ^ x (mod p)
    cout << "Открытый ключ (p, g, y) = " << "(" << p << ", " << g << ", " << y << ")"<<endl;
    cout << "Закрытый ключ x = " << x;
    cout << "\nШифруемый текст: ";
    while (inf.good())
    {
        int m = inf.get();
        if (m > 0)
        {
            cout << (char)m;
            int k = rand() % (p - 2) + 1;       // случайное число 1 < k < (p - 1) 
            int a = power(g, k, p);             // а = g ^ k (mod p)
            int b = mul(power(y, k, p), m, p);  // b = m * y ^ k (mod p)
            outf << a << " " << b << " ";       // пара {a, b} — шифротекст
        }
    }
    cout << endl;
    inf.close();
    outf.close();
}

void decrypt(int p, int x, string inFileName, string outFileName)
{
    ifstream inf(inFileName.c_str());
    ofstream outf(outFileName.c_str());
    cout << "Дешифрованый текст: ";
    while (inf.good())
    {
        int a = 0;
        int b = 0;
        inf >> a;
        inf >> b;
        if (a != 0 && b != 0)
        {
            int deM = mul(b, power(a, p - 1 - x, p), p);
            char m = static_cast<char>(deM);
            outf << m;      // m = b * a ^ (p - 1 - x) mod p
            cout << m;      
        }
    }
    cout << endl;
    inf.close();
    outf.close();
}

int main()
{
    setlocale(LC_ALL, "rus");
    cout << "Задание №1  ---Алгоритм Диффи-Хеллмана---\n";
    int publicAlice = 67;
    int publicBob = 73;
    int privateAlice = 101;
    int privateBob = 89;
    cout << "Публичный ключ Алисы:\t" << publicAlice << endl;
    cout << "Публичный ключ Боба:\t" << publicBob << endl << endl;
    cout << "Приватный ключ Алисы:\t" << privateAlice << endl;
    cout << "Приватный ключ Боба:\t" << privateBob << endl << endl;

    int partialAlice = power(publicAlice, privateAlice, publicBob);
    int partialBob = power(publicAlice, privateBob, publicBob);
    cout << "Открытый ключ Алисы:\t" << partialAlice << endl;
    cout << "Открытый ключ Боба:\t" << partialBob << endl << endl;

    int fullAlice = power(partialBob, privateAlice, publicBob);
    int fullBob = power(partialAlice, privateBob, publicBob);
    cout << "Полный ключ Алисы:\t" << fullAlice << endl;
    cout << "Полный ключ Боба:\t" << fullBob << endl ;

    cout << "\nЗадание №2  ---RSA--- \n";
    int p = 191, q = 193;
    cout << "p = " << p << "\tq = " << q << endl;
    int n = p * q;
    int l = (p - 1) * (q - 1);
    int e = 7;
    int d = (2*l+1)/e; //d=e^(-1)mod l <==> d=(k*l+1)/e
    cout << "n = " << n << "\tl = " << l << endl;
    cout << "e = " << e << "\td = " << d << endl;

    int msg;
    cout << "Текст: ";
    cin>> msg;

    // Пара {e, n} — октрытый ключ
    int c = power(msg, e, n);//msg^E % N
    cout << "Зашифрованный текст:  " << c << endl;
    // Пара {d, c} — закрытый ключ
    int m = power(c, d, n);//С^D % N 
    cout << "Расшифрованный текст: " << m << endl;

    cout << "\nЗадание №3  ---Алгоритм Эль-Гамаля--- \n";
    srand(time(NULL));
    crypt(593, 123, 8, "in.txt", "crypt.txt");
    decrypt(593, 8, "crypt.txt", "decrypt.txt");

    return 0;
}