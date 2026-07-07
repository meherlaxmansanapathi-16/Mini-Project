#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;


class Question {
protected:
    string question;
    int marks;

public:
    Question(string q, int m) {
        question = q;
        marks = m;
    }

    virtual void DisplayQuestion() {
        cout << question << endl;
    }

    virtual int evaluateAnswer(string ans) = 0;
};


class MCQquestion : public Question {
private:
    string A, B, C, D;
    string correct;

public:
    MCQquestion(string q, int m, string a, string b, string c, string d, string ans)
        : Question(q, m) {
        A = a;
        B = b;
        C = c;
        D = d;
        correct = ans;
    }

    void DisplayQuestion() override {

        cout << question << endl;

        cout << "A. " << A << endl;

        cout << "B. " << B << endl;

        cout << "C. " << C << endl;
        
        cout << "D. " << D << endl;
    }

    int evaluateAnswer(string ans) override {
        return (ans == correct) ? marks : 0;
    }
};


class TFQuestion : public Question {
private:
    string correct;

public:
    TFQuestion(string q, int m, string c)
        : Question(q, m) {
        correct = c;
    }

    void DisplayQuestion() override {
        cout << question << " (true/false)" << endl;
    }

    int evaluateAnswer(string ans) override {
        return (ans == correct) ? marks : 0;
    }
};


class Player {
private:
    string name;
    int score;

public:
    Player(string n) {
        name = n;
        score = 0;
    }

    void addScore(int s) {
        score += s;
    }

    int getScore() const {
        return score;
    }

    string getName() const {
        return name;
    }
};


class Quiz {
private:
    vector<Question*> questions;

public:
    void setQuestions(vector<Question*> qlist) {
        questions = qlist;
    }

    void startQuiz(Player &p) {
        string ans;
        int examTime = 120;

        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < questions.size(); i++) {

            auto now = chrono::high_resolution_clock::now();
            int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();

            if (elapsed >= examTime) {
                cout << "\n=====================\n";
                cout << "EXAM TIME OVER!\n";
                cout << "Remaining questions skipped.\n";
                break;
            }

            cout << "\n----------------------\n";

            
            questions[i]->DisplayQuestion();

            cout << "Enter answer: ";
            cin >> ans;

            now = chrono::high_resolution_clock::now();
            elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();

            if (elapsed < examTime) {


                p.addScore(questions[i]->evaluateAnswer(ans));
            }
        }

        cout << "\nExam Finished\n";
        
    }
};


bool compare(const Player &a, const Player &b) {


    return a.getScore() > b.getScore();
}

class Leaderboard {
private:
    vector<Player> players;

public:
    void addPlayer(Player p) {
        players.push_back(p);
    }

    void sortLeaderboard() {
        sort(players.begin(), players.end(), compare);
    }

    void display() {
        cout << "\n===== LEADERBOARD =====\n";
        for (int i = 0; i < players.size(); i++) {


            cout << i + 1 << ". "
                 << players[i].getName()
                 << " - " << players[i].getScore() << endl;
        }
    }
};


int main() {

    Leaderboard lb;

    int n;
    cout << "\nEnter number of players: ";
    cin >> n;

    for (int i = 0; i < n; i++) {

        string name;
        cout << "\nEnter player name: ";
        cin >> name;

        cout << "\nYou have total 2 minutes to complete this exam\n";

        Player p(name);

       
        vector<Question*> qlist;

        qlist.push_back(new MCQquestion("Which concept of OOP hides the internal details of a class?",5,"Inheritance","Polymorphism","Encapsulation","Abstraction","D"));

        qlist.push_back(new MCQquestion("Which feature allows one class to acquire properties of another class?",5,"Encapsulation","Inheritance","Polymorphism","Overloading","B"));

        qlist.push_back(new MCQquestion("Which of the following is used to achieve runtime polymorphism?",5,"Function overloading","Operation overloading","Virtual Functions","Inline Functoins","C"));

        qlist.push_back(new MCQquestion("What is inheritance in OOP?",5,"Hiding data","Binding data and methods","Acquiring properties of another calss","Creating objects","C"));

        qlist.push_back(new MCQquestion("Which access specifier allows members to be accessed only within the same class?",5,"pulbic","private","protected","global","B"));
        
        qlist.push_back(new TFQuestion("Inheritance helps in code reusability.",5,"true"));
        
        qlist.push_back(new TFQuestion("Private members of a class can be accessed directly by derived class.",5,"false"));
        
        qlist.push_back(new TFQuestion("Encapsulation can helps in data hiding.",5,"true"));
        
        qlist.push_back(new TFQuestion("Protected members are accessible outside the class wihtout inheritance.",5,"false"));
       
        qlist.push_back(new TFQuestion("In multiple inheritance, a class can inherit from more than one base calss.",5,"true"));

        unsigned seed = chrono::steady_clock::now().time_since_epoch().count()
                        + hash<string>{}(name);

        shuffle(qlist.begin(), qlist.end(), default_random_engine(seed));

        
        Quiz quiz;
        quiz.setQuestions(qlist);

        quiz.startQuiz(p);

        lb.addPlayer(p);

        
        for (auto q : qlist) {
            delete q;
        }
    }

    lb.sortLeaderboard();
    lb.display();

    return 0;
}
