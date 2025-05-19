#ifndef SIMPLE_2D_COMPONENTS_JSON_H
#define SIMPLE_2D_COMPONENTS_JSON_H
#include <simple-2d/component.h>
#include <nlohmann/json.hpp>

namespace simple_2d {
    class JsonComponent : public Component {
    public:
        JsonComponent();
        ~JsonComponent();
        void SetJson(const nlohmann::json& json);
        nlohmann::json GetJson() const;
        Error Step() override;
    private:
        nlohmann::json mJson;
    };
}
#endif // SIMPLE_2D_COMPONENTS_JSON_H
