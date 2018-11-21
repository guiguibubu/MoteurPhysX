#include "Goal.h"

physx::PxTransform Goal::changePosition() {
// TODO : Changement de position aleatoire
   physx::PxTransform oldPosition = rigidBody->getGlobalPose();
   physx::PxTransform newPosition = physx::PxTransform{ physx::PxVec3{oldPosition.p.x + 100.f, oldPosition.p.y, oldPosition.p.z + 100.f} , oldPosition.q};
   rigidBody->setGlobalPose(newPosition);
   return newPosition;
}