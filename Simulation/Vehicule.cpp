#include "Vehicule.h"

std::unique_ptr<Balle> Vehicule::tir(const physx::PxReal& _rayon, const physx::PxReal& _vitesse) {
   return std::make_unique<Balle>(rigidBody->getGlobalPose().rotate(physx::PxVec3(_vitesse, 0.f, 0.f)), _rayon, rigidBody->getGlobalPose(), gPhysics, gMaterial);
}