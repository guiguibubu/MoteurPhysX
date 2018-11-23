#include "Goal.h"
#include <random>

physx::PxTransform Goal::changePosition() {
// TODO : Changement de position aleatoire

   std::mt19937 prng(std::random_device{}());
   std::uniform_real_distribution<float> realDist{ -50.f,50.f };
   
   auto dx = realDist(prng);
   auto dz = realDist(prng);

   physx::PxTransform oldPosition = rigidBody->getGlobalPose();
   physx::PxTransform newPosition = physx::PxTransform{ physx::PxVec3{oldPosition.p.x + dx, oldPosition.p.y, oldPosition.p.z + dz} , oldPosition.q};
   rigidBody->setGlobalPose(newPosition);
   return newPosition;
}