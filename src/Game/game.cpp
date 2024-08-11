#include "game.hpp"

namespace Game
{

    static std::vector<render_entity_data> data =
    {
        {
            {
                    -0.75f,  0.00f, 0.0f,
                    0.75f,  0.00f, 0.0f,
                    0.00f,  0.75f, 0.0f,
                    0.00f, -0.75f, 0.0f
            },
            {
                    0, 1, 2,
                    0, 1, 3
            }
        }
    };

    game::game()
    {
        _renderer->set_render_entity(data);
    }


}
