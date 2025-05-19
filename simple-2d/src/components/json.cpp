#include <simple-2d/components/json.h>

simple_2d::JsonComponent::JsonComponent() {
    mJson = nlohmann::json();
}

simple_2d::JsonComponent::~JsonComponent() {
}

void simple_2d::JsonComponent::SetJson(const nlohmann::json& json) {
    mJson = json;
}

nlohmann::json simple_2d::JsonComponent::GetJson() const {
    return mJson;
}

simple_2d::Error simple_2d::JsonComponent::Step() {
    // Do nothing. Simply a component to store json data.
    return Error::OK;
}