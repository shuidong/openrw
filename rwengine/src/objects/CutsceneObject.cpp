#include <data/Skeleton.hpp>
#include <engine/Animator.hpp>
#include <objects/CutsceneObject.hpp>
#include <engine/GameWorld.hpp>
#include <engine/GameData.hpp>

CutsceneObject::CutsceneObject(GameWorld *engine, ObjectID modelid, const glm::vec3 &pos,
                               const glm::quat &rot)
    : GameObject(engine, modelid, pos, rot), _parent(nullptr), _bone(nullptr)
{
  skeleton = new Skeleton;
  animator = new Animator(engine->data->getOrLoadObjectDFF(modelid), skeleton);
}

CutsceneObject::~CutsceneObject()
{
}

void CutsceneObject::tick(float dt)
{
  animator->tick(dt);
}

void CutsceneObject::setParentActor(GameObject *parent, ModelFrame *bone)
{
  _parent = parent;
  _bone = bone;
}
