##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## action_decoder.py
##

ACTIONS = [
    "Forward", "Left", "Right", "Look", "Take food", "Set linemate", "Set deraumere",
    "Set sibur", "Set mendiane", "Set phiras", "Set thystame", "Incantation", "Fork"
]

def decode_action(output_tensor):
    idx = output_tensor.argmax().item()
    return ACTIONS[idx]
