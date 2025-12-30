##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## state_encoder.py
##

import torch
from core.config import RESOURCES

def encode_inventory(inventory):
    return [inventory.get(res, 0) for res in RESOURCES]

def encode_vision(vision_tiles):
    flat = []
    for tile in vision_tiles:
        counts = [tile.count(res) for res in RESOURCES]
        flat.extend(counts)
    return flat

def encode_state(state, max_tiles=16):
    inv = encode_inventory(state.inventory)
    lvl = [state.level]
    vis = encode_vision(state.vision_tiles)
    vis += [0] * (len(RESOURCES) * (max_tiles - len(state.vision_tiles)))
    food = [state.food] if hasattr(state, 'food') else [0]
    pos = [state.position.x, state.position.y] if hasattr(state, 'position') else [0, 0]
    return torch.tensor(inv + lvl + vis + food + pos, dtype=torch.float32)
