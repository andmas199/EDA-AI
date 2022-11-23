#include "Player.hh"
#include <list>


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME JoelJordan


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  //TODO: cambiar actual y primer por pos
  //Realiza una busqueda BFS y en funcion de eso escoge la direccion en la que se ha de mover la unidad
  Dir BFS(int id, vector<int>& alive){
    Pos actual = unit(id).pos;
    Pos primer = unit(id).pos;
    list<Pos> L;
    map<Pos,Pos> orden;
    queue<Pos> Q;
    set<Pos> visto;
    bool found = false;
    cerr << "Comenzando bucle" << endl;

    int u = 0;
    while (not found) {
      /*
      switch(u){
        case 0:
          actual = unit(id).pos;
          actual += Up;
          break;
        case 1:
          actual = unit(id).pos;
          actual += Down;
          break;
        case 2:
          actual = unit(id).pos;
          actual += Left;
          break;
        case 3:
          actual = unit(id).pos;
          actual += Right;
          break;
        default:
          actual = unit(id).pos;
          break;
      }
      */
      //auto u = visto.find(actual);
        //cerr << "primer if" << endl;
        if (visto.find(actual) == visto.end()) { //Si no esta, ejecutamos cosas
          Q.push(actual);
          //if (visto.size() != 0) orden.insert(pair<Pos,Pos> (actual,*(visto.end()--)));
          //else orden.insert(pair<Pos,Pos> (actual,primer));
          /*
          for (auto it : visto) {
            cerr << "ultima posicion de visto" << it << endl;
          }
          */
          //cerr << visto.size() << endl;
          visto.insert(actual);

          while (not Q.empty()) {
            //cerr << "Entrando cola" << endl;
            Pos x = Q.front(); Q.pop();
            if (Q.size() > 1) orden.insert(pair<Pos,Pos> (Q.front(),x));
            L.push_back(x);
            //cerr << "Comprobando si es un waste y actual es: " << x << endl;

            
            //Si la celda no es un desecho, comprobamos las celdas colindantes a la actual y las añadimos a la pila
            if (cell(x).type != Waste and pos_ok(x)) {
              for (int i = 0; i < 3; ++i) {
                Pos w = x;
                switch(i) {
                  case 0:
                    w += Up;
                    //cerr << "comprobando arriba" << endl;
                    break;
                  case 1:
                    w += Down;
                    //cerr << "comprobando abajo" << endl;
                    break;
                  case 2:
                    w += Left;
                   // cerr << "comprobando izquierda" << endl;
                    break;
                  case 3:
                    w += Right;
                   // cerr << "comprobando derecha" << endl;
                    break;
                }
                //cerr << "w es " << w << endl;
                if(pos_ok(w)) {
                  //Comprobamos que no se haya comprobado ya la casilla
                  if (visto.find(w) == visto.end()) { //Si no esta, añadir elemento a cola y a visto
                    Q.push(w); visto.insert(w);
                  }

                  //Si hay comida ves para alla
                  if (cell(w).food) found = true;

                  
                  bool same_team = false;
                  unsigned int j = 0;
                  //Indica si en la casilla hay un miembro del mismo equipo
                  while (not same_team and j < alive.size()) {
                    int id_actual = cell(w).id;
                    if(id_actual == alive[i]) same_team = true;
                    ++j;
                  }

                  //Si en la celda no hay una unidad del mismo equipo ves para alla
                  if (cell(w).id != -1 and not same_team) found = true;
                }
              }
            }
          }
        }
        ++u;
    }
      Pos aux;
      /*cerr << orden.size() << endl;
      auto it = orden.find(*(--L.end())); 
      cerr << (it != orden.end()) << endl;
      */
      auto it = orden.begin();
      while (it != orden.end()) {
        cerr << "posicion actual es " <<  it->first << "y la anterior " << it->second << endl;
        cerr << primer << endl;
        ++it;
      }
      auto it2 = L.begin();
      while (it2 != L.end()) {
        cerr << "Elemento de la lista: " <<  *it2 << endl;
        ++it2;
      }
      //Pos aux = (orden.begin()++)->first;
      //Comprobamos en que posicion se tiene que mover la unidad
      cerr << "Aux es " << aux << endl;
      if (aux ==  (primer + Down)) return Down;
      else if(aux == (primer + Left)) return Left;
      else if(aux == (primer + Up)) return Up;
      else return Right;
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
      move(id,BFS(id,alive));
    }
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
