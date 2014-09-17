#include "CCLight3D.h"
#include "2d/CCScene.h"

NS_CC_BEGIN

void BaseLight3D::setIntensity(float intensity)
{
    CC_ASSERT(intensity >= 0);
    _intensity = intensity;
}

void BaseLight3D::onEnter()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter == lights.end())
            lights.push_back(this);
    }
    Node::onEnter();
}
void BaseLight3D::onExit()
{
    auto scene = getScene();
    if (scene)
    {
        auto &lights = scene->_lights;
        auto iter = std::find(lights.begin(), lights.end(), this);
        if (iter != lights.end())
            lights.erase(iter);
    }
    Node::onExit();
}

void BaseLight3D::setRotationFromDirection( const Vec3 &direction )
{
    float projLen = sqrt(direction.x * direction.x + direction.z * direction.z);
    float rotY = CC_RADIANS_TO_DEGREES(atan2f(-direction.x, -direction.z));
    float rotX = -CC_RADIANS_TO_DEGREES(atan2f(-direction.y, projLen));
    setRotation3D(Vec3(rotX, rotY, 0.0f));
}

BaseLight3D::BaseLight3D()
: _intensity(1.0f)
, _enabled(true)
, _lightFlag(LightFlag::LIGHT0)
{
    
}
BaseLight3D::~BaseLight3D()
{
    
}


////////////////////////////////////////////////////////////////////
DirectionLight3D* DirectionLight3D::create(const Vec3 &direction, const Color3B &color)
{
    auto light = new (std::nothrow) DirectionLight3D();
    light->setRotationFromDirection(direction);
    light->setColor(color);
    light->autorelease();
    return light;
}

void DirectionLight3D::setDirection(const Vec3 &dir)
{
    setRotationFromDirection(dir);
}
const Vec3& DirectionLight3D::getDirection() const
{
    static Vec3 dir;
    Mat4 mat = getNodeToParentTransform();
    dir.set(-mat.m[8], -mat.m[9], -mat.m[10]);
    return dir;
}
const Vec3& DirectionLight3D::getDirectionInWorld() const
{
    static Vec3 dir;
    Mat4 mat = getNodeToWorldTransform();
    dir.set(-mat.m[8], -mat.m[9], -mat.m[10]);
    return dir;
}
DirectionLight3D::DirectionLight3D()
{
    
}
DirectionLight3D::~DirectionLight3D()
{
    
}

//////////////////////////////////////////////////////////////////
PointLight3D* PointLight3D::create(const Vec3 &position, const Color3B &color, float range)
{
    auto light = new (std::nothrow) PointLight3D();
    light->setPosition3D(position);
    light->setColor(color);
    light->_range = range;
    light->autorelease();
    return light;
}

PointLight3D::PointLight3D()
{
    
}
PointLight3D::~PointLight3D()
{
    
}

//////////////////////////////////////////////////////////////
SpotLight3D* SpotLight3D::create(const Vec3 &direction, const Vec3 &position, const Color3B &color, float innerAngle, float outerAngle, float range)
{
    auto light = new (std::nothrow) SpotLight3D();
    light->setRotationFromDirection(direction);
    light->setPosition3D(position);
    light->setColor(color);
    light->setInnerAngle(innerAngle);
    light->setOuterAngle(outerAngle);
    light->_range = range;
    light->autorelease();
    return light;
}

void SpotLight3D::setDirection(const Vec3 &dir)
{
    setRotationFromDirection(dir);
}

const Vec3& SpotLight3D::getDirection() const
{
    static Vec3 dir;
    Mat4 mat = getNodeToParentTransform();
    dir.set(-mat.m[8], -mat.m[9], -mat.m[10]);
    return dir;
}

const Vec3& SpotLight3D::getDirectionInWorld() const
{
    static Vec3 dir;
    Mat4 mat = getNodeToWorldTransform();
    dir.set(-mat.m[8], -mat.m[9], -mat.m[10]);
    return dir;
}

void SpotLight3D::setInnerAngle(float angle)
{
    _innerAngle = angle;
    _cosInnerAngle = cosf(angle);
}

void SpotLight3D::setOuterAngle(float angle)
{
    _outerAngle = angle;
    _cosInnerAngle = cosf(angle);
}

/////////////////////////////////////////////////////////////

Light3D::Light3D()
    : _isEnabled(true)
    , _range(0.0)
    , _innerAngle(0.0)
    , _outerAngle(0.0)
{
}

Light3D::~Light3D()
{
}

Light3D* Light3D::createDirectionalLight( const Vec3 &direction, const Color3B &color )
{
    Light3D *light = new Light3D;
    light->_lightType = Light3D::LightType::DIRECTIONAL;
    light->calculateRotation(direction);
    light->setColor(color);
    light->autorelease();
    return light;
}

Light3D* Light3D::createPointLight( const Vec3 &position, const Color3B &color, float range )
{
    Light3D *light = new Light3D;
    light->_lightType = Light3D::LightType::POINT;
    light->setPosition3D(position);
    light->setColor(color);
    light->_range = range;
    light->autorelease();
    return light;
}

Light3D* Light3D::createSpotLight( const Vec3 &direction, const Vec3 &position, const Color3B &color, float innerAngle, float outerAngle, float range )
{
    Light3D *light = new Light3D;
    light->_lightType = Light3D::LightType::SPOT;
    light->calculateRotation(direction);
    light->setPosition3D(position);
    light->setColor(color);
    light->_innerAngle = innerAngle;
    light->_outerAngle = outerAngle;
    light->_range = range;
    light->autorelease();
    return light;
}


void Light3D::setLightType( LightType lightType )
{
    _lightType = lightType;
}


Light3D::LightType Light3D::getLightType()
{
    return _lightType;
}

void Light3D::setRange( float range )
{
    _range = range;
}

float Light3D::getRange()
{
    return _range;
}

void Light3D::setInnerAngle( float angle )
{
    _innerAngle = angle;
}

float Light3D::getInnerAngle()
{
    return _innerAngle;
}

void Light3D::setOuterAngle( float angle )
{
    _outerAngle = angle;
}

float Light3D::getOuterAngle()
{
    return _outerAngle;
}

void Light3D::onEnter()
{
//    auto scene = getScene();
//    if (scene)
//    {
//        auto &lights = scene->_lights;
//        auto iter = std::find(lights.begin(), lights.end(), this);
//        if (iter == lights.end())
//            lights.push_back(this);
//    }
//    Node::onEnter();
}

void Light3D::onExit()
{
//    auto scene = getScene();
//    if (scene)
//    {
//        auto &lights = scene->_lights;
//        auto iter = std::find(lights.begin(), lights.end(), this);
//        if (iter != lights.end())
//            lights.erase(iter);
//    }
//    Node::onExit();
}

void Light3D::setEnabled( bool enabled )
{
    _isEnabled = enabled;
}

bool Light3D::getEnabled()
{
    return _isEnabled;
}

void Light3D::calculateRotation( const Vec3 &direction )
{
    float projLen = sqrt(direction.x * direction.x + direction.z * direction.z);
    float rotY = CC_RADIANS_TO_DEGREES(atan2f(-direction.x, -direction.z));
    float rotX = -CC_RADIANS_TO_DEGREES(atan2f(-direction.y, projLen));
    setRotation3D(Vec3(rotX, rotY, 0.0f));
}

void Light3D::setDirection( const Vec3 &dir )
{
    calculateRotation(dir);
}

Vec3 Light3D::getDirection() const
{
    Mat4 mat = getNodeToParentTransform();
//    mat.m[12] = mat.m[13] = mat.m[14] = 0.0f;
//    mat.inverse();
//    mat.transpose();
//
//    Vec3 dir;
//    mat.transformVector(0.0f, 0.0f, -1.0f, 0.0f, &dir);
//
//    return dir;
    
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

Vec3 Light3D::getWorldDirection() const
{
    Mat4 mat = getNodeToWorldTransform();
//    mat.m[12] = mat.m[13] = mat.m[14] = 0.0f;
//    mat.inverse();
//    mat.transpose();
//
//    Vec3 dir;
//    mat.transformVector(0.0f, 0.0f, -1.0f, 0.0f, &dir);
//
//    return dir;
    return Vec3(-mat.m[8], -mat.m[9], -mat.m[10]);
}

NS_CC_END