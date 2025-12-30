/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AnimatedSprite.cpp
*/

#include "AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite() = default;

AnimatedSprite::AnimatedSprite(const sf::Texture &texture, std::size_t frameWidth, std::size_t frameHeight, float frameTime)
    : m_texture(&texture), m_frameTime(frameTime)
{
    setTexture(texture, frameWidth, frameHeight);
}

void AnimatedSprite::setTexture(const sf::Texture &texture, std::size_t frameWidth, std::size_t frameHeight)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_texture = &texture;
    m_frames.clear();
    auto texSize = texture.getSize();
    for (std::size_t y = 0; y + frameHeight <= texSize.y; y += frameHeight)
        for (std::size_t x = 0; x + frameWidth <= texSize.x; x += frameWidth)
            m_frames.emplace_back(x, y, frameWidth, frameHeight);
    m_currentFrame = 0;
    m_sprite.setTexture(texture);
    if (!m_frames.empty())
        m_sprite.setTextureRect(m_frames[0]);
}

void AnimatedSprite::addFrame(const sf::IntRect &rect)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_frames.push_back(rect);
}

void AnimatedSprite::setFrameTime(float time)
{
    m_frameTime = time;
}

void AnimatedSprite::setLooped(bool looped)
{
    m_looped = looped;
}

void AnimatedSprite::setCallback(Callback cb)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_callback = std::move(cb);
}

void AnimatedSprite::play()
{
    m_playing = true;
}

void AnimatedSprite::pause()
{
    m_playing = false;
}

void AnimatedSprite::stop()
{
    m_playing = false;
    reset();
}

void AnimatedSprite::reset()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_currentFrame = 0;
    m_currentTime = 0.f;
    if (!m_frames.empty())
        m_sprite.setTextureRect(m_frames[0]);
}

bool AnimatedSprite::isPlaying() const
{
    return m_playing;
}

bool AnimatedSprite::isLooped() const
{
    return m_looped;
}

std::size_t AnimatedSprite::getFrameCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_frames.size();
}

std::size_t AnimatedSprite::getCurrentFrame() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_currentFrame;
}

void AnimatedSprite::update(float deltaTime)
{
    if (!m_playing || m_frames.empty())
        return;
    m_currentTime += deltaTime;
    while (m_currentTime >= m_frameTime)
    {
        m_currentTime -= m_frameTime;
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_currentFrame + 1 < m_frames.size())
        {
            ++m_currentFrame;
        }
        else if (m_looped)
        {
            m_currentFrame = 0;
            if (m_callback)
                m_callback();
        }
        else
        {
            m_playing = false;
            if (m_callback)
                m_callback();
            break;
        }
        m_sprite.setTextureRect(m_frames[m_currentFrame]);
    }
}

void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    states.transform *= getTransform();
    target.draw(m_sprite, states);
}
