#include "Player.hh"
#include <list>
#include <chrono>
#include <thread>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME JoelJordan


struct PLAYER_NAME : public Player {

  /**
   * Fxactory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  bool sube_baja(Pos centro,vector<int>& alive) {
    if(cell(centro+DR).id != -1 and unit(cell(centro+DR).id).type == Alive and not same_team(cell(centro+DR),alive)) return true;
    if(cell(centro+RU).id != -1 and unit(cell(centro+RU).id).type == Alive and not same_team(cell(centro+RU),alive)) return true;
    if(cell(centro+UL).id != -1 and unit(cell(centro+UL).id).type == Alive and not same_team(cell(centro+UL),alive)) return true;
    if(cell(centro+LD).id != -1 and unit(cell(centro+LD).id).type == Alive and not same_team(cell(centro+LD),alive)) return true;
    return false;
  }

  int prob_ganar(Unit enemigo) {
    return strength(me())/(strength(me())+strength(enemigo.player));
  }

  //Funcion que nos dice si hay un enemigo cerca
  bool enemigo_cerca(Pos centro) {
    if(cell(centro+Up).id != -1 and unit(cell(centro+Up).id).type == Alive) return true;
    if(cell(centro+Down).id != -1 and unit(cell(centro+Down).id).type == Alive) return true;
    if(cell(centro+Left).id != -1 and unit(cell(centro+Left).id).type == Alive) return true;
    if(cell(centro+Right).id != -1 and unit(cell(centro+Right).id).type == Alive) return true;
    if(cell(centro+DR).id != -1 and unit(cell(centro+DR).id).type == Alive) return true;
    if(cell(centro+RU).id != -1 and unit(cell(centro+RU).id).type == Alive) return true;
    if(cell(centro+UL).id != -1 and unit(cell(centro+UL).id).type == Alive) return true;
    if(cell(centro+LD).id != -1 and unit(cell(centro+LD).id).type == Alive) return true;
    return false;
  }

  //Devuelve true si en la casilla hay un miembro del mismo equipo y false en caso contrario
  bool same_team(Cell actual,vector<int>& alive) {
    int id_actual = actual.id;
    for (int id : alive) {
      if(id_actual == id) return true;
    }
    return false;
  }

  bool is_zombie_cerca(Pos pos) {
    if(unit(cell(pos+Up+Up).id).type == Zombie) return true;
    if(unit(cell(pos+Down+Down).id).type == Zombie) return true;
    if(unit(cell(pos+Left+Left).id).type == Zombie) return true;
    if(unit(cell(pos+Right+Right).id).type == Zombie) return true;
    return false;
  }

  //TODO: cambiar actual y primer por pos
  //Realiza una busqueda BFS y en funcion de eso escoge la direccion en la que se ha de mover la unidad
  Dir BFS(int id, vector<int>& alive){
    Pos actual = unit(id).pos;
    Pos primer = unit(id).pos;
    Pos comida;
    map<Pos,Pos> orden;
    set<Pos> visto;
    //list<Pos> L;
    queue<Pos> Q;
    bool found_comida = false;
    bool found_enemigo = false;
    bool found_zombie = false;

    while(not found_comida and not found_enemigo and not found_zombie) {
      if(visto.find(actual) == visto.end()) {
        Q.push(actual);
        visto.insert(actual);
        //cerr << "q.empty()" << endl;
        while(not Q.empty() and not found_comida and not found_enemigo and not found_zombie) {
          Pos x = Q.front(); Q.pop();
          //L.push_back(x);
          for(int i = 0; i < 4; ++i) {
            //cerr << "for: " << i << endl;
            Pos w = x;
            switch(i) {
              case 0:
                w += Up;
                break;
              case 1:
                w += Down;
                break;
              case 2:
                w += Left;
                break;
              case 3:
                w += Right;
                break;
            }
            Cell cell_actual = cell(w);
            //cerr << "elemento w es:" << w << "elemento x es: " << x << endl;
            if (pos_ok(w) and cell_actual.type != Waste and visto.find(w) == visto.end()) {
              Q.push(w);
              visto.insert(w);
              orden.insert(pair<Pos,Pos> (w,x));
              //Si hay un zombie ves a por el
              if (cell_actual.id != -1 and unit(cell_actual.id).type == Zombie) {
                found_zombie = true;
                comida = w;
              }
              //Si hay comida ves para ya
              if(cell_actual.food) {
                found_comida = true;
                comida = w;
                //L.push_back(w);
              }
                //Si en la celda no hay una unidad del mismo equipo ni una muerta ves para alla
              if (cell_actual.id != -1 and unit(cell_actual.id).type != Dead and not same_team(cell_actual,alive)){
                found_enemigo = true;
                comida = w;
              }
            //cerr << "salimos del for" << endl;
            }
          }
        }
      }
    }
    Pos aux;
    auto it = orden.find(comida);
    while (it->first != primer) {
      if (it -> second == primer) {aux = it->first;break;}
      it = orden.find(it->second);
      //this_thread::sleep_for(chrono::seconds(1));
    }
    if (aux ==  (primer + Down)) {
      if(cell(primer+Down).id != -1 and same_team(cell(primer+Down),alive)) return Up;
      else return Down;
    }
    else if(aux == (primer + Left)){
      if(cell(primer+Left).id != -1 and same_team(cell(primer+Left),alive)) return Right; 
      else return Left;
    }
    else if(aux == (primer + Up)) {
      if(cell(primer+Up).id != -1 and same_team(cell(primer+Up),alive)) return Down;
      else return Up;
    }
    else if(aux == (primer + Right)) {
      if(cell(primer+Right).id != -1 and same_team(cell(primer+Right),alive)) return Left;
      else return Right;
    }
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector<int> alive = alive_units(me());

    for (auto id : alive) {
      if (not sube_baja(unit(id).pos,alive)) {
        Dir mover = BFS(id,alive);
        move(id,mover);
      }
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
