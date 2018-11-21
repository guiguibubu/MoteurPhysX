#ifndef GOAL_H
#define GOAL_H

#include "commonImport.h"
#include <functional>

class Simulation;

class Goal {


   physx::PxReal rayon;
   physx::PxReal hauteur;
   physx::PxVec3 vitesse;
   std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> rigidBody;
   physx::PxPhysics* gPhysics;
   physx::PxMaterial* gMaterial;

public:
   Goal() = delete;
   ~Goal() = default;
   
   Goal(const physx::PxReal& _rayon, const physx::PxReal& _hauteur, const physx::PxTransform& t, physx::PxPhysics* _gPhysics, physx::PxMaterial*  _gMaterial)
      : rayon{ _rayon }
      , hauteur{ _hauteur }
      , gPhysics{ _gPhysics }
      , gMaterial{ _gMaterial }
      , rigidBody{ std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(_gPhysics->createRigidDynamic(physx::PxTransform{ t.p, physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)) }), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); }) } {

      std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>> shape = std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>>(gPhysics->createShape(physx::PxCapsuleGeometry(hauteur/2.f, hauteur / 2.f), *gMaterial, true, physx::PxShapeFlag::eSIMULATION_SHAPE), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); });
      rigidBody->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
      rigidBody->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::Enum::eLOCK_LINEAR_X | physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
      rigidBody->setSleepThreshold(physx::PxReal{ 0 });
   }

   physx::PxTransform changePosition();

   physx::PxRigidDynamic* getRigidBody() {
      return rigidBody.get();
   }
};

#endif //GOAL_H
