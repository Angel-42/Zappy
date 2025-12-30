/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AnimatedSprite.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <mutex>
#include <atomic>
#include <functional>

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
    using Callback = std::function<void()>;

    AnimatedSprite();
    AnimatedSprite(const sf::Texture& texture, std::size_t frameWidth, std::size_t frameHeight, float frameTime = 0.1f);

    void setTexture(const sf::Texture& texture, std::size_t frameWidth, std::size_t frameHeight);
    void addFrame(const sf::IntRect& rect);
    void setFrameTime(float time);
    void setLooped(bool looped);
    void setCallback(Callback cb);

    void play();
    void pause();
    void stop();
    void reset();

    bool isPlaying() const;
    bool isLooped() const;
    std::size_t getFrameCount() const;
    std::size_t getCurrentFrame() const;

    void update(float deltaTime);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<sf::IntRect> m_frames;
    const sf::Texture* m_texture = nullptr;
    float m_frameTime = 0.1f;
    float m_currentTime = 0.f;
    std::size_t m_currentFrame = 0;
    std::atomic<bool> m_playing{false};
    std::atomic<bool> m_looped{true};
    mutable std::mutex m_mutex;
    Callback m_callback = nullptr;
    sf::Sprite m_sprite;
};
