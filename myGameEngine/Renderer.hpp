//
//  Renderer.hpp
//  myGameEngine
//
//  Created by Anna Dai on 2025/6/17.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

class Renderer {
public:
    Renderer(MTL::Device * const pDevice);
    ~Renderer();
    void drawFrame(const CA::MetalDrawable * const pDrawable);
private:
    MTL::Device * const _pDevice;
    MTL::CommandQueue * const _pCommandQueue;
    std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState *)> _pRenderPipelineState;
    float _timer;
    
    void buildShaders();
};

#endif /* Renderer_hpp */
