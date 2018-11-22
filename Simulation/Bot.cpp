#include "Bot.h"

void Bot::updateState() {
   switch (currentState) {
   case Etat::FIND_POSITION: 
      findPosition(); 
      currentState = Etat::MOVE;
      break;
   case Etat::MOVE:
      move();
      if (positionReached()) {
         currentState = Etat::TIR;
      }
      break;
   case Etat::TIR:
      vehicule->tir(Balle::VITESSE_BALLE*getDirectionTir().getNormalized());
      currentState = Etat::FIND_POSITION;
      break;
   default: break;
   }
}