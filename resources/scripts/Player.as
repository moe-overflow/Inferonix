
class Player
{
    uint entity;
    float speed = 2.5f;

    void Start(uint handle)
    {
        entity = handle;
    }

    void Update(float dt)
    {
        Transform@ tf = GetTransform(entity);

        if (tf is null)
        {
            return;
        }

        if (IsKeyDown(Key::W))
        {
            tf.Translate(vec3(0.0f, 0.0f, -speed * dt));
        }
        if (IsKeyDown(Key::S))
        {
            tf.Translate(vec3(0.0f, 0.0f, speed * dt));
        }
        if (IsKeyDown(Key::A))
        {
            tf.Translate(vec3(-speed * dt, 0.0f, 0.0f));
        }
        if (IsKeyDown(Key::D))
        {
            tf.Translate(vec3(speed * dt, 0.0f, 0.0f));
        }
    }
}

