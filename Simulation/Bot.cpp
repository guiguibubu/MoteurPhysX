#include "Bot.h"
#include "Simulation.h"

void Bot::updateState() {
   switch (currentState) {
   case Etat::FIND_POSITION:
      findPosition();
      currentState = Etat::MOVE;
      break;
   case Etat::MOVE:
      move();
      if (positionReached()) {
         currentState = Etat::ARRET;
         tirDemande = true;
      }
      if (!cargoImmobile()) {
         currentState = Etat::FIND_POSITION;
      }
      break;
   case Etat::ARRET:
      freine();
      if (tirDemande) {
         currentState = Etat::TIR;
      }
      break;
   case Etat::TIR:
      if (tirDemande) {
         if (!minuteur.isConfigured()) {
            minuteur.setDecompte(Vehicule::CADENCE_TIR);
            minuteur.start();

            tirBalle();

         }
         else {
            minuteur.refresh();
            if (minuteur.isFinished()) {

               tirBalle();

               minuteur.start();
            }
         }
         // on desamorce la demande de tir
         tirDemande = false;
      }

      if (!tirDemande) {
         currentState = Etat::FIND_POSITION;
      }
      break;
   default: break;
   }
}

void Bot::findPosition() {
   physx::PxVec3 positionCargo = cargo->getGlobalPose().p;
   positionSpotTir = projectionPlanSol(positionCargo - getDirectionTir().getNormalized() * distanceTir);
}
void Bot::move() {
   physx::PxVec3 directionMove = projectionPlanSol(positionSpotTir - vehicule->getRigidBody()->getGlobalPose().p);
   vehicule->getRigidBody()->setLinearVelocity(Vehicule::VITESSE * directionMove.getNormalized());
}

void Bot::freine() {
   vehicule->getRigidBody()->setLinearVelocity({ physx::PxZERO::PxZero });
}

bool Bot::positionReached() {
   auto positionVehicule = vehicule->getRigidBody()->getGlobalPose().p;
   auto vecteurDiff = (positionVehicule - positionSpotTir).abs();
   return vecteurDiff.x < 1.f && vecteurDiff.z < 1.f;
}

bool Bot::cargoImmobile() {
   return cargo->getLinearVelocity().abs().x < 5.f && cargo->getLinearVelocity().abs().z < 5.f;
}

physx::PxVec3 Bot::getDirectionTir() {
   physx::PxVec3 positionGoal = goal->getRigidBody()->getGlobalPose().p;
   physx::PxVec3 positionCargo = cargo->getGlobalPose().p;
   return projectionPlanSol(positionGoal - positionCargo);
}

physx::PxVec3 Bot::projectionPlanSol(physx::PxVec3& vector) {
   return physx::PxVec3{ vector.x, 0, vector.z };
}

void Bot::tirBalle() {

   Simulation::get().tirBalle(*vehicule, getDirectionTir());

}