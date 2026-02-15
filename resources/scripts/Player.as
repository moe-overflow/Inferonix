
class Player
{
    uint entity;
    float speed = 5.0f;

    void Start(uint entityHandle)
    {
        entity = entityHandle;
    }

    void Update(float dt)
    {
        Transform@ tf = GetTransform(entity);
        if (tf is null) return;

        vec3 velocity(0.0f);

        if (IsKeyDown(Key::W)) velocity.z -= 1.0f;
        if (IsKeyDown(Key::S)) velocity.z += 1.0f;
        if (IsKeyDown(Key::A)) velocity.x -= 1.0f;
        if (IsKeyDown(Key::D)) velocity.x += 1.0f;

        tf.Translate(velocity * speed * dt);
    }
}
