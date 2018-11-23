#ifndef BOT_H
#define BOT_H

#include "commonImport.h"
#include "Vehicule.h"
#include "Goal.h"
#include "Minuteur.h"

class Bot {

   enum Etat {
      MOVE,
      TIR,
      FIND_POSITION,
      ARRET
   };

   Etat currentState = FIND_POSITION;
   float distanceTir = Vehicule::DIMENSION_COTE * 2.f;
   bool tirDemande = false;
   Minuteur minuteur{Vehicule::CADENCE_TIR, true};

   physx::PxVec3 positionSpotTir;
   Goal* goal;
   Vehicule* vehicule;
   physx::PxRigidDynamic* cargo;
   physx::PxScene* gScene;
   
public:
   Bot() = delete;
   ~Bot() = default;

   Bot(Goal* _goal, Vehicule* _vehicule, physx::PxRigidDynamic* _cargo, physx::PxScene* _gScene)
      : goal{ _goal }
      , vehicule{ _vehicule }
      , cargo{ _cargo }
      , gScene{ _gScene }
   {}

public:
   void updateState();
private:
   void findPosition();
   void move();
   void freine();
   bool positionReached();
   bool cargoImmobile();
   physx::PxVec3 getDirectionTir();
   physx::PxVec3 projectionPlanSol(physx::PxVec3& vector);
   void tirBalle();
   
};

#endif //BOT_H
