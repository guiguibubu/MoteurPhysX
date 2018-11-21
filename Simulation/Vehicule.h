#ifndef VEHICULE_H
#define VEHICULE_H

#include "commonImport.h"
#include "Balle.h"
#include <functional>

class Vehicule {

   physx::PxReal vitesse;
   std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> rigidBody;
   physx::PxPhysics* gPhysics;
   physx::PxMaterial* gMaterial;
   
public:
   Vehicule() = delete;
   ~Vehicule() = default;

   Vehicule(const physx::PxReal& _vitesse, const physx::PxTransform& t, physx::PxShape* shape, physx::PxPhysics* _gPhysics, physx::PxMaterial*  _gMaterial)
      : vitesse{ _vitesse }
      , gPhysics{ _gPhysics }
      , gMaterial{ _gMaterial }
      , rigidBody { std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(_gPhysics->createRigidDynamic(t), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); })} {
     
      rigidBody->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
      rigidBody->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
      rigidBody->setSleepThreshold(physx::PxReal{ 0 });
  
   }

   std::unique_ptr<Balle> tir(const physx::PxReal& _rayon, const physx::PxReal& _vitesse);
   
   physx::PxRigidDynamic* getRigidBody() {
      return rigidBody.get();
   }
};

#endif //VEHICULE_H
