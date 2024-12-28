import random
import math

def generate_geometric_progression(start, ratio, length):
    progression = []
    current_term = start
    for i in range(length):
        progression.append(current_term)
        current_term *= ratio
    return progression

def play_geometric_progression_game():
    name = input("Welcome to the Geometric Progression Game! May I have your name? ")
    print(f"Hello, {name}!")

    for i in range(3):
        # Генерация длины прогрессии (от 5 до 10) начального значения и знаменателя
        progression_length = random.randint(5, 10)
        start = random.randint(1, 10)
        ratio = random.randint(2, 5)

        progression = generate_geometric_progression(start, ratio, progression_length)

        # Случайный выбор позиции для скрытого элемента
        hidden_index = random.randint(0, progression_length - 1)
        hidden_value = progression[hidden_index]
        progression[hidden_index] = ".."  # Заменяем на две точки

        print("What number is missing in progression?")
        for i, num in enumerate(progression):
            print(f"{num if type(num) == int else '..'}", end=" ")  # Печать "..", если число скрыто

        try:
            user_guess = int(input("\nYour answer: "))
        except ValueError:
            print("Wrong input. Try a number.")
            continue

        if user_guess == hidden_value:
            print("Correct!")
        else:
            print(f"{user_guess} is wrong! Correct answer was {hidden_value}")
            print(f"Let's try again, {name}!")

    print(f"Good game, {name}!")
