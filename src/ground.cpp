#include "ground.h"
#include <simple-2d/core.h>
#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>

simple_2d::Error Ground::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent("static_repetitive_sprite");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add static_repetitive_sprite component";
        return error;
    }
    error = AddComponent("motion");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add motion component";
        return error;
    }
    auto groundBitmapBundle = engine.GetGraphics().LoadImageFromFile("assets/ground_tile.png");
    auto repetitiveSprite = std::static_pointer_cast<simple_2d::StaticRepetitiveSpriteComponent>(GetComponent("static_repetitive_sprite"));
    repetitiveSprite->SetUnitSurface(groundBitmapBundle.surface);
    repetitiveSprite->SetDimensions(simple_2d::RectangularDimensions<int>(100, 600));
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent("motion"));
    motion->SetPosition(simple_2d::XYCoordinate<float>(100, 400));
    return simple_2d::Error::OK;
}

