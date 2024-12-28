import random
import math

def calculate_lcm(numbers):
  lcm = 1
  for num in numbers:
      lcm = lcm * num // math.gcd(lcm, num)
  return lcm


def play_lcm_game():
    name = input("Welcome to the Brain Games! May I have your name? ")
    print(f"Hello, {name}!")

    for i in range(3):
      numbers = [random.randint(2, 20) for i in range(3)]
      print(f"Find the smallest common multiple of numbers: {numbers[0]}, {numbers[1]}, {numbers[2]}")

      correct_lcm = calculate_lcm(numbers)
      try:
        user_lcm = int(input("Your answer: "))
      except ValueError:
        print("Wrong input. Try a number.")
        continue

      if user_lcm == correct_lcm:
          print("Correct!")
      else:
          print(f"{user_lcm} is wrong! Correct answer was {correct_lcm}")
          print(f"Let's try again, {name}!")
      
    print(f"Good game, {name}!")
