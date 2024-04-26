#include "item.hpp"
#include <ncurses.h>
#include <vector>
#include <unordered_map>
#pragma once
enum class Gender
{
  Male,
  Female,
  Other
};
enum class Race
{
  Human,
  Elf,
  Dwarf,
  Orc
};
enum class Orientation
{
  Good,
  Neutral,
  Evil
};
enum class ClassType
{
  Warrior,
  Archer,
  Mage,
  Thief
};
class Character
{
protected:
  // Members
  int health;
  int max_health;
  std::unordered_map<std::string, int> skills;
  int experience;
  Item weapon;
  Item armor;
  std::string name;
  bool alive;

  Gender gender;
  Race race;
  Orientation orientation;
  ClassType class_type;

public:
  // Constructors
  Character();
  Character(std::string n, int h, int mh, std::unordered_map<std::string, int> s, int e, Item w, Item a, Gender g, Race r, Orientation o, ClassType c);
  ~Character();

  // Methods
  std::string get_name() { return name; };
  Gender get_gender() { return gender; };
  Race get_race() { return race; };
  Orientation get_orientation() { return orientation; };
  ClassType get_class_type() { return class_type; };
  int attack(WINDOW *win);
  void defend(WINDOW *win, int a);
  int get_speed() { return skills["SPD"]; };
  bool is_alive() { return alive; };
};
