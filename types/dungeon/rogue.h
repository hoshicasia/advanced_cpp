#pragma once

#include "dungeon.h"

#include <stdexcept>
#include <deque>
#include <algorithm>

Room* FindFinalRoom(Room* starting_room) {
    std::vector<std::string> cur_keys;
    if (starting_room->IsFinal()) {
        return starting_room;
    }
    for (size_t i = 0; i < starting_room->NumKeys(); ++i) {
        cur_keys.push_back(starting_room->GetKey(i));
    }
    std::deque<Room*> q;
    q.push_back(starting_room);
    size_t idx = 1e4;
    while (!q.empty()) {
        if (idx == 0) {
            return nullptr;
        }
        idx--;
        Room* cur = q.front();
        if (cur->IsFinal()) {
            return cur;
        }
        for (size_t i = 0; i < cur->NumKeys(); ++i) {
            cur_keys.push_back(cur->GetKey(i));
        }
        q.pop_front();
        for (size_t i = 0; i < cur->NumDoors(); ++i) {
            if (cur->GetDoor(i)->IsOpen() &&
                (std::find(q.begin(), q.end(), cur->GetDoor(i)->GoThrough()) == q.end())) {
                q.push_back(cur->GetDoor(i)->GoThrough());
            } else if (!cur->GetDoor(i)->IsOpen()) {
                for (auto k : cur_keys) {
                    if (cur->GetDoor(i)->TryOpen(k) &&
                        std::find(q.begin(), q.end(), cur->GetDoor(i)->GoThrough()) == q.end()) {
                        q.push_back(cur->GetDoor(i)->GoThrough());
                        break;
                    }
                }
            }
        }
    }
    return nullptr;
}