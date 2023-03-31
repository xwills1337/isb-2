#include <random>
#include <math.h>
#include <iostream>

using namespace std;

int* generator()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 1);
    int* mas = new int[128];
    for (int i = 0; i < 128; ++i)
    {
        mas[i] = dist(gen);
    }
    return mas;
}

double test_NIST_1(int* mas)
{
    double S = 0;
    for (int i = 0; i < 128; i++)
    {
        if (mas[i] == 1) S++;
        else S--;
    }
    S = S / (sqrt(128.0));
    return erfc(S / (sqrt(2)));
}

double test_NIST_2(int* mas)
{
    double S = 0;
    for (int i = 0; i < 128; i++)
    {
        S += mas[i];
    }
    S = S / 128;
    if (abs(S - 0.5) >= (2 / (sqrt(128)))) return 0;

    double V = 0;
    for (int i = 0; i < 127; i++)
    {
        if (mas[i] != mas[i + 1]) V++;
    }

    return erfc(abs(V - (256 * S * (1 - S))) / (2 * sqrt(256) * S * (1 - S)));
}

double test_NIST_3(int* mas)
{
    int v[4] = { 0, 0, 0 ,0 };
    for (int i = 0; i < 16; i++)
    {
        int l = 1, l_max = 1;
        for (int j = 0; j < 7; j++)
        {
            if (mas[i * 8 + j] == 1 && mas[i * 8 + j + 1] == 1) l++;
            else
            {
                if (l > l_max) l_max = l;
                l = 1;
            }
        }
        if (l > l_max) l_max = l;
        l = 1;
        if (l_max == 1) v[0]++;
        if (l_max == 2) v[1]++;
        if (l_max == 3) v[2]++;
        if (l_max >= 4) v[3]++;
    }

    double pi[4] = { 0.2148, 0.3672, 0.2305, 0.1875 };

    double V = 0;
    for (int i = 0; i < 4; i++)
    {
        V += (pow((v[i] - (16 * pi[i])), 2) / (16 * pi[i]));
    }

    return V / 2;
}

int main()
{
    int* mas = generator();
    for (int i = 0; i < 128; ++i)
    {
        cout << mas[i] << " ";
    }
    cout << "\n";
    cout << test_NIST_1(mas) << "\n";
    cout << test_NIST_2(mas) << "\n";
    cout << test_NIST_3(mas) << "\n";
}