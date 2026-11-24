
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Match {
    int homeGoals;
    int awayGoals;
    string homeTeam;
    string awayTeam;
    int halfHome;
    int halfAway;
    int yellowCards;
    int redCards;
};

// function declarations
void readMatches(vector<Match>& matches);
void printMatch(const Match& m);
void printMatches(const vector<Match>& matches);
void calculateAverage(const vector<Match>& matches);
void findFewestGoals(const vector<Match>& matches);
void findComebacks(const vector<Match>& matches);
void countWins(const vector<Match>& matches);
void saveUnder40(const vector<Match>& matches);

// main function
int main() {
    srand(time(0));

    vector<Match> matches;

    readMatches(matches);

    printMatches(matches);
    calculateAverage(matches);
    findFewestGoals(matches);
    findComebacks(matches);
    countWins(matches);
    saveUnder40(matches);

    return 0;
}

// read matches from file
void readMatches(vector<Match>& matches) {
    ifstream file("merkozesek.txt");

    if (!file) {
        cout << "Nem sikerult megnyitni a fajlt!" << endl;
        return;
    }

    Match m;

    while (file >> m.homeGoals >> m.awayGoals
        >> m.homeTeam >> m.awayTeam
        >> m.halfHome >> m.halfAway) {

        m.yellowCards = rand() % 9 + 2;

        do {
            cout << m.homeTeam << " - " << m.awayTeam
                << " piroslapok szama (0-4): ";
            cin >> m.redCards;
        } while (m.redCards < 0 || m.redCards > 4);

        matches.push_back(m);
    }

    file.close();

    cout << "\nOsszesen " << matches.size()
        << " merkozes volt.\n";
}

// print one match
void printMatch(const Match& m) {
    cout << m.homeTeam << " - " << m.awayTeam
        << " " << m.homeGoals << " : " << m.awayGoals
        << " (" << m.halfHome << " : " << m.halfAway << ")"
        << " sargalap: " << m.yellowCards
        << " piroslap: " << m.redCards << endl;
}

// print all matches
void printMatches(const vector<Match>& matches) {
    cout << "\nMerkozesek adatai:\n";

    for (const Match& m : matches) {
        printMatch(m);
    }
}

// calculate average home goals
void calculateAverage(const vector<Match>& matches) {
    if (matches.empty()) return;

    int sum = 0;

    for (const Match& m : matches) {
        sum += m.homeGoals;
    }

    double average = (double)sum / matches.size();

    cout << "\nHazai csapatok golatlaga: "
        << fixed << setprecision(2) << average << endl;
}

// find match with the fewest goals
void findFewestGoals(const vector<Match>& matches) {
    if (matches.empty()) return;

    int minGoals = matches[0].homeGoals + matches[0].awayGoals;
    int index = 0;

    for (int i = 1; i < matches.size(); i++) {
        int goals = matches[i].homeGoals + matches[i].awayGoals;

        if (goals < minGoals) {
            minGoals = goals;
            index = i;
        }
    }

    cout << "\nLegkevesebb golos merkozes:\n";
    printMatch(matches[index]);
}

// find comebacks
void findComebacks(const vector<Match>& matches) {
    bool found = false;

    cout << "\nForditasok:\n";

    for (const Match& m : matches) {
        if ((m.halfHome < m.halfAway &&
            m.homeGoals > m.awayGoals) ||
            (m.halfAway < m.halfHome &&
                m.awayGoals > m.homeGoals)) {

            printMatch(m);
            found = true;
        }
    }

    if (!found) {
        cout << "Nem volt olyan csapat, amelyik forditani tudott!"
            << endl;
    }
}

// count home and away wins
void countWins(const vector<Match>& matches) {
    int homeWins = 0;
    int awayWins = 0;

    for (const Match& m : matches) {
        if (m.homeGoals > m.awayGoals)
            homeWins++;
        else if (m.homeGoals < m.awayGoals)
            awayWins++;
    }

    cout << "\nHazai gy: " << homeWins
        << " vendeg gy: " << awayWins << endl;

    if (homeWins > awayWins)
        cout << "Hazai gyozelembol volt tobb." << endl;
    else if (awayWins > homeWins)
        cout << "Vendeg gyozelembol volt tobb." << endl;
    else
        cout << "Egyenlo volt a hazai es vendeg gyozelmek szama."
        << endl;
}

// save matches with fewer than 40 goals
void saveUnder40(const vector<Match>& matches) {
    vector<Match> selected;

    for (const Match& m : matches) {
        if (m.homeGoals + m.awayGoals < 40) {
            selected.push_back(m);
        }
    }

    ofstream file("kevesebb40.txt");

    cout << "\n40-nel kevesebb gol:\n";

    for (const Match& m : selected) {
        cout << m.homeTeam << " - " << m.awayTeam
            << " " << m.homeGoals << " : " << m.awayGoals
            << " (" << m.halfHome << " : " << m.halfAway << ")"
            << endl;

        file << m.homeTeam << " " << m.awayTeam << " "
            << m.homeGoals << " " << m.awayGoals << " "
            << m.halfHome << " " << m.halfAway << endl;
    }

    file.close();
}