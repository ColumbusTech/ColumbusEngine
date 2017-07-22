/************************************************
*              ParticleEffect.cpp               *
*************************************************
*          This file is a part of:              *
*               COLUMBUS ENGINE                 *
*************************************************
*             Nikolay(Columbus) Red             *
*                   20.07.2017                  *
*************************************************/

#include <Graphics/ParticleEffect.h>

namespace C
{
  //////////////////////////////////////////////////////////////////////////////
  //Constructor
  C_ParticleEffect::C_ParticleEffect() {}
  //////////////////////////////////////////////////////////////////////////////
  //Set particle material
  void C_ParticleEffect::setMaterial(const C_Material* aMaterial)
  {
    mMaterial = (C_Material*)aMaterial;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set count of particles
  void C_ParticleEffect::setParticlesCount(const unsigned aParticlesCount)
  {
    mParticleCount = (unsigned)aParticlesCount;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particles visible
  void C_ParticleEffect::setVisible(const bool aVisible)
  {
    mVisible = (bool)aVisible;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set negative direction limit
  void C_ParticleEffect::setMinDirection(const C_Vector3 aMinDirection)
  {
    mMinDirection = (C_Vector3)aMinDirection;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set positive direction limit
  void C_ParticleEffect::setMaxDirection(const C_Vector3 aMaxDirection)
  {
    mMaxDirection = (C_Vector3)aMaxDirection;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set constant force acting on particles
  void C_ParticleEffect::setConstantForce(const C_Vector3 aConstantForce)
  {
    mConstantForce = (C_Vector3)aConstantForce;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle minimum size
  void C_ParticleEffect::setMinParticleSize(const C_Vector2 aMinParticleSize)
  {
    mMinParticleSize = (C_Vector2)aMinParticleSize;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle maximum size
  void C_ParticleEffect::setMaxParticleSize(const C_Vector2 aMaxParticleSize)
  {
    mMaxParticleSize = (C_Vector2)aMaxParticleSize;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set paritcle minimum time to live
  void C_ParticleEffect::setMinTimeToLive(const float aMinTimeToLive)
  {
    mMinTimeToLive = (float)aMinTimeToLive;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle maximum time to live
  void C_ParticleEffect::setMaxTimeToLive(const float aMaxTimeToLive)
  {
    mMaxTimeToLive = (float)aMaxTimeToLive;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle minimum velocity
  void C_ParticleEffect::setMinVelocity(const float aMinVelocity)
  {
    mMinVelocity = (float)aMinVelocity;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle maximum velocity
  void C_ParticleEffect::setMaxVelocity(const float aMaxVelocity)
  {
    mMaxVelocity = (float)aMaxVelocity;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle minimum roation
  void C_ParticleEffect::setMinRotation(const float aMinRotation)
  {
    mMinRotation = (float)aMinRotation;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle maximum rotation
  void C_ParticleEffect::setMaxRotation(const float aMaxRotation)
  {
    mMaxRotation = (float)aMaxRotation;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle minimum rotation speed
  void C_ParticleEffect::setMinRotationSpeed(const float aMinRotationSpeed)
  {
    mMinRotationSpeed = (float)aMinRotationSpeed;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Set particle maximum rotation speed
  void C_ParticleEffect::setMaxRotationSpeed(const float aMaxRotationSpeed)
  {
    mMaxRotationSpeed = (float)aMaxRotationSpeed;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return material
  C_Material* C_ParticleEffect::getMaterial()
  {
    return mMaterial;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particles count
  unsigned C_ParticleEffect::getParticlesCount()
  {
    return mParticleCount;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particles visible
  bool C_ParticleEffect::getVisible()
  {
    return mVisible;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return minimum particle direction
  C_Vector3 C_ParticleEffect::getMinDirection()
  {
    return mMinDirection;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return maximum particle direction
  C_Vector3 C_ParticleEffect::getMaxDirection()
  {
    return mMaxDirection;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return constant force acting on particles
  C_Vector3 C_ParticleEffect::getConstantForce()
  {
    return mConstantForce;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return minimum particle size
  C_Vector2 C_ParticleEffect::getMinParticleSize()
  {
    return mMinParticleSize;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return maximum particle size
  C_Vector2 C_ParticleEffect::getMaxParticleSize()
  {
    return mMaxParticleSize;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle minimum time to live
  float C_ParticleEffect::getMinTimeToLive()
  {
    return mMinTimeToLive;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle maximum time to live
  float C_ParticleEffect::getMaxTimeToLive()
  {
    return mMaxTimeToLive;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle minimum velocity
  float C_ParticleEffect::getMinVelocity()
  {
    return mMinVelocity;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle maximum velocity
  float C_ParticleEffect::getMaxVelocity()
  {
    return mMaxVelocity;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle minimum rotation
  float C_ParticleEffect::getMinRotation()
  {
    return mMinRotation;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle maximum rotation
  float C_ParticleEffect::getMaxRotation()
  {
    return mMaxRotation;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle minimum rotation speed
  float C_ParticleEffect::getMinRotationSpeed()
  {
    return mMinRotationSpeed;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Return particle maximum rotation speed
  float C_ParticleEffect::getMaxRotationSpeed()
  {
    return mMaxRotationSpeed;
  }
  //////////////////////////////////////////////////////////////////////////////
  //Destructor
  C_ParticleEffect::~C_ParticleEffect() {}

}
