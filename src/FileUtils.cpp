// basic file operations
// Radoi Alex

#include <iostream>
#include <fstream>
#include <locale>

int test () {
    float cont = 0;
    float num;
    float total = 0;
    float aver;

    std::ifstream myfile; //input
    std::ofstream another_useless_file; //output

    another_useless_file.open("uselessness.txt"); //deschidem fisierul de iesire
    myfile.open("eu.txt"); //deschidem si citim din fisierul sursa

    // float a, b, c;

    while(myfile >> num)
    {
        cont++; //incrementam un contor pt cate date gasim
        total += num; //facem suma datelor
        std::cout << num << std::endl; //verificare in consola ca totul merge bine
        // cout << total << endl;
        another_useless_file << num << std::endl;
    }

    std::cout << cont << std::endl;
    aver = total / cont;
    std::cout << "The average is" << std::endl;
    std::cout << aver << std::endl;

    another_useless_file << "The average was: " <<  aver << std::endl; // afisam media aritmetica
    myfile.close();
    another_useless_file.close();

    return 0;
}
