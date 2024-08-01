#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> // Beep�p
#include <algorithm> 

// �����ẴN���X
class NumberGuessingGame {
public:
    // �R���X�g���N�^
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
// �R���X�g���N�^
NumberGuessingGame::NumberGuessingGame(int rangeStart, int rangeEnd, int maxAttempts)
    : rangeStart(rangeStart), rangeEnd(rangeEnd), maxAttempts(maxAttempts) {
    secretNumber = generateSecretNumber();
}

// �ԍ�����
int NumberGuessingGame::generateSecretNumber() {
    std::srand(std::time(0));
    return std::rand() % (rangeEnd - rangeStart + 1) + rangeStart;
}

// �\�z�����������ǂ���
bool NumberGuessingGame::isCorrectGuess(int guess) {
    return guess == secretNumber;
}

// �ȑO�̉񓚂�\��
void NumberGuessingGame::displayPreviousGuesses() {
    std::cout << "����܂ł̂悻��: ";
    for (auto it = previousGuesses.begin(); it != previousGuesses.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// Beep��
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

// �Q�[��
void NumberGuessingGame::play() {
    int attempts = 0;
    int guess;

    while (attempts < maxAttempts) {
        std::cout << "�悻���������������ɂイ��傭���Ă� (" << rangeStart << " - " << rangeEnd << "): ";
        std::cin >> guess;

        if (std::find(previousGuesses.begin(), previousGuesses.end(), guess) != previousGuesses.end()) {
            std::cout << "���̂������͂��������ɂイ��傭������D�ׂ̂����������߂��Ă݂�." << std::endl;
            continue;
        }

        previousGuesses.push_back(guess);
        attempts++;

        if (isCorrectGuess(guess)) {
            std::cout << "���߂łƂ�! ���������̂�������: " << secretNumber << std::endl;
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
                std::cout << "�������I������Ƃ�������" << std::endl;
                playSound(false, true, true, false);
            }
            else if (slightlyLow) {
                std::cout << "�������I������Ƃ�������" << std::endl;
                playSound(false, true, false, true);
            }
            else if (guess < secretNumber) {
                std::cout << "�����������I" << std::endl;
                playSound(false, false, false, true);
            }
            else {
                std::cout << "�����������I" << std::endl;
                playSound(false, false, true, false);
            }
            displayPreviousGuesses();
        }
    }

    std::cout << "���߂�ˁD���ꂢ���傤�͂ɂイ��傭�ł��Ȃ�. ����������: " << secretNumber << std::endl;
}

// �����L���O�\��
void NumberGuessingGame::displayRankings() const {
    std::cout << "\n�����L���O (���s��):" << std::endl;
    for (size_t i = 0; i < rankings.size(); ++i) {
        std::cout << (i + 1) << ". " << rankings[i] << " ��" << std::endl;
    }
}

int main() {
    NumberGuessingGame game(1, 100, 20);

    char playAgain = 'y';
    do {
        game.play();
        std::cout << "���������������H�����ԂȂ炙�L�[�C�����Ȃ炎�L�[�������� (y/n): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    game.displayRankings(); 

    return 0;
}