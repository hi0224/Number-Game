#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> // Beep用
#include <algorithm> 

// 数当てのクラス
class NumberGuessingGame {
public:
    // コンストラクタ
    NumberGuessingGame(int rangeStart, int rangeEnd, int maxAttempts);
    void play();
    void displayRankings() const;

private:
    int secretNumber;
    int rangeStart;
    int rangeEnd;
    int maxAttempts;
    std::vector<int> previousGuesses;
    std::vector<int> rankings;

    int generateSecretNumber();
    bool isCorrectGuess(int guess);
    void displayPreviousGuesses();
    void playSound(bool correctGuess, bool closeGuess, bool slightlyHigh, bool slightlyLow);
};
// コンストラクタ
NumberGuessingGame::NumberGuessingGame(int rangeStart, int rangeEnd, int maxAttempts)
    : rangeStart(rangeStart), rangeEnd(rangeEnd), maxAttempts(maxAttempts) {
    secretNumber = generateSecretNumber();
}

// 番号生成
int NumberGuessingGame::generateSecretNumber() {
    std::srand(std::time(0));
    return std::rand() % (rangeEnd - rangeStart + 1) + rangeStart;
}

// 予想が正しいかどうか
bool NumberGuessingGame::isCorrectGuess(int guess) {
    return guess == secretNumber;
}

// 以前の回答を表示
void NumberGuessingGame::displayPreviousGuesses() {
    std::cout << "これまでのよそう: ";
    for (auto it = previousGuesses.begin(); it != previousGuesses.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// Beep音
void NumberGuessingGame::playSound(bool correctGuess, bool closeGuess, bool slightlyHigh, bool slightlyLow) {
    if (correctGuess) {
        
        Beep(784, 300); // G5
        Beep(1047, 500); // C6
    }
    else if (closeGuess) {
        if (slightlyHigh) {
            Beep(660, 300); // E5
        }
        else if (slightlyLow) {
            Beep(440, 300); // A4
        }
    }
    else {
        if (slightlyHigh) {
            Beep(523, 300); // C5
        }
        else if (slightlyLow) {
            Beep(349, 300); // F4
        }
    }
}

// ゲーム
void NumberGuessingGame::play() {
    int attempts = 0;
    int guess;

    while (attempts < maxAttempts) {
        std::cout << "よそうしたすうちをにゅうりょくしてね (" << rangeStart << " - " << rangeEnd << "): ";
        std::cin >> guess;

        if (std::find(previousGuesses.begin(), previousGuesses.end(), guess) != previousGuesses.end()) {
            std::cout << "このすうちはさっきもにゅうりょくしたよ．べつのすうちをためしてみて." << std::endl;
            continue;
        }

        previousGuesses.push_back(guess);
        attempts++;

        if (isCorrectGuess(guess)) {
            std::cout << "おめでとう! せいかいのすうちは: " << secretNumber << std::endl;
            playSound(true, false, false, false);
            rankings.push_back(attempts); 
            std::sort(rankings.begin(), rankings.end()); 
            return;
        }
        else {
            int difference = std::abs(guess - secretNumber);
            bool slightlyHigh = (guess > secretNumber && difference <= 10);
            bool slightlyLow = (guess < secretNumber && difference <= 10);

            if (slightlyHigh) {
                std::cout << "おしい！ちょっとおおきい" << std::endl;
                playSound(false, true, true, false);
            }
            else if (slightlyLow) {
                std::cout << "おしい！ちょっとちいさい" << std::endl;
                playSound(false, true, false, true);
            }
            else if (guess < secretNumber) {
                std::cout << "ちいさすぎ！" << std::endl;
                playSound(false, false, false, true);
            }
            else {
                std::cout << "おおきすぎ！" << std::endl;
                playSound(false, false, true, false);
            }
            displayPreviousGuesses();
        }
    }

    std::cout << "ごめんね．これいじょうはにゅうりょくできない. せいかいは: " << secretNumber << std::endl;
}

// ランキング表示
void NumberGuessingGame::displayRankings() const {
    std::cout << "\nランキング (試行回数):" << std::endl;
    for (size_t i = 0; i < rankings.size(); ++i) {
        std::cout << (i + 1) << ". " << rankings[i] << " 回" << std::endl;
    }
}

int main() {
    NumberGuessingGame game(1, 100, 20);

    char playAgain = 'y';
    do {
        game.play();
        std::cout << "もういっかいやる？あそぶならｙキー，おわるならｎキーをおして (y/n): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    game.displayRankings(); 

    return 0;
}