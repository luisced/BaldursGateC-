#include "game.hpp"
#include <fstream>
#include <algorithm>
#include "../helper/functions.hpp"
#include <ncurses.h>
#include "../helper/error.hpp"
#include "dungeon.hpp"
#include "../data/constants.hpp"
#include "../helper/screen.hpp"
//////////////////////////////////////////////////////////////
//////////////////////  Constructors  ////////////////////////
//////////////////////////////////////////////////////////////

Game::Game() : play_game(true), reload(false)
{
  P = Player();
}

//////////////////////////////////////////////////////////////
//////////////////////  Methods  /////////////////////////////
//////////////////////////////////////////////////////////////

// create game instance
void Game::initialize()
{
  print_log(MAIN_LOG, "Entering initialize()", false);
  srand(time(0));
  // Load data
  std::string filename;
  if (reload)
  {
    filename = SAVE_FILE;
  }
  else
  {
    filename = DEFAULT_FILE;
  }
  print_log(MAIN_LOG, filename);
  load_data(filename);
  create_map();
  reload = false;
  print_log(MAIN_LOG, "Exiting initialize()");
}

// load data from file
void Game::load_data(std::string s)
{
  map.clear();
  print_log(MAIN_LOG, "Entering load_data()");
  std::ifstream input(s);
  if (!input)
  {
    std::string e = "Error: File " + s + " failed to open.";
    print_log(MAIN_LOG, e.c_str());
    return;
  }
  nlohmann::json object;
  // get input
  int i = 0;
  while (input >> object)
  {
    print_log(MAIN_LOG, object["object"]);
    if (object["object"] == "Location")
    {
      Location L = object;
      map.push_back(L);
    }
    else if (object["object"] == "Player")
    {
      P = object;
    }
    else if (object["object"] == "END")
    {
      break;
    }
    print_log(MAIN_LOG, "Done loading object");
  }
  input.close();
  print_log(MAIN_LOG, "Exiting load_data()");
}

void Game::welcome_screen(WINDOW *win)
{
  // Welcome Screen
  int y = 0;
  std::string s;
  std::ifstream input("data/welcome.txt");
  while (std::getline(input, s))
  {
    waddstr(win, s.c_str());
    wmove(win, ++y, 0);
  }
  wmove(win, y, 0);
  input.close();
  std::vector<std::string> v;
  v.push_back("New Game");
  v.push_back("Reload");
  while (1)
  {
    std::string choice = select(win, v, y, false);
    if (choice == "New Game")
      break;
    else
    {
      reload = true;
      break;
    }
  }
  initialize();
  // Player info
  // get input
  s.clear();
  if (!reload)
  {
    wmove(win, ++y, 0);
    waddstr(win, "What is your name?\n");
    while (1)
    {
      s = get_line(win, y);
      if (s != "NULL" && s.length() < NAME_LENGTH)
      {
        break;
      }
      waddstr(win, "Sorry, that is not a valid name.\n");
    }
    P.set_name(s);
    wmove(win, ++y, 0);
    waddstr(win, "Choose your class:\n 1. Warrior\n 2. Archer\n 3. Mage\n 4. Thief\n");
    while (1)
    {
      s = get_line(win, y);
      if (s == "1")
      {
        P.set_class(ClassType::Warrior);
        break;
      }
      else if (s == "2")
      {
        P.set_class(ClassType::Archer);
        break;
      }
      else if (s == "3")
      {
        P.set_class(ClassType::Mage);
        break;
      }
      else if (s == "4")
      {
        P.set_class(ClassType::Thief);
        break;
      }
      waddstr(win, "Sorry, that is not a valid class.\n");
    }

    wmove(win, ++y, 0);

    waddstr(win, "Choose your Race:\n 1. Human\n 2. Elf\n 3. Dwarf\n 4. Orc\n");
    while (1)
    {
      s = get_line(win, y);
      if (s == "1")
      {
        P.set_race(Race::Human);
        break;
      }
      else if (s == "2")
      {
        P.set_race(Race::Elf);
        break;
      }
      else if (s == "3")
      {
        P.set_race(Race::Dwarf);
        break;
      }
      else if (s == "4")
      {
        P.set_race(Race::Orc);
        break;
      }
      waddstr(win, "Sorry, that is not a valid Race.\n");
    }

    wmove(win, ++y, 0);
    waddstr(win, "Choose your Orientation\n 1. Good\n2. Bad\n3.Evil:\n");
    while (1)
    {
      s = get_line(win, y);
      if (s == "1")
      {
        P.set_orientation(Orientation::Good);
        break;
      }
      else if (s == "2")
      {
        P.set_orientation(Orientation::Neutral);
        break;
      }
      else if (s == "3")
      {
        P.set_orientation(Orientation::Evil);
        break;
      }
      waddstr(win, "Sorry, that is not a valid Orientation.\n");
    }

    wmove(win, ++y, 0);
    waddstr(win, "Choose your Gender\n 1. Male\n 2. Female\n 3. Other\n");
    while (1)
    {
      s = get_line(win, y);
      if (s == "1")
      {
        P.set_class(ClassType::Warrior);
        break;
      }
      else if (s == "2")
      {
        P.set_class(ClassType::Archer);
        break;
      }
      else if (s == "3")
      {
        P.set_class(ClassType::Mage);
        break;
      }
      else if (s == "4")
      {
        P.set_class(ClassType::Thief);
        break;
      }
      waddstr(win, "Sorry, that is not a valid class.\n");
    }

    wmove(win, ++y, 0);
    s = "Welcome " + s;
    waddstr(win, s.c_str());
  }
  wmove(win, ++y, 0);
  // start game
  int ch = wgetch(win);
}

void Game::main_screen()
{
  print_log(MAIN_LOG, "Entering main_screen()");
  WINDOW *main = newwin(20, 60, 0, 0);
  WINDOW *info = newwin(8, 19, 0, 60);
  WINDOW *map = newwin(6, 19, 8, 60);
  WINDOW *poi = newwin(8, 19, 14, 60);
  keypad(main, TRUE);
  int x = 0;
  int y = 0;
  wrefresh(main);

  int ch;

  // refresh dungeon
  wclear(poi);
  poi_screen(poi);
  wrefresh(poi);
  // Refresh map
  wclear(main);
  wclear(map);
  map_screen(map);
  wrefresh(map);
  // scrollok(main, TRUE);
  while (P.is_alive() && play_game)
  {
    if (y > 23)
    {
      y = 0;
      wclear(main);
      wrefresh(main);
    }
    // Refresh stats
    P.print_stats(info);
    wrefresh(info);

    // refresh dungeon
    poi_screen(poi);
    wrefresh(poi);

    x = 0;
    wmove(main, ++y, x);
    wrefresh(main);
    waddstr(main, "What would you like to do?\n");
    wrefresh(main);
    std::vector<std::string> v = split(get_line(main, y), ' ');
    if (v[0].size() != 1)
    {
      std::transform(v[0].begin(), v[0].end(), v[0].begin(), ::tolower);
    }
    wmove(main, ++y, x);
    // inventory management
    if (v[0] == "inventory")
    {
      wclear(main);
      P.print_inventory(main, info);
      wclear(main);
      y = 0;
    }
    // change locations
    else if (v[0] == "move")
    {
      bool change = false;
      for (int i = 2; i < v.size(); i++)
      {
        v[1] += " " + v[i];
      }
      P.move(main, info, v[1]);
      // refresh dungeon
      poi_screen(poi);
      wrefresh(poi);
      // refresh map
      wclear(map);
      map_screen(map);
      wrefresh(map);
    }
    // explore
    else if (v[0] == "explore")
    {
      for (int i = 2; i < v.size(); i++)
      {
        v[1] += " " + v[i];
      }
      std::vector<std::string> c = P.current->list_poi();
      for (int i = 0; i < c.size(); i++)
      {
        if (c[i] == v[1])
        {
          Dungeon D = generate_dungeon(c[i]);
          print_log(MAIN_LOG, "Entering dungeon " + D.get_name());
          wclear(main);
          wrefresh(main);
          D.explore(main, info, P);
          print_log(MAIN_LOG, "Deleting dungeon " + D.get_name());
          P.current->poi.erase(P.current->poi.begin() + i);
          nocbreak();
          echo();
          break;
        }
        if (i == c.size() - 1)
        {
          waddstr(main, "Sorry, that is not a valid location\n");
          wrefresh(main);
          break;
        }
      }
    }
    // save
    else if (v[0] == "save")
    {
      save(main);
    }
    // reload Save
    else if (v[0] == "reload")
    {
      wclear(info);
      wrefresh(info);
      wclear(map);
      wrefresh(map);
      wclear(poi);
      wrefresh(poi);
      play_game = false;
      reload = true;
    }
    // quit game
    else if (v[0] == "quit")
    {
      play_game = false;
    }

    // help commands
    else if (v[0] == "whereami")
    {
      std::string s = P.current->get_name() + "\n";
      waddstr(main, s.c_str());
    }
    else if (v[0] == "whoami")
    {
      std::string s = P.get_name() + "\n";
      waddstr(main, P.get_name().c_str());
    }
    else if (v[0] == "help")
    {
      wclear(main);
      y = 0;
      std::ifstream input(HELP_FILE);
      std::string s;
      while (std::getline(input, s))
      {
        s += "\n";
        waddstr(main, s.c_str());
        y++;
      }
      input.close();
    }
    else
    {
      waddstr(main, "Sorry, that command is not recognized");
    }
    wrefresh(main);
    // wclear(main);
  }
  if (!P.is_alive())
  {
    wclear(main);
    y = 0;
    waddstr(main, "You have died! Would you like to reload your last save?");
    wrefresh(main);
    std::vector<std::string> choices;
    choices.push_back("Yes");
    choices.push_back("No");
    std::string choice = select(main, choices, ++y, false);
    if (choice == "Yes")
    {
      reload = true;
    }
    else
    {
      reload = false;
      play_game = false;
    }
  }
  if (reload)
  {
    waddstr(main, "Reloading last save");
    wrefresh(main);
  }

  print_log(MAIN_LOG, "Exiting main_screen()");
}

void Game::poi_screen(WINDOW *win)
{
  wmove(win, 0, 0);
  std::vector<std::string> c = P.current->list_poi();
  std::string b;
  int y = 1;
  waddstr(win, "| POI:\n");
  for (int i = 0; i < c.size(); i++)
  {
    ++y;
    b = "| -" + c[i] + "\n";
    waddstr(win, b.c_str());
  }
  while (y < 7)
  {
    waddstr(win, "|");
    wmove(win, ++y, 0);
  }
  waddstr(win, "|__________________\n");
}

void Game::map_screen(WINDOW *win)
{
  wmove(win, 0, 0);
  std::vector<std::string> c = P.current->list_connections();
  std::string b = "| Current: \n| -" + P.current->get_name() + "\n";
  waddstr(win, b.c_str());
  waddstr(win, "| Connections: \n");
  int y = 2;
  for (int i = 0; i < c.size(); i++)
  {
    y++;
    b = "| -" + c[i] + "\n";
    waddstr(win, b.c_str());
  }
  while (y < 5)
  {
    waddstr(win, "|");
    wmove(win, ++y, 0);
  }
  waddstr(win, "|__________________\n");
}
// add connections between locations in map
void Game::create_map()
{
  print_log(MAIN_LOG, "Entering create_map()");
  std::ifstream input(MAP_FILE);
  nlohmann::json j;

  while (input >> j)
  {
    std::string a = j.at("object").get<std::string>();
    print_log(MAIN_LOG, a.c_str());
    // create vector of pointers to locations and map them
    if (j["object"] == "Mapping")
    {
      std::vector<std::string> v = j.at("connections").get<std::vector<std::string>>();
      std::vector<Location *> c;
      // Loop through all connections listed
      for (int i = 0; i < v.size(); i++)
      {
        // loop through all other maps
        for (int j = 0; j < map.size(); j++)
        {
          // match the connection to the address of the location stored in map
          if (v[i] == map[j].get_name())
          {
            c.push_back(&map[j]);
          }
        }
      }

      // add pointers to location creating its connection
      std::string name = j.at("name").get<std::string>();
      for (int i = 0; i < map.size(); i++)
      {
        if (map[i].get_name() == name)
        {
          map[i].add_connections(c);
        }
      }
    }
    else if (j["object"] == "P_Location")
    {
      for (int i = 0; i < map.size(); i++)
      {
        if (j["current"] == map[i].get_name())
        {
          P.set_location(&map[i]);
        }
      }
    }
    else if (j["object"] == "END")
    {
      return;
    }
    else
    {
      print_log(MAIN_LOG, "Error: Data type not recognized in map.json");
    }
  }
  print_log(MAIN_LOG, "Exiting map_created()");
}

Dungeon Game::generate_dungeon(std::string s)
{
  std::ifstream input(DUNGEON_FILE);
  nlohmann::json j;
  while (input >> j)
  {
    if (j["name"] == s)
    {
      Dungeon D = j;
      return D;
    }
  }
  // add error handling
  return Dungeon();
}
// loop that contains top level game
void Game::play()
{
  main_screen();
}

void Game::start()
{
  initscr();
  keypad(stdscr, TRUE);
  nocbreak();
  echo();
  // include option to reload
  welcome_screen(stdscr);
  while (play_game)
  {
    play();
    if (reload)
    {
      initialize();
      play_game = true;
    }
  }
  endwin();
}

void Game::save(WINDOW *win)
{
  print_log(MAIN_LOG, "Saving Data");
  std::string filename = SAVE_FILE;
  std::string s = "Overwriting " + filename;
  waddstr(win, s.c_str());
  std::ofstream output(filename);
  if (!output)
  {
    s = "Error: File " + filename + " failed to open";
    print_log(MAIN_LOG, s);
    waddstr(win, s.c_str());
    return;
  }

  // save locations
  nlohmann::json j;
  for (int i = 0; i < map.size(); i++)
  {
    j = map[i];
    output << j << std::endl;
  }
  // save player
  j = P;
  output << j << std::endl;
  nlohmann::json k;
  k["object"] = "END";
  output << k << std::endl;
  output.close();

  // Save map
  output.open(MAP_FILE);
  nlohmann::json j2;
  j2["object"] = "P_Location";
  j2["current"] = P.get_location()->get_name();
  output << j2 << "\n";
  nlohmann::json j3;

  for (int i = 0; i < map.size(); i++)
  {
    std::vector<std::string> v;
    v = map[i].list_connections();
    j3["object"] = "Mapping";
    j3["connections"] = v;
    j3["name"] = map[i].get_name();
    output << j3 << std::endl;
  }
  output << k << std::endl;
  output.close();
  print_log(MAIN_LOG, "Data Saved");
}

// Print general game details for debugging
int Game::status(bool print)
{
  WINDOW *info = newwin(40, 20, 60, 0);
  if (!print)
  {
  }
  std::cout << "------------------" << std::endl;
  std::cout << "Play Status: " << play_game << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << "Player: " << P.get_name() << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << "Map size: " << map.size() << std::endl;
  for (int i = 0; i < map.size(); i++)
  {
    std::cout << map[i].get_name();
    map[i].print_connections();
  }
  std::cout << "------------------" << std::endl;
  if (P.get_name() != "NULL")
  {
    std::cout << "Player: " << std::endl;
    P.print_stats(info);
  }
  return 1;
}
