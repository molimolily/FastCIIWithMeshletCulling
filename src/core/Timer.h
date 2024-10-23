#include <iostream>
#include <chrono>

class Timer {
public:
    Timer() : lastFrameTime(std::chrono::steady_clock::now()), deltaTime(0.0f), frameCount(0), fps(0.0f), fpsAccumulator(0.0f), outputInterval(1.0f), timeSinceLastOutput(0.0f) {}

    void Update(bool fpsOutput = false) {
        auto currentFrameTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> frameTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        deltaTime = frameTime.count();
        CalculateFPS(fpsOutput);
    }

    float GetDeltaTime() const {
        return deltaTime;
    }

    float GetFPS() const {
        return fps;
    }

private:
    std::chrono::steady_clock::time_point lastFrameTime;
    float deltaTime;
    int frameCount;
    float fps;
    float fpsAccumulator;
    float outputInterval; // FPS出力間隔（秒）
    float timeSinceLastOutput; // 最後のFPS出力からの経過時間

    void CalculateFPS(bool fpsOutput) {
        frameCount++;
        fpsAccumulator += deltaTime;
        timeSinceLastOutput += deltaTime;

        if (timeSinceLastOutput >= outputInterval) {
            fps = frameCount / fpsAccumulator;
            if(fpsOutput)
                std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            fpsAccumulator = 0.0f;
            timeSinceLastOutput = 0.0f;
        }
    }
};
