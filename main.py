from lcm_game import play_lcm_game
from gp_game import play_geometric_progression_game

if __name__ == "__main__":

    game_choice = input("Do you want to play 1.LCM or 2.GP? (1/2): ").lower()
    if game_choice == "1":
        play_lcm_game()
    else:
        play_geometric_progression_game()

    input("press any key to exit")