#ifndef BALLE_H
#define BALLE_H

#include "commonImport.h"
#include <functional>

class Balle {

   physx::PxReal rayon;
   physx::PxVec3 vitesse;
   std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> rigidBody;
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
      , rigidBody { std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(PxCreateDynamic(*_gPhysics, t, physx::PxSphereGeometry(rayon), *_gMaterial, 10.0f), [](physx::PxBase* pxBaseToDelete) { pxBaseToDelete->release(); }) } {

      rigidBody->setAngularDamping(0.5f);
      rigidBody->setLinearVelocity(_vitesse);
   }

   physx::PxRigidDynamic* getRigidBody() {
      return rigidBody.get();
   }

};

#endif //BALLE_H
