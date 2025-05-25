#ifndef SIMPLE_2D_COMPONENTS_JSON_H
#define SIMPLE_2D_COMPONENTS_JSON_H
#include <simple-2d/component.h>
#include <nlohmann/json.hpp>

namespace simple_2d {
    class JsonComponent : public Component {
    public:
        JsonComponent(EntityId entityId);
        ~JsonComponent();
        void SetJson(const nlohmann::json& json);
        nlohmann::json GetJson() const;
        Error Step() override;
    private:
        nlohmann::json mJson;
    };

    class JsonComponentManager : public ComponentManager {
    public:
        JsonComponentManager() = default;
        ~JsonComponentManager() = default;
        void Step() override;
    };
}
#endif // SIMPLE_2D_COMPONENTS_JSON_H
