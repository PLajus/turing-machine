#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <thread>

struct TuringCodeStruct
{
    std::string currstate = ""; // dabartine busena
    char currsymbol = '\0'; // dabartinis simbolis
    char newsymbol = '\0'; // naujas simbolis
    char direction = '\0'; // i kuria puse eiti
    std::string newstate = ""; // nauja busena
};

class Turing
{
public:
   void reading(std::ifstream &in);
   void simulating(int x, int y);

private:
    std::string input = "", filename = "";
    int headposition = 0;
    std::vector<TuringCodeStruct> TuringCode;
    TuringCodeStruct inputchars;
    std::string state = "0";
};

void Turing::reading(std::ifstream& in)
{
    std::cout << '\n' << "Iveskite ivedimo failo pavadinima: " << std::endl;
    std::cin >> filename;

    in.open(filename);
    if (!in) {
        std::cerr << "Nepavyko atidaryti failo";
        return;
    }
    in >> input >> headposition;
    std::cout << '\n' << "Ivestis: " << input << std::endl;
    --headposition;

    while (in >> inputchars.currstate >> inputchars.currsymbol >> inputchars.newsymbol >> inputchars.direction >> inputchars.newstate)
    {
        TuringCode.push_back(inputchars);
    }
    in.close();
}

void Turing::simulating(int x, int y)
{
    while (true)
    {
        bool changed = false;

        for (auto i : TuringCode)
        {
            if (i.currstate == state && input[headposition] == i.currsymbol)
            {
                changed = true;

                if (i.currsymbol != i.newsymbol)
                {
                    input[headposition] = i.newsymbol;

                    COORD coord;
                    coord.X = x;
                    coord.Y = y;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                    std::cout << input << std::flush;

                    Sleep(300);
                }
                state = i.newstate;

                if (i.direction == 'R')
                {
                    headposition++;
                }
                else
                {
                    headposition--;

                    if (headposition < 0)
                    {
                        return;
                    }
                    if (headposition > input.max_size())
                    {
                        return;
                    }
                }
                break;
            }
        }
        if (!changed)
        {
            return; // nera aprasyto atvejo su busena arba simboliu
        }
        if (_kbhit())
        {
            break;
        }
    }
}

void startthreads(std::vector<Turing> vec, int mas_kiek)
{
    int coordy = 0;

    switch (mas_kiek)
    {
    case 1:
        coordy = 12;
        break;
    case 2:
        coordy = 17;
        break;
    case 3:
        coordy = 22;
        break;
    case 4:
        coordy = 27;
        break;
    }

    Turing* turing_ptr1 = &vec[0];
    Turing* turing_ptr2 = &vec[1];
    Turing* turing_ptr3 = &vec[2];
    Turing* turing_ptr4 = &vec[3];

    std::thread thread1(&Turing::simulating, turing_ptr1, 0, coordy);
    Sleep(100);
    std::thread thread2(&Turing::simulating, turing_ptr2, 0, coordy+1);
    Sleep(100);
    std::thread thread3(&Turing::simulating, turing_ptr3, 0, coordy+2);
    Sleep(100);
    std::thread thread4(&Turing::simulating, turing_ptr4, 0, coordy+3);
    Sleep(100);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
}

int main()
{
    std::ifstream inFile;

    Turing ob1, ob2, ob3, ob4;

    std::vector<Turing> ob_vec = { ob1,ob2,ob3,ob4 };

    std::string pasirinkimas = "";

    system("CLS");
    int mas_kiek = 0;
    std::cout << "///////////// MENIU /////////////" << std::endl;
    std::cout << '\n';

    std::cout << "Kiek norite paleisti masinu (1-4)?" << std::endl;
    std::cin >> mas_kiek;
    if (mas_kiek < 1 || mas_kiek > 4)
    {
        std::cout << "Nera tokio pasirinkimo" << std::endl;
        return 0;
    }
    for (int i = 0; i < mas_kiek; i++)
    {
        ob_vec[i].reading(inFile);
    }

    std::cout << '\n' << "Paspauskite bet koki mygtuka, kad iseiti." << std::endl;
    std::cout << '\n';

    startthreads(ob_vec, mas_kiek);

    while(true)
    {
      Sleep(500);
      system("CLS");
      std::cout << "Ar norite grizti i meniu? (yes/no):";
      std::cin >> pasirinkimas;
      if (pasirinkimas != "yes" && pasirinkimas != "no")
      {
        std::cout << "Nera tokio pasirinkimo" << std::endl;
        continue;
      }
      if (pasirinkimas == "yes")
      {
        main();
      }
      else
      {
        exit(1);
      }
    }
   return 0;
}