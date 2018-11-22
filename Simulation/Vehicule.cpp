#include "Vehicule.h"

const unsigned short Vehicule::CADENCE_TIR = 1000 / 3; //3 balle par seconde
const physx::PxReal VITESSE = 10.f;

std::unique_ptr<Balle> Vehicule::tir(const physx::PxVec3& _vitesse) {
   return std::make_unique<Balle>(rigidBody->getGlobalPose().rotate(_vitesse), Balle::RAYON_BALLE, physx::PxTransform{ rigidBody->getGlobalPose().p }, gPhysics, gMaterial);
}