#ifndef BALLE_H
#define BALLE_H

#include "commonImport.h"
#include <functional>

class Balle {

   physx::PxReal rayon;
   physx::PxVec3 vitesse;
   //std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> rigidBody;
   physx::PxRigidDynamic* rigidBody;
   physx::PxPhysics* gPhysics;
   physx::PxMaterial* gMaterial;

public:
   Balle() = delete;
   ~Balle() = default;

   Balle(const physx::PxVec3& _vitesse, const physx::PxReal& _rayon, const physx::PxTransform& t, physx::PxPhysics* _gPhysics, physx::PxMaterial*  _gMaterial)
      : vitesse{ _vitesse }
      , rayon { _rayon }
      , gPhysics{ _gPhysics }
      , gMaterial{ _gMaterial }
      //, rigidBody { std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(_gPhysics->createRigidDynamic(t), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); }) } 
      , rigidBody{ _gPhysics->createRigidDynamic(t) }
   {
      std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>> shape = std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>>(gPhysics->createShape(physx::PxSphereGeometry(rayon), *gMaterial, false, physx::PxShapeFlag::eSIMULATION_SHAPE), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); });
      rigidBody->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
      rigidBody->setAngularDamping(0.5f);
      rigidBody->setLinearVelocity(vitesse);
      rigidBody->setSleepThreshold(physx::PxReal{ 0 });
   }

   physx::PxRigidDynamic* getRigidBody() {
      return rigidBody;
   }

};

#endif //BALLE_H
