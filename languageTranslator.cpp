#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

struct Translation
{
    string american;
    string english;
};

struct Date
{
    int day;
    int month;
    int year;
};

struct Person
{
    char name[20];
    double score;
    Date testTaken;
};

Translation* readTranslation(const string& filename, int& num);
void testersUpdates(const Translation t[], int numT, const string& fileName);
void takeTest(const Translation t[], int numT, Person& p);
void displayTesters(const string& fileName);

//****************************************************************************************************

int main ()
{
    int numTrans;

    short seed = time(0);
    srand(seed); 

    Translation* trans = nullptr; 
    trans = readTranslation("Translation.txt", numTrans);
    testersUpdates(trans, numTrans, "Testers.dat");
    displayTesters("Testers.dat");

    delete[] trans;

    return 0;
}

//****************************************************************************************************

Translation* readTranslation(const string& filename, int &num)
{
    ifstream f;
    f.open(filename);
    Translation* t = nullptr;

    if(f.is_open())
    {
        f >> num;
        f.ignore();

        t = new Translation[num];

        for (int i = 0; i < num; i++)
        {
            getline(f, t[i].american, ',');
            getline(f, t[i].english);
        }
    }
    else
    {
        cout << "File does not exist.\n";
    }

    return t;
}

void testersUpdates(const Translation t[], int numT, const string& fileName)
{
    int numP;
    int random;
    Person p;
    Date today;
    fstream f;

    f.open(fileName, ios:: in | ios:: out | ios::binary);

    f.read(reinterpret_cast<char*>(&numP), sizeof(int));

    cout << "Enter the today's date:\n";
    cout << "Day: ";
    cin >> today.day;
    cout << "Month: ";
    cin >> today.month;
    cout << "Year: ";
    cin >> today.year;

    for (int i = 0; i < 3; i++)
    {
        random = rand() % numP + 1;

        f.seekg(sizeof(int) + (random - 1) * sizeof(Person));

        f.read(reinterpret_cast<char*>(&p), sizeof(Person));

        takeTest(t, numT, p);

        p.testTaken = today;

        f.seekp(sizeof(int) + (random - 1) * sizeof(Person));

        f.write(reinterpret_cast<char*>(&p), sizeof(Person));
    }

    f.close();
}

void takeTest(const Translation t[], int numT, Person& p)
{
    const int NUM_QUEST = 10;
    int random;
    string answer;
    int total = 0;

    cout << "Test Taker: " << p.name << endl;

    for (int i = 0; i < NUM_QUEST; i++)
    {
        random = rand() % numT;

        cout << "Question " << i + 1 
             << ": Translate the word: " << t[random].american << " to English: ";
        cin >> answer;

        if (answer == t[random].english)
        {
            cout << "Correct!\n";
            total += 10;
        }
        else 
        {
            cout << "Incorrect answer. The correct answer was: " << t[random].english << endl;
        }
    }
    
    p.score = (total * 10.0) / NUM_QUEST;  
}

void displayTesters(const string& fileName)
{
    ifstream f;
    int numP;
    Person p;

    f.open(fileName, ios::binary);

    if(f.fail())
    {
        cout << "File does not exist.\n";
    }

    f.read(reinterpret_cast<char*>(&numP), sizeof(int));

    cout << left << setw(20) << "NAME" << setw(10) << "SCORE" << setw(15) << "TEST DATE" << endl;

    for (int i = 0; i < numP; i++)
    {
        f.read(reinterpret_cast<char*>(&p), sizeof(Person));

        cout << left << setw(20) << p.name << setw(10) << fixed << setprecision(1) << p.score
             << setw(2) << p.testTaken.day << '/' << setw(2) << p.testTaken.month << '/'
             << setw(4) << p.testTaken.year << endl;
    }

    f.close();
}