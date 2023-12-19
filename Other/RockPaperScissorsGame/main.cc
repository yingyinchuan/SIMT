#include <iostream>
#include <ctime>
#include <cstdlib>

enum Choice
{
    ROCK,
    PAPER,
    SCISSORS
};

enum Result
{
    WIN,
    LOSE,
    DRAW
};

Choice getUserChoice()
{
    int choice;
    std::cout << "Enter your choice (0 for Rock, 1 for Paper, 2 for Scissors): ";
    std::cin >> choice;
    return static_cast<Choice>(choice);
}

Choice getComputerChoice()
{
    return static_cast<Choice>(std::rand() % 3);
}

Result determineWinner(Choice user, Choice computer)
{
    if (user == computer)
        return DRAW;
    else if ((user == ROCK && computer == SCISSORS) ||
             (user == PAPER && computer == ROCK) ||
             (user == SCISSORS && computer == PAPER))
        return WIN;
    else
        return LOSE;
}

void printResult(Result result)
{
    if (result == WIN)
        std::cout << "You win!" << std::endl;
    else if (result == LOSE)
        std::cout << "You lose!" << std::endl;
    else
        std::cout << "It's a draw!" << std::endl;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed for random number generation

    while (true)
    {
        Choice userChoice = getUserChoice();
        Choice computerChoice = getComputerChoice();

        std::cout << "You chose: ";
        switch (userChoice)
        {
        case ROCK:
            std::cout << "Rock";
            break;
        case PAPER:
            std::cout << "Paper";
            break;
        case SCISSORS:
            std::cout << "Scissors";
            break;
        }

        std::cout << "\nComputer chose: ";
        switch (computerChoice)
        {
        case ROCK:
            std::cout << "Rock";
            break;
        case PAPER:
            std::cout << "Paper";
            break;
        case SCISSORS:
            std::cout << "Scissors";
            break;
        }

        Result result = determineWinner(userChoice, computerChoice);
        printResult(result);

        char playAgain;
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;

        if (playAgain != 'y' && playAgain != 'Y')
            break;
    }

    return 0;
}
