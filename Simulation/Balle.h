#ifndef BALLE_H
#define BALLE_H

#include "commonImport.h"
#include <functional>

class Balle {
public:
   static const unsigned short VITESSE_BALLE;
   static const physx::PxReal RAYON_BALLE;
   static const unsigned short NB_BALLE_VIE_MAX;
private:
   physx::PxReal rayon;
   physx::PxVec3 vitesse;
   std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> rigidBody;
   //physx::PxRigidDynamic* rigidBody;
   physx::PxPhysics* gPhysics;
   physx::PxMaterial* gMaterial;

public:
   Balle() = delete;
   ~Balle() = default;

   Balle(const physx::PxVec3& _vitesse, const physx::PxReal& _rayon, const physx::PxTransform& t, physx::PxPhysics* _gPhysics, physx::PxMaterial*  _gMaterial)
      : vitesse{ _vitesse }
      , rayon { _rayon }
      , gPhysics{ _gPhysics }
      , gMaterial{ _gPhysics->createMaterial(0.f, 0.f, 0.f) }
      , rigidBody { std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(_gPhysics->createRigidDynamic(t), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); }) } 
      //, rigidBody{ _gPhysics->createRigidDynamic(t) }
   {
      std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>> shape = std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>>(gPhysics->createShape(physx::PxSphereGeometry(rayon), *gMaterial, true, physx::PxShapeFlag::eSIMULATION_SHAPE), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); });
      rigidBody->attachShape(*shape);
      physx::PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
      rigidBody->setAngularDamping(0.5f);
      rigidBody->setLinearVelocity(vitesse);
      rigidBody->setLinearDamping(0.f);
      rigidBody->setSleepThreshold(physx::PxReal{ 0 });
   }

   physx::PxRigidDynamic* getRigidBody() {
      return rigidBody.get();
   }

};

#endif //BALLE_H
