#ifndef BOT_H
#define BOT_H

#include "commonImport.h"
#include "Vehicule.h"
#include "Goal.h"

class Bot {

   enum Etat {
      MOVE,
      TIR,
      FIND_POSITION,
      ARRET
   };

   Etat currentState = FIND_POSITION;
   float distanceTir = 10.f;
   physx::PxVec3 positionGoal;
   Goal* goal;
   Vehicule* vehicule;
   physx::PxRigidDynamic* cargo;
   
public:
   Bot() = delete;
   ~Bot() = default;

   Bot(Goal* _goal, Vehicule* _vehicule, physx::PxRigidDynamic* _cargo)
      : goal{ _goal }
      , vehicule{ _vehicule }
      , cargo{ _cargo }
   {}

private:
   void updateState();
   void findPosition();
   void move();
   bool positionReached();
   physx::PxVec3 getDirectionTir();
   
};

#endif //BOT_H
